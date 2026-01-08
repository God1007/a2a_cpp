// 概述: 内存版 TaskStore 实现，使用互斥锁保障线程安全的任务与历史操作。
#include <a2a/server/memory_task_store.hpp>
#include <algorithm>

namespace a2a {

std::optional<AgentTask> MemoryTaskStore::get_task(const std::string& task_id) {
    // 读取指定任务，未找到则返回空（非常重要）。
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = tasks_.find(task_id);
    if (it != tasks_.end()) {
        return it->second;
    }
    
    return std::nullopt;
}

void MemoryTaskStore::set_task(const AgentTask& task) {
    // 写入或更新任务对象。
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_[task.id()] = task;
}

void MemoryTaskStore::update_status(const std::string& task_id,
                                    TaskState status,
                                    const std::string& message) {
    // 更新任务状态并可附加消息。
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = tasks_.find(task_id);
    if (it != tasks_.end()) {
        AgentTaskStatus new_status(status);
        if (!message.empty()) {
            new_status.set_message(message);
        }
        it->second.set_status(new_status);
    }
}

void MemoryTaskStore::add_artifact(const std::string& task_id,
                                   const Artifact& artifact) {
    // 追加任务产物。
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = tasks_.find(task_id);
    if (it != tasks_.end()) {
        it->second.add_artifact(artifact);
    }
}

void MemoryTaskStore::add_history_message(const std::string& task_id,
                                          const AgentMessage& message) {
    // 追加历史消息。
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = tasks_.find(task_id);
    if (it != tasks_.end()) {
        it->second.add_history_message(message);
    }
}

std::vector<AgentMessage> MemoryTaskStore::get_history(const std::string& context_id,
                                                        int max_length) {
    // 读取历史消息并按长度截断（非常重要）。
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 查找对应的 task
    auto it = tasks_.find(context_id);
    if (it == tasks_.end()) {
        return {};
    }
    
    const auto& history = it->second.history();
    
    // 如果 max_length <= 0 或大于历史长度，返回全部
    if (max_length <= 0 || static_cast<size_t>(max_length) >= history.size()) {
        return history;
    }
    
    // 返回最近的 max_length 条消息
    return std::vector<AgentMessage>(
        history.end() - max_length,
        history.end()
    );
}

bool MemoryTaskStore::delete_task(const std::string& task_id) {
    // 删除任务并返回是否删除成功。
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = tasks_.find(task_id);
    if (it != tasks_.end()) {
        tasks_.erase(it);
        return true;
    }
    
    return false;
}

bool MemoryTaskStore::task_exists(const std::string& task_id) {
    // 判断任务是否存在。
    std::lock_guard<std::mutex> lock(mutex_);
    return tasks_.find(task_id) != tasks_.end();
}

size_t MemoryTaskStore::size() const {
    // 获取任务数量。
    std::lock_guard<std::mutex> lock(mutex_);
    return tasks_.size();
}

void MemoryTaskStore::clear() {
    // 清空所有任务。
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.clear();
}

} // namespace a2a
