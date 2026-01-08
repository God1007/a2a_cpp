// 概述: 基于 libcurl 实现 HTTP 客户端，支持普通与流式请求并封装响应解析。
#include <a2a/core/http_client.hpp>
#include <a2a/core/exception.hpp>
#include <curl/curl.h>
#include <sstream>
#include <cstring>

namespace a2a {

// Callback for writing response data
// libcurl 写回调：聚合响应数据到字符串。
static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

// Callback for streaming data
// libcurl 流式回调：按块推送响应内容。
static size_t stream_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    auto callback = static_cast<std::function<void(const std::string&)>*>(userp);
    std::string chunk(static_cast<char*>(contents), total_size);
    (*callback)(chunk);
    return total_size;
}

// PIMPL implementation
class HttpClient::Impl {
public:
    // 构造实现对象并初始化 libcurl 全局状态。
    Impl() : timeout_(30L) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    
    // 析构实现对象并清理 libcurl 全局状态。
    ~Impl() {
        curl_global_cleanup();
    }
    
    long timeout_;
    std::map<std::string, std::string> headers_;
};

HttpClient::HttpClient() : impl_(std::make_unique<Impl>()) {}

// 默认析构，释放 PIMPL。
HttpClient::~HttpClient() = default;

// 移动构造，转移 PIMPL。
HttpClient::HttpClient(HttpClient&&) noexcept = default;
// 移动赋值，转移 PIMPL。
HttpClient& HttpClient::operator=(HttpClient&&) noexcept = default;

HttpResponse HttpClient::get(const std::string& url) {
    // 发起 GET 请求并返回状态码与响应体（非常重要）。
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw A2AException("Failed to initialize CURL", ErrorCode::InternalError);
    }
    
    std::string response_body;
    HttpResponse response;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, impl_->timeout_);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Add custom headers
    struct curl_slist* header_list = nullptr;
    for (const auto& [key, value] : impl_->headers_) {
        std::string header = key + ": " + value;
        header_list = curl_slist_append(header_list, header.c_str());
    }
    if (header_list) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        curl_slist_free_all(header_list);
        curl_easy_cleanup(curl);
        throw A2AException(
            std::string("CURL error: ") + curl_easy_strerror(res),
            ErrorCode::InternalError
        );
    }
    
    long status_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    
    response.status_code = static_cast<int>(status_code);
    response.body = response_body;
    
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl);
    
    return response;
}

HttpResponse HttpClient::post(const std::string& url,
                              const std::string& body,
                              const std::string& content_type) {
    // 发起 POST 请求并返回状态码与响应体（非常重要）。
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw A2AException("Failed to initialize CURL", ErrorCode::InternalError);
    }
    
    std::string response_body;
    HttpResponse response;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, impl_->timeout_);
    
    // Set headers
    struct curl_slist* header_list = nullptr;
    header_list = curl_slist_append(header_list, ("Content-Type: " + content_type).c_str());
    
    for (const auto& [key, value] : impl_->headers_) {
        std::string header = key + ": " + value;
        header_list = curl_slist_append(header_list, header.c_str());
    }
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        curl_slist_free_all(header_list);
        curl_easy_cleanup(curl);
        throw A2AException(
            std::string("CURL error: ") + curl_easy_strerror(res),
            ErrorCode::InternalError
        );
    }
    
    long status_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    
    response.status_code = static_cast<int>(status_code);
    response.body = response_body;
    
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl);
    
    return response;
}

void HttpClient::post_stream(const std::string& url,
                             const std::string& body,
                             const std::string& content_type,
                             std::function<void(const std::string&)> callback) {
    // 发起流式 POST 请求并按块回调处理响应。
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw A2AException("Failed to initialize CURL", ErrorCode::InternalError);
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, impl_->timeout_);
    
    // Set headers
    struct curl_slist* header_list = nullptr;
    header_list = curl_slist_append(header_list, ("Content-Type: " + content_type).c_str());
    header_list = curl_slist_append(header_list, "Accept: text/event-stream");
    
    for (const auto& [key, value] : impl_->headers_) {
        std::string header = key + ": " + value;
        header_list = curl_slist_append(header_list, header.c_str());
    }
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        throw A2AException(
            std::string("CURL error: ") + curl_easy_strerror(res),
            ErrorCode::InternalError
        );
    }
}

void HttpClient::set_timeout(long seconds) {
    // 设置 HTTP 超时秒数。
    impl_->timeout_ = seconds;
}

void HttpClient::add_header(const std::string& key, const std::string& value) {
    // 添加自定义请求头。
    impl_->headers_[key] = value;
}

void HttpClient::clear_headers() {
    // 清空所有自定义请求头。
    impl_->headers_.clear();
}

} // namespace a2a
