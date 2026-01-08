// 概述: 定义 TaskStore 接口，用于任务与历史的持久化，基于 C++17 optional 与模型类型。
#pragma once

#include "../models/agent_task.hpp"
#include <string>
#include <optional>
#include <memory>

namespace a2a {

/**
 * @brief Interface for storing and retrieving agent tasks
 */
class ITaskStore {
public:
    // 虚析构，保证派生类正确释放资源。
    virtual ~ITaskStore() = default;
    
    /**
     * @brief Retrieve a task by its ID
     * @param task_id Task identifier
     * @return Task if found, nullopt otherwise
     */
    // 按任务 ID 读取任务对象（非常重要）。
    virtual std::optional<AgentTask> get_task(const std::string& task_id) = 0;
    
    /**
     * @brief Store or update a task
     * @param task Task to store
     */
    // 保存或更新任务数据（非常重要）。
    virtual void set_task(const AgentTask& task) = 0;
    
    /**
     * @brief Update task status
     * @param task_id Task identifier
     * @param status New status
     * @param message Optional message associated with status
     */
    // 更新任务状态与附加信息。
    virtual void update_status(const std::string& task_id,
                              TaskState status,
                              const std::string& message = "") = 0;
    
    /**
     * @brief Add artifact to task
     * @param task_id Task identifier
     * @param artifact Artifact to add
     */
    // 向任务添加产物/附件。
    virtual void add_artifact(const std::string& task_id,
                             const Artifact& artifact) = 0;
    
    /**
     * @brief Add message to task history
     * @param task_id Task identifier
     * @param message Message to add
     */
    // 向任务历史追加消息。
    virtual void add_history_message(const std::string& task_id,
                                    const AgentMessage& message) = 0;
    
    /**
     * @brief Get history messages for a task/context
     * @param context_id Context identifier (or task_id)
     * @param max_length Maximum number of messages to return (0 = all)
     * @return Vector of history messages
     */
    // 读取上下文/任务的历史消息集合（非常重要）。
    virtual std::vector<AgentMessage> get_history(const std::string& context_id,
                                                   int max_length = 0) = 0;
    
    /**
     * @brief Delete a task
     * @param task_id Task identifier
     * @return true if task was deleted, false if not found
     */
    // 删除任务并返回是否成功。
    virtual bool delete_task(const std::string& task_id) = 0;
    
    /**
     * @brief Check if task exists
     * @param task_id Task identifier
     * @return true if task exists
     */
    // 判断任务是否存在。
    virtual bool task_exists(const std::string& task_id) = 0;
};

} // namespace a2a
