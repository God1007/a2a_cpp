// 概述: 实现 AgentCard 解析器，通过 HTTP GET 拉取并解析 AgentCard JSON。
#include <a2a/client/card_resolver.hpp>
#include <a2a/core/exception.hpp>

namespace a2a {

// PIMPL implementation
class A2ACardResolver::Impl {
public:
    // 构造实现对象并拼接 AgentCard URL。
    Impl(const std::string& base_url, const std::string& agent_card_path)
        : base_url_(base_url)
        , agent_card_path_(agent_card_path)
        , http_client_() {
        
        // Construct full URL
        if (base_url_.back() == '/') {
            base_url_.pop_back();
        }
        
        if (agent_card_path_.front() != '/') {
            agent_card_url_ = base_url_ + "/" + agent_card_path_;
        } else {
            agent_card_url_ = base_url_ + agent_card_path_;
        }
    }
    
    std::string base_url_;
    std::string agent_card_path_;
    std::string agent_card_url_;
    HttpClient http_client_;
};

A2ACardResolver::A2ACardResolver(const std::string& base_url,
                                 const std::string& agent_card_path)
    // 构造解析器并创建内部实现。
    : impl_(std::make_unique<Impl>(base_url, agent_card_path)) {}

// 默认析构。
A2ACardResolver::~A2ACardResolver() = default;

// 移动构造。
A2ACardResolver::A2ACardResolver(A2ACardResolver&&) noexcept = default;
// 移动赋值。
A2ACardResolver& A2ACardResolver::operator=(A2ACardResolver&&) noexcept = default;

AgentCard A2ACardResolver::get_agent_card() {
    // 请求并解析 AgentCard（非常重要）。
    try {
        // Perform GET request
        auto response = impl_->http_client_.get(impl_->agent_card_url_);
        
        // Check response status
        if (!response.is_success()) {
            throw A2AException(
                "Failed to fetch agent card: HTTP " + std::to_string(response.status_code),
                ErrorCode::InternalError
            );
        }
        
        // Parse JSON response
        AgentCard card = AgentCard::from_json(response.body);
        
        return card;
        
    } catch (const A2AException&) {
        throw;
    } catch (const std::exception& e) {
        throw A2AException(
            std::string("Failed to get agent card: ") + e.what(),
            ErrorCode::InternalError
        );
    }
}

std::string A2ACardResolver::get_agent_card_url() const {
    // 返回 AgentCard 的完整 URL。
    return impl_->agent_card_url_;
}

} // namespace a2a
