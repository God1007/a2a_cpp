// 概述: 提供基于内存的 TaskStore 实现，使用 mutex 保证线程安全。
#pragma once

#include "task_store.hpp"
#include <map>
#include <mutex>

namespace a2a {

/**
 * @brief In-memory implementation of ITaskStore
 * Thread-safe using mutex
 */
class MemoryTaskStore : public ITaskStore {
public:
    // 默认构造内存存储。
    MemoryTaskStore() = default;
    // 默认析构。
    ~MemoryTaskStore() override = default;
    
    // ITaskStore implementation
    // 从内存中按 ID 读取任务。
    std::optional<AgentTask> get_task(const std::string& task_id) override;
    
    // 将任务写入内存映射表。
    void set_task(const AgentTask& task) override;
    
    // 更新任务状态并可记录消息。
    void update_status(const std::string& task_id,
                      TaskState status,
                      const std::string& message = "") override;
    
    // 追加任务产物。
    void add_artifact(const std::string& task_id,
                     const Artifact& artifact) override;
    
    // 追加历史消息。
    void add_history_message(const std::string& task_id,
                            const AgentMessage& message) override;
    
    // 获取历史消息列表（可限制长度）。
    std::vector<AgentMessage> get_history(const std::string& context_id,
                                          int max_length = 0) override;
    
    // 删除任务并返回是否删除成功。
    bool delete_task(const std::string& task_id) override;
    
    // 检查任务是否存在。
    bool task_exists(const std::string& task_id) override;
    
    /**
     * @brief Get number of tasks in store
     */
    // 获取当前内存中任务数量。
    size_t size() const;
    
    /**
     * @brief Clear all tasks
     */
    // 清空所有任务记录。
    void clear();

private:
    mutable std::mutex mutex_;
    std::map<std::string, AgentTask> tasks_;
};

} // namespace a2a
