// 概述: 定义任务状态模型，记录状态、时间戳与可选消息，依赖 C++17 chrono。
#pragma once

#include "../core/types.hpp"
#include <string>
#include <chrono>

namespace a2a {

/**
 * @brief Agent Task Status
 */
class AgentTaskStatus {
public:
    // 默认构造为 Submitted 状态并记录当前时间。
    AgentTaskStatus() 
        : state_(TaskState::Submitted)
        , timestamp_(std::chrono::system_clock::now()) {}
    
    // 构造指定状态并记录当前时间。
    explicit AgentTaskStatus(TaskState state)
        : state_(state)
        , timestamp_(std::chrono::system_clock::now()) {}
    
    // 构造指定状态与时间戳。
    AgentTaskStatus(TaskState state, const Timestamp& timestamp)
        : state_(state)
        , timestamp_(timestamp) {}
    
    // 获取当前任务状态。
    TaskState state() const { return state_; }
    // 获取状态时间戳。
    const Timestamp& timestamp() const { return timestamp_; }
    // 获取状态附加消息。
    const std::string& message() const { return message_; }
    
    // 设置任务状态。
    void set_state(TaskState state) { state_ = state; }
    // 设置时间戳。
    void set_timestamp(const Timestamp& ts) { timestamp_ = ts; }
    // 设置附加消息。
    void set_message(const std::string& msg) { message_ = msg; }
    
    /**
     * @brief Check if task is in terminal state
     */
    // 判断任务是否处于终止状态。
    bool is_terminal() const {
        return state_ == TaskState::Completed ||
               state_ == TaskState::Failed ||
               state_ == TaskState::Canceled ||
               state_ == TaskState::Rejected;
    }
    
    /**
     * @brief Serialize to JSON
     */
    // 序列化任务状态为 JSON。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON
     */
    // 从 JSON 反序列化任务状态。
    static AgentTaskStatus from_json(const std::string& json);

private:
    TaskState state_;
    Timestamp timestamp_;
    std::string message_;  // Optional status message
};

} // namespace a2a
