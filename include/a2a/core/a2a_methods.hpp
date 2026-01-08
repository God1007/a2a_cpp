// 概述: 定义 A2A JSON-RPC 方法名常量及校验工具，依赖 C++17 标准库字符串比较。
#pragma once

#include <string>

namespace a2a {

/**
 * @brief Constants for A2A JSON-RPC method names
 */
class A2AMethods {
public:
    // Message methods
    static constexpr const char* MESSAGE_SEND = "message/send";
    static constexpr const char* MESSAGE_STREAM = "message/stream";
    
    // Task methods
    static constexpr const char* TASK_GET = "tasks/get";
    static constexpr const char* TASK_CANCEL = "tasks/cancel";
    static constexpr const char* TASK_SUBSCRIBE = "tasks/resubscribe";
    
    // Push notification methods
    static constexpr const char* TASK_PUSH_NOTIFICATION_CONFIG_SET = 
        "tasks/pushNotificationConfig/set";
    static constexpr const char* TASK_PUSH_NOTIFICATION_CONFIG_GET = 
        "tasks/pushNotificationConfig/get";
    
    /**
     * @brief Check if a method requires streaming response
     */
    // 判断方法名是否为流式响应类型（非常重要）。
    static bool is_streaming_method(const std::string& method) {
        return method == MESSAGE_STREAM || method == TASK_SUBSCRIBE;
    }
    
    /**
     * @brief Check if a method name is valid
     */
    // 校验方法名是否属于协议支持的集合（非常重要）。
    static bool is_valid_method(const std::string& method) {
        return method == MESSAGE_SEND ||
               method == MESSAGE_STREAM ||
               method == TASK_GET ||
               method == TASK_CANCEL ||
               method == TASK_SUBSCRIBE ||
               method == TASK_PUSH_NOTIFICATION_CONFIG_SET ||
               method == TASK_PUSH_NOTIFICATION_CONFIG_GET;
    }
};

} // namespace a2a
