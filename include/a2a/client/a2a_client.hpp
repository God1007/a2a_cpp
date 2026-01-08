// 概述: 定义 A2A 客户端接口，基于 HTTP/JSON-RPC 与 Agent 服务交互。
#pragma once

#include "../models/agent_task.hpp"
#include "../models/agent_message.hpp"
#include "../models/message_send_params.hpp"
#include "../models/a2a_response.hpp"
#include "../core/http_client.hpp"
#include <string>
#include <memory>
#include <functional>

namespace a2a {

/**
 * @brief Main A2A Client for communicating with agents
 */
class A2AClient {
public:
    /**
     * @brief Construct client with agent base URL
     * @param base_url Base URL of the agent service
     */
    // 构造客户端并指定 Agent 服务基址（非常重要）。
    explicit A2AClient(const std::string& base_url);
    
    // 析构客户端，释放内部 HTTP 资源。
    ~A2AClient();
    
    // Disable copy, enable move
    A2AClient(const A2AClient&) = delete;
    A2AClient& operator=(const A2AClient&) = delete;
    // 移动构造以转移内部实现。
    A2AClient(A2AClient&&) noexcept;
    // 移动赋值以转移内部实现。
    A2AClient& operator=(A2AClient&&) noexcept;
    
    /**
     * @brief Send a non-streaming message request
     * @param params Message parameters
     * @return A2AResponse containing Task or Message
     * @throws A2AException on error
     */
    // 发送非流式消息请求并返回响应（非常重要）。
    A2AResponse send_message(const MessageSendParams& params);
    
    /**
     * @brief Send a streaming message request
     * @param params Message parameters
     * @param callback Called for each event received (Task, Message, or status update)
     * @throws A2AException on error
     */
    // 发送流式消息请求并按事件回调处理响应。
    void send_message_streaming(const MessageSendParams& params,
                               std::function<void(const std::string&)> callback);
    
    /**
     * @brief Get a task by ID
     * @param task_id Task identifier
     * @return AgentTask object
     * @throws A2AException if task not found
     */
    // 获取指定任务详情。
    AgentTask get_task(const std::string& task_id);
    
    /**
     * @brief Cancel a task
     * @param task_id Task identifier
     * @return Updated AgentTask
     * @throws A2AException if task cannot be cancelled
     */
    // 取消指定任务并返回更新后的状态。
    AgentTask cancel_task(const std::string& task_id);
    
    /**
     * @brief Subscribe to task updates (streaming)
     * @param task_id Task identifier
     * @param callback Called for each event received
     * @throws A2AException on error
     */
    // 订阅任务更新流并回调处理事件。
    void subscribe_to_task(const std::string& task_id,
                          std::function<void(const std::string&)> callback);
    
    /**
     * @brief Set request timeout
     * @param seconds Timeout in seconds
     */
    // 设置请求超时时间。
    void set_timeout(long seconds);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace a2a
