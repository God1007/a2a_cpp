// 概述: 定义 A2A 响应联合类型，封装任务或消息结果。
#pragma once

#include "agent_task.hpp"
#include "agent_message.hpp"

namespace a2a {

/**
 * @brief A2A Response - can be either AgentTask or AgentMessage
 */
class A2AResponse {
public:
    enum class Type {
        Task,
        Message
    };
    
    // 构造任务类型响应。
    A2AResponse(const AgentTask& task)
        : type_(Type::Task), task_(task), message_() {}
    
    // 构造消息类型响应。
    A2AResponse(const AgentMessage& message)
        : type_(Type::Message), task_(), message_(message) {}
    
    // 获取响应类型枚举。
    Type type() const { return type_; }
    
    // 判断是否为任务响应。
    bool is_task() const { return type_ == Type::Task; }
    // 判断是否为消息响应。
    bool is_message() const { return type_ == Type::Message; }
    
    // 以任务类型访问响应。
    const AgentTask& as_task() const { return task_; }
    // 以消息类型访问响应。
    const AgentMessage& as_message() const { return message_; }

private:
    Type type_;
    AgentTask task_;
    AgentMessage message_;
};

} // namespace a2a
