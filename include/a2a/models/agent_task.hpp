// 概述: 定义 Agent 任务模型，包含状态、产物与历史消息，支持 JSON 序列化。
#pragma once

#include "task_status.hpp"
#include "artifact.hpp"
#include "agent_message.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace a2a {

/**
 * @brief Agent Task - represents a task that can be processed by an agent
 */
class AgentTask {
public:
    // 默认构造任务对象。
    AgentTask() = default;
    
    // 构造带任务 ID 与上下文 ID 的任务。
    AgentTask(const std::string& id, const std::string& context_id)
        : id_(id)
        , context_id_(context_id)
        , status_(TaskState::Submitted) {}
    
    // 获取任务 ID。
    const std::string& id() const { return id_; }
    // 获取上下文 ID。
    const std::string& context_id() const { return context_id_; }
    // 获取任务状态对象。
    const AgentTaskStatus& status() const { return status_; }
    // 获取任务产物列表。
    const std::vector<Artifact>& artifacts() const { return artifacts_; }
    // 获取历史消息列表。
    const std::vector<AgentMessage>& history() const { return history_; }
    // 获取元数据映射。
    const std::map<std::string, std::string>& metadata() const { return metadata_; }
    
    // 设置任务 ID。
    void set_id(const std::string& id) { id_ = id; }
    // 设置上下文 ID。
    void set_context_id(const std::string& context_id) { context_id_ = context_id; }
    // 设置任务状态对象。
    void set_status(const AgentTaskStatus& status) { status_ = status; }
    // 设置任务状态枚举。
    void set_status(TaskState state) { status_ = AgentTaskStatus(state); }
    
    /**
     * @brief Add an artifact to the task
     */
    // 追加任务产物。
    void add_artifact(const Artifact& artifact) {
        artifacts_.push_back(artifact);
    }
    
    /**
     * @brief Add a message to the history
     */
    // 追加历史消息。
    void add_history_message(const AgentMessage& message) {
        history_.push_back(message);
    }
    
    /**
     * @brief Add metadata
     */
    // 添加/更新元数据键值。
    void add_metadata(const std::string& key, const std::string& value) {
        metadata_[key] = value;
    }
    
    /**
     * @brief Check if task is in terminal state
     */
    // 判断任务是否终止状态。
    bool is_terminal() const {
        return status_.is_terminal();
    }
    
    /**
     * @brief Serialize to JSON
     */
    // 序列化任务为 JSON。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON
     */
    // 从 JSON 反序列化任务。
    static AgentTask from_json(const std::string& json);
    
    /**
     * @brief Create a new AgentTask
     */
    // 创建空任务对象。
    static AgentTask create() {
        return AgentTask();
    }
    
    /**
     * @brief Fluent API methods
     */
    // 链式设置任务 ID。
    AgentTask& with_id(const std::string& id) {
        id_ = id;
        return *this;
    }
    
    // 链式设置上下文 ID。
    AgentTask& with_context_id(const std::string& context_id) {
        context_id_ = context_id;
        return *this;
    }
    
    // 链式设置任务状态。
    AgentTask& with_status(TaskState state) {
        status_ = AgentTaskStatus(state);
        return *this;
    }
    
    // 链式追加产物。
    AgentTask& with_artifact(const Artifact& artifact) {
        artifacts_.push_back(artifact);
        return *this;
    }
    
    // 链式追加历史消息。
    AgentTask& with_history_message(const AgentMessage& message) {
        history_.push_back(message);
        return *this;
    }

private:
    std::string id_;
    std::string context_id_;
    AgentTaskStatus status_;
    std::vector<Artifact> artifacts_;
    std::vector<AgentMessage> history_;
    std::map<std::string, std::string> metadata_;
};

} // namespace a2a
