// 概述: 封装基于 libcurl 的 HTTP 客户端与响应结构，提供 GET/POST/流式 POST 能力。
#pragma once

#include <string>
#include <map>
#include <memory>
#include <functional>

namespace a2a {

/**
 * @brief HTTP Response
 */
struct HttpResponse {
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
    
    // 判断 HTTP 状态码是否为 2xx 成功。
    bool is_success() const {
        return status_code >= 200 && status_code < 300;
    }
};

/**
 * @brief HTTP Client wrapper (uses libcurl internally)
 */
class HttpClient {
public:
    // 构造 HTTP 客户端，初始化 libcurl 资源（非常重要）。
    HttpClient();
    // 析构 HTTP 客户端，释放 libcurl 资源。
    ~HttpClient();
    
    // Disable copy, enable move
    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;
    // 移动构造以转移底层 libcurl 句柄。
    HttpClient(HttpClient&&) noexcept;
    // 移动赋值以转移底层 libcurl 句柄。
    HttpClient& operator=(HttpClient&&) noexcept;
    
    /**
     * @brief Perform GET request
     */
    // 发起 GET 请求并返回响应体与状态码。
    HttpResponse get(const std::string& url);
    
    /**
     * @brief Perform POST request
     */
    // 发起 POST 请求并返回响应体与状态码（非常重要）。
    HttpResponse post(const std::string& url, 
                     const std::string& body,
                     const std::string& content_type = "application/json");
    
    /**
     * @brief Perform POST request with streaming response
     * @param callback Called for each chunk of data received
     */
    // 发起流式 POST 请求，按块回调处理响应数据。
    void post_stream(const std::string& url,
                    const std::string& body,
                    const std::string& content_type,
                    std::function<void(const std::string&)> callback);
    
    /**
     * @brief Set request timeout in seconds
     */
    // 设置请求超时时间（秒）。
    void set_timeout(long seconds);
    
    /**
     * @brief Add custom header
     */
    // 添加自定义 HTTP 头。
    void add_header(const std::string& key, const std::string& value);
    
    /**
     * @brief Clear all custom headers
     */
    // 清空所有自定义 HTTP 头。
    void clear_headers();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace a2a
