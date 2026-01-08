// 概述: 定义 Agent 消息模型与分片管理，支持 JSON 序列化与深拷贝语义。
#pragma once

#include "../core/types.hpp"
#include "message_part.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <ctime>

namespace a2a {

/**
 * @brief Agent Message - represents a message in the A2A protocol
 */
class AgentMessage {
public:
    // 默认构造消息对象。
    AgentMessage() = default;
    
    // Copy constructor
    // 深拷贝构造，复制消息分片内容。
    AgentMessage(const AgentMessage& other)
        : message_id_(other.message_id_)
        , context_id_(other.context_id_)
        , task_id_(other.task_id_)
        , role_(other.role_)
    {
        // Deep copy parts
        for (const auto& part : other.parts_) {
            parts_.push_back(part->clone());
        }
    }
    
    // Copy assignment
    // 深拷贝赋值，复制消息分片内容。
    AgentMessage& operator=(const AgentMessage& other) {
        if (this != &other) {
            message_id_ = other.message_id_;
            context_id_ = other.context_id_;
            task_id_ = other.task_id_;
            role_ = other.role_;
            
            // Deep copy parts
            parts_.clear();
            for (const auto& part : other.parts_) {
                parts_.push_back(part->clone());
            }
        }
        return *this;
    }
    
    // Move constructor and assignment (default)
    // 移动构造，转移资源所有权。
    AgentMessage(AgentMessage&&) = default;
    // 移动赋值，转移资源所有权。
    AgentMessage& operator=(AgentMessage&&) = default;
    
    // 获取消息 ID。
    const std::string& message_id() const { return message_id_; }
    // 获取上下文 ID（可选）。
    const std::optional<std::string>& context_id() const { return context_id_; }
    // 获取任务 ID（可选）。
    const std::optional<std::string>& task_id() const { return task_id_; }
    // 获取消息角色。
    MessageRole role() const { return role_; }
    // 获取分片集合。
    const std::vector<std::unique_ptr<Part>>& parts() const { return parts_; }
    
    // 设置消息 ID。
    void set_message_id(const std::string& id) { message_id_ = id; }
    // 设置上下文 ID。
    void set_context_id(const std::string& id) { context_id_ = id; }
    // 设置任务 ID。
    void set_task_id(const std::string& id) { task_id_ = id; }
    // 设置消息角色。
    void set_role(MessageRole role) { role_ = role; }
    
    /**
     * @brief Add a text part to the message
     */
    // 向消息追加文本分片。
    void add_text_part(const std::string& text) {
        parts_.push_back(std::make_unique<TextPart>(text));
    }
    
    /**
     * @brief Add a file part to the message
     */
    // 向消息追加文件分片。
    void add_file_part(const std::string& filename, 
                      const std::string& mime_type,
                      const std::vector<uint8_t>& data) {
        parts_.push_back(std::make_unique<FilePart>(filename, mime_type, data));
    }
    
    /**
     * @brief Add a data part to the message
     */
    // 向消息追加结构化数据分片。
    void add_data_part(const std::string& data_json) {
        parts_.push_back(std::make_unique<DataPart>(data_json));
    }
    
    /**
     * @brief Add a part (takes ownership)
     */
    // 追加任意分片并接管所有权。
    void add_part(std::unique_ptr<Part> part) {
        parts_.push_back(std::move(part));
    }
    
    /**
     * @brief Get the first text part content (convenience method)
     */
    // 获取首个文本分片内容，便于快速读取文本消息（非常重要）。
    std::string get_text() const {
        for (const auto& part : parts_) {
            if (part->kind() == PartKind::Text) {
                return static_cast<const TextPart*>(part.get())->text();
            }
        }
        return "";
    }
    
    /**
     * @brief Serialize to JSON
     */
    // 序列化消息为 JSON。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON
     */
    // 从 JSON 反序列化消息。
    static AgentMessage from_json(const std::string& json);
    
    /**
     * @brief Create a new AgentMessage with default values
     */
    // 创建带默认 ID 的消息对象。
    static AgentMessage create() {
        AgentMessage msg;
        msg.message_id_ = "msg-" + std::to_string(std::time(nullptr));
        return msg;
    }
    
    /**
     * @brief Fluent API methods for building messages
     */
    // 链式设置消息 ID。
    AgentMessage& with_message_id(const std::string& id) {
        message_id_ = id;
        return *this;
    }
    
    // 链式设置上下文 ID。
    AgentMessage& with_context_id(const std::string& id) {
        context_id_ = id;
        return *this;
    }
    
    // 链式设置任务 ID。
    AgentMessage& with_task_id(const std::string& id) {
        task_id_ = id;
        return *this;
    }
    
    // 链式设置角色。
    AgentMessage& with_role(MessageRole role) {
        role_ = role;
        return *this;
    }
    
    // 链式添加文本分片。
    AgentMessage& with_text(const std::string& text) {
        add_text_part(text);
        return *this;
    }

private:
    std::string message_id_;
    std::optional<std::string> context_id_;
    std::optional<std::string> task_id_;
    MessageRole role_ = MessageRole::User;
    std::vector<std::unique_ptr<Part>> parts_;
};

} // namespace a2a
