// 概述: 定义消息发送与任务查询参数模型，包含 JSON 序列化接口与链式设置方法。
#pragma once

#include "agent_message.hpp"
#include <optional>

namespace a2a {

/**
 * @brief Parameters for sending a message to an agent
 */
class MessageSendParams {
public:
    // 默认构造参数对象。
    MessageSendParams() = default;
    
    // 构造并绑定消息对象。
    explicit MessageSendParams(const AgentMessage& message)
        : message_(message) {}
    
    // 获取消息对象。
    const AgentMessage& message() const { return message_; }
    // 获取历史长度限制（可选）。
    const std::optional<int>& history_length() const { return history_length_; }
    // 获取上下文 ID（可选）。
    const std::optional<std::string>& context_id() const { return context_id_; }
    // 获取任务 ID（可选）。
    const std::optional<std::string>& task_id() const { return task_id_; }
    
    // 设置消息对象。
    void set_message(const AgentMessage& message) { message_ = message; }
    // 设置历史消息长度限制。
    void set_history_length(int length) { history_length_ = length; }
    // 设置上下文 ID。
    void set_context_id(const std::string& id) { context_id_ = id; }
    // 设置任务 ID。
    void set_task_id(const std::string& id) { task_id_ = id; }
    
    /**
     * @brief Serialize to JSON
     */
    // 序列化参数为 JSON。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON
     */
    // 从 JSON 反序列化参数。
    static MessageSendParams from_json(const std::string& json);
    
    /**
     * @brief Create a new MessageSendParams
     */
    // 创建空参数对象。
    static MessageSendParams create() {
        return MessageSendParams();
    }
    
    /**
     * @brief Fluent API methods
     */
    // 链式设置消息对象。
    MessageSendParams& with_message(const AgentMessage& message) {
        message_ = message;
        return *this;
    }
    
    // 链式设置历史长度限制。
    MessageSendParams& with_history_length(int length) {
        history_length_ = length;
        return *this;
    }
    
    // 链式设置上下文 ID。
    MessageSendParams& with_context_id(const std::string& id) {
        context_id_ = id;
        return *this;
    }
    
    // 链式设置任务 ID。
    MessageSendParams& with_task_id(const std::string& id) {
        task_id_ = id;
        return *this;
    }

private:
    AgentMessage message_;
    std::optional<int> history_length_;
    std::optional<std::string> context_id_;
    std::optional<std::string> task_id_;
};

/**
 * @brief Parameters for querying a task
 */
struct TaskQueryParams {
    std::string id;
    std::optional<int> history_length;
    std::map<std::string, std::string> metadata;
    
    // 序列化任务查询参数为 JSON。
    std::string to_json() const;
    // 从 JSON 反序列化任务查询参数。
    static TaskQueryParams from_json(const std::string& json);
};

/**
 * @brief Parameters for task ID operations
 */
struct TaskIdParams {
    std::string id;
    
    // 序列化任务 ID 参数为 JSON。
    std::string to_json() const;
    // 从 JSON 反序列化任务 ID 参数。
    static TaskIdParams from_json(const std::string& json);
};

} // namespace a2a
