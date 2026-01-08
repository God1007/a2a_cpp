// 概述: 定义 AgentCard 解析器，通过 HTTP 拉取并解析 AgentCard 信息。
#pragma once

#include "../models/agent_card.hpp"
#include "../core/http_client.hpp"
#include <string>
#include <memory>

namespace a2a {

/**
 * @brief Resolves Agent Card information from an A2A-compatible endpoint
 */
class A2ACardResolver {
public:
    /**
     * @brief Construct resolver with base URL
     * @param base_url Base URL of the agent service
     * @param agent_card_path Path to agent card (default: "/.well-known/agent-card.json")
     */
    // 构造解析器并指定 AgentCard 路径（非常重要）。
    explicit A2ACardResolver(const std::string& base_url,
                            const std::string& agent_card_path = "/.well-known/agent-card.json");
    
    // 析构解析器，释放 HTTP 资源。
    ~A2ACardResolver();
    
    // Disable copy, enable move
    A2ACardResolver(const A2ACardResolver&) = delete;
    A2ACardResolver& operator=(const A2ACardResolver&) = delete;
    // 移动构造以转移内部实现。
    A2ACardResolver(A2ACardResolver&&) noexcept;
    // 移动赋值以转移内部实现。
    A2ACardResolver& operator=(A2ACardResolver&&) noexcept;
    
    /**
     * @brief Get the agent card asynchronously
     * @return AgentCard object
     * @throws A2AException if request fails or JSON is invalid
     */
    // 获取并解析 AgentCard（非常重要）。
    AgentCard get_agent_card();
    
    /**
     * @brief Get the full agent card URL
     */
    // 获取 AgentCard 完整 URL。
    std::string get_agent_card_url() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace a2a
