// 概述: 定义 JSON-RPC 2.0 请求模型与序列化接口，使用 C++17 标准库字符串/可选值能力。
#pragma once

#include <string>
#include <optional>
#include <memory>

namespace a2a {

/**
 * @brief JSON-RPC 2.0 Request
 */
class JsonRpcRequest {
public:
    // 默认构造，便于后续逐字段填充请求对象。
    JsonRpcRequest() = default;
    
    // 构造包含请求 ID、方法名与参数 JSON 的请求对象。
    JsonRpcRequest(const std::string& id, 
                   const std::string& method,
                   const std::string& params_json = "{}")
        : jsonrpc_("2.0")
        , id_(id)
        , method_(method)
        , params_json_(params_json) {}
    
    // 获取 JSON-RPC 版本号字符串。
    const std::string& jsonrpc() const { return jsonrpc_; }
    // 获取请求 ID。
    const std::string& id() const { return id_; }
    // 获取方法名。
    const std::string& method() const { return method_; }
    // 获取参数 JSON 字符串。
    const std::string& params_json() const { return params_json_; }
    
    // 设置请求 ID。
    void set_id(const std::string& id) { id_ = id; }
    // 设置方法名。
    void set_method(const std::string& method) { method_ = method; }
    // 设置参数 JSON 字符串。
    void set_params_json(const std::string& params) { params_json_ = params; }
    
    /**
     * @brief Serialize to JSON string
     */
    // 序列化请求为 JSON 字符串（非常重要）。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON string
     */
    // 从 JSON 字符串反序列化请求（非常重要）。
    static JsonRpcRequest from_json(const std::string& json);

private:
    std::string jsonrpc_ = "2.0";
    std::string id_;
    std::string method_;
    std::string params_json_ = "{}";
};

} // namespace a2a
