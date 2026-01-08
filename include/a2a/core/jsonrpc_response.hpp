// 概述: 定义 JSON-RPC 2.0 响应与错误结构体，基于 C++17 optional 表达成功/失败分支。
#pragma once

#include "error_code.hpp"
#include <string>
#include <optional>

namespace a2a {

/**
 * @brief JSON-RPC 2.0 Error object
 */
struct JsonRpcError {
    int32_t code;
    std::string message;
    std::string data;  // Optional additional data
    
    // 默认构造，初始化错误码为 0。
    JsonRpcError() : code(0) {}
    // 使用数值错误码与错误信息构造。
    JsonRpcError(int32_t c, const std::string& msg) 
        : code(c), message(msg) {}
    // 使用 ErrorCode 枚举构造错误对象。
    JsonRpcError(ErrorCode ec, const std::string& msg)
        : code(static_cast<int32_t>(ec)), message(msg) {}
};

/**
 * @brief JSON-RPC 2.0 Response
 */
class JsonRpcResponse {
public:
    // 默认构造响应对象。
    JsonRpcResponse() = default;
    
    // Success response
    // 构造成功响应，携带结果 JSON 字符串。
    JsonRpcResponse(const std::string& id, const std::string& result_json)
        : jsonrpc_("2.0")
        , id_(id)
        , result_json_(result_json)
        , error_() {}
    
    // Error response
    // 构造错误响应，携带错误对象。
    JsonRpcResponse(const std::string& id, const JsonRpcError& error)
        : jsonrpc_("2.0")
        , id_(id)
        , result_json_()
        , error_(error) {}
    
    // 获取 JSON-RPC 版本号。
    const std::string& jsonrpc() const { return jsonrpc_; }
    // 获取响应 ID。
    const std::string& id() const { return id_; }
    // 获取结果 JSON（成功时有值）。
    const std::optional<std::string>& result_json() const { return result_json_; }
    // 获取错误对象（失败时有值）。
    const std::optional<JsonRpcError>& error() const { return error_; }
    
    // 判断是否为错误响应。
    bool is_error() const { return error_.has_value(); }
    // 判断是否为成功响应。
    bool is_success() const { return result_json_.has_value(); }
    
    /**
     * @brief Serialize to JSON string
     */
    // 序列化响应为 JSON 字符串（非常重要）。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON string
     */
    // 从 JSON 字符串反序列化响应（非常重要）。
    static JsonRpcResponse from_json(const std::string& json);
    
    /**
     * @brief Create error response
     */
    // 构造错误响应对象。
    static JsonRpcResponse create_error(const std::string& id, 
                                       ErrorCode code, 
                                       const std::string& message);
    
    /**
     * @brief Create success response
     */
    // 构造成功响应对象。
    static JsonRpcResponse create_success(const std::string& id, 
                                         const std::string& result_json);

private:
    std::string jsonrpc_ = "2.0";
    std::string id_;
    std::optional<std::string> result_json_;
    std::optional<JsonRpcError> error_;
};

} // namespace a2a
