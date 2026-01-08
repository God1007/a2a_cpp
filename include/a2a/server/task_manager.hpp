// 概述: 定义任务管理器接口，负责任务生命周期与消息处理，结合 TaskStore 与回调机制实现。
#pragma once

#include "task_store.hpp"
#include "../models/agent_task.hpp"
#include "../models/agent_card.hpp"
#include "../models/agent_message.hpp"
#include "../models/message_send_params.hpp"
#include "../models/a2a_response.hpp"
#include <functional>
#include <memory>
#include <string>

namespace a2a {

/**
 * @brief Task Manager - manages the complete lifecycle of agent tasks
 */
class TaskManager {
public:
    /**
     * @brief Callback type for message received
     * Returns either AgentMessage or AgentTask
     */
    using MessageCallback = std::function<A2AResponse(const MessageSendParams&)>;
    
    /**
     * @brief Callback type for task lifecycle events
     */
    using TaskCallback = std::function<void(const AgentTask&)>;
    
    /**
     * @brief Callback type for agent card query
     */
    using AgentCardCallback = std::function<AgentCard(const std::string& agent_url)>;
    
    /**
     * @brief Construct with optional custom task store
     */
    // 构造任务管理器，可注入自定义 TaskStore（非常重要）。
    explicit TaskManager(std::shared_ptr<ITaskStore> task_store = nullptr);
    
    // 析构任务管理器，释放内部实现资源。
    ~TaskManager();
    
    // Disable copy, enable move
    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    // 移动构造以转移内部实现指针。
    TaskManager(TaskManager&&) noexcept;
    // 移动赋值以转移内部实现指针。
    TaskManager& operator=(TaskManager&&) noexcept;
    
    // === Lifecycle Callbacks ===
    
    /**
     * @brief Set callback for when a message is received
     * This is the main handler for agent logic
     */
    // 设置消息处理回调，承载 Agent 业务逻辑（非常重要）。
    void set_on_message_received(MessageCallback callback);
    
    /**
     * @brief Set callback for when a task is created
     */
    // 设置任务创建事件回调。
    void set_on_task_created(TaskCallback callback);
    
    /**
     * @brief Set callback for when a task is cancelled
     */
    // 设置任务取消事件回调。
    void set_on_task_cancelled(TaskCallback callback);
    
    /**
     * @brief Set callback for when a task is updated
     */
    // 设置任务更新事件回调。
    void set_on_task_updated(TaskCallback callback);
    
    /**
     * @brief Set callback for agent card queries
     */
    // 设置查询 AgentCard 的回调。
    void set_on_agent_card_query(AgentCardCallback callback);
    
    // === Task Operations ===
    
    /**
     * @brief Create a new task
     * @param context_id Optional context ID (generated if not provided)
     * @param task_id Optional task ID (generated if not provided)
     * @return Created task
     */
    // 创建新任务并返回对象（非常重要）。
    AgentTask create_task(const std::string& context_id = "",
                         const std::string& task_id = "");
    
    /**
     * @brief Get a task by ID
     * @param task_id Task identifier
     * @return Task if found
     * @throws A2AException if not found
     */
    // 按任务 ID 获取任务，未找到会抛异常。
    AgentTask get_task(const std::string& task_id);
    
    /**
     * @brief Cancel a task
     * @param task_id Task identifier
     * @return Updated task
     * @throws A2AException if task cannot be cancelled
     */
    // 取消指定任务并更新状态。
    AgentTask cancel_task(const std::string& task_id);
    
    /**
     * @brief Update task status
     * @param task_id Task identifier
     * @param status New status
     * @param message Optional message
     */
    // 更新任务状态并可附带消息。
    void update_status(const std::string& task_id,
                      TaskState status,
                      const AgentMessage* message = nullptr);
    
    /**
     * @brief Add artifact to task
     * @param task_id Task identifier
     * @param artifact Artifact to add
     */
    // 向任务返回产物/附件。
    void return_artifact(const std::string& task_id,
                        const Artifact& artifact);
    
    // === Message Processing ===
    
    /**
     * @brief Process a message (non-streaming)
     * @param params Message parameters
     * @return Response (Task or Message)
     */
    // 处理非流式消息请求并返回响应（非常重要）。
    A2AResponse send_message(const MessageSendParams& params);
    
    /**
     * @brief Process a message (streaming)
     * @param params Message parameters
     * @param callback Called for each event
     */
    // 处理流式消息请求，逐块回调输出结果（非常重要）。
    void send_message_streaming(const MessageSendParams& params,
                               std::function<void(const std::string&)> callback);
    
    /**
     * @brief Get agent card
     * @param agent_url Agent URL
     * @return AgentCard
     */
    // 获取指定 Agent 的卡片元数据。
    AgentCard get_agent_card(const std::string& agent_url);
    
    /**
     * @brief Get the task store
     * @return Shared pointer to task store
     */
    // 获取当前使用的 TaskStore 实例。
    std::shared_ptr<ITaskStore> get_task_store() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace a2a
