// 概述: 定义 SDK 通用类型、枚举与字符串转换工具，依赖 C++17 标准库时间与容器。
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <map>
#include <chrono>

namespace a2a {

// Forward declarations
class AgentTask;
class AgentMessage;
class AgentCard;
class MessageSendParams;
class A2AResponse;

// Type aliases
using Timestamp = std::chrono::system_clock::time_point;
using Json = std::map<std::string, std::string>; // Simplified, use nlohmann::json in practice
using Callback = std::function<void()>;

// Enumerations
enum class MessageRole {
    User,
    Agent,
    System
};

enum class TaskState {
    Submitted,
    Running,
    Completed,
    Failed,
    Canceled,
    Rejected
};

enum class PartKind {
    Text,
    File,
    Data
};

enum class AgentTransport {
    JsonRpc,
    Http
};

// Helper functions
// 将消息角色枚举转换为协议字符串。
inline std::string to_string(MessageRole role) {
    switch (role) {
        case MessageRole::User: return "user";
        case MessageRole::Agent: return "agent";
        case MessageRole::System: return "system";
        default: return "unknown";
    }
}

// 将任务状态枚举转换为协议字符串。
inline std::string to_string(TaskState state) {
    switch (state) {
        case TaskState::Submitted: return "submitted";
        case TaskState::Running: return "running";
        case TaskState::Completed: return "completed";
        case TaskState::Failed: return "failed";
        case TaskState::Canceled: return "canceled";
        case TaskState::Rejected: return "rejected";
        default: return "unknown";
    }
}

// 从协议字符串解析消息角色枚举。
inline MessageRole message_role_from_string(const std::string& str) {
    if (str == "user") return MessageRole::User;
    if (str == "agent") return MessageRole::Agent;
    if (str == "system") return MessageRole::System;
    return MessageRole::User;
}

// 从协议字符串解析任务状态枚举。
inline TaskState task_state_from_string(const std::string& str) {
    if (str == "submitted") return TaskState::Submitted;
    if (str == "running") return TaskState::Running;
    if (str == "completed") return TaskState::Completed;
    if (str == "failed") return TaskState::Failed;
    if (str == "canceled") return TaskState::Canceled;
    if (str == "rejected") return TaskState::Rejected;
    return TaskState::Submitted;
}

} // namespace a2a
