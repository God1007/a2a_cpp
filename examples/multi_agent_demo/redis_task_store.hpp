// 概述: Redis 版 TaskStore，实现跨进程任务与历史共享，基于 hiredis 客户端。
#pragma once

#include <a2a/server/task_store.hpp>
#include <hiredis/hiredis.h>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <stdexcept>

namespace a2a {

/**
 * @brief Redis-based TaskStore implementation for distributed deployment
 * 
 * This implementation uses Redis to store tasks and history messages,
 * allowing multiple agents in different processes/machines to share the same storage.
 */
class RedisTaskStore : public ITaskStore {
public:
    /**
     * @brief Construct with Redis connection parameters
     * @param host Redis host (default: localhost)
     * @param port Redis port (default: 6379)
     */
    // 构造 RedisTaskStore 并建立连接参数（非常重要）。
    explicit RedisTaskStore(const std::string& host = "127.0.0.1", int port = 6379);
    
    // 析构并释放 Redis 连接。
    ~RedisTaskStore();
    
    // Disable copy, enable move
    RedisTaskStore(const RedisTaskStore&) = delete;
    RedisTaskStore& operator=(const RedisTaskStore&) = delete;
    // 移动构造。
    RedisTaskStore(RedisTaskStore&&) noexcept = default;
    // 移动赋值。
    RedisTaskStore& operator=(RedisTaskStore&&) noexcept = default;
    
    // ITaskStore interface implementation
    // 从 Redis 获取任务。
    std::optional<AgentTask> get_task(const std::string& task_id) override;
    // 将任务写入 Redis。
    void set_task(const AgentTask& task) override;
    // 判断任务是否存在。
    bool task_exists(const std::string& task_id) override;
    // 删除任务数据。
    bool delete_task(const std::string& task_id) override;
    // 更新任务状态。
    void update_status(const std::string& task_id,
                      TaskState status,
                      const std::string& message = "") override;
    // 添加任务产物。
    void add_artifact(const std::string& task_id,
                     const Artifact& artifact) override;
    // 添加历史消息。
    void add_history_message(const std::string& task_id,
                            const AgentMessage& message) override;
    // 获取历史消息列表。
    std::vector<AgentMessage> get_history(const std::string& context_id,
                                          int max_length = 0) override;

private:
    /**
     * @brief Get Redis key for task
     */
    // 生成任务 Redis Key。
    std::string task_key(const std::string& task_id) const {
        return "a2a:task:" + task_id;
    }
    
    /**
     * @brief Get Redis key for history
     */
    // 生成历史 Redis Key。
    std::string history_key(const std::string& context_id) const {
        return "a2a:history:" + context_id;
    }
    
    /**
     * @brief Execute Redis command and check for errors
     */
    // 执行 Redis 命令并返回结果（非常重要）。
    redisReply* execute_command(const char* format, ...);
    
    /**
     * @brief Reconnect to Redis if connection is lost
     */
    // 检查并重连 Redis。
    void ensure_connection();
    
    redisContext* context_;
    std::string host_;
    int port_;
    mutable std::mutex mutex_;
};

} // namespace a2a
