// 概述: 实现 JSON-RPC 请求序列化与反序列化，使用 nlohmann::json 解析与构建 JSON。
#include <a2a/core/jsonrpc_request.hpp>
#include <a2a/core/exception.hpp>
#include <json.hpp>

using json = nlohmann::json;

namespace a2a {

std::string JsonRpcRequest::to_json() const {
    // 将请求对象序列化为 JSON 字符串（非常重要）。
    try {
        json j;
        j["jsonrpc"] = jsonrpc_;
        j["id"] = id_;
        j["method"] = method_;
        
        if (!params_json_.empty() && params_json_ != "{}") {
            // Parse params_json_ as JSON object and add it
            j["params"] = json::parse(params_json_);
        }
        
        return j.dump();
    } catch (const json::exception& e) {
        throw A2AException(
            std::string("JSON serialization error: ") + e.what(),
            ErrorCode::InternalError
        );
    }
}

JsonRpcRequest JsonRpcRequest::from_json(const std::string& json_str) {
    // 解析 JSON 字符串为请求对象，处理字段缺省与 ID 类型（非常重要）。
    try {
        json j = json::parse(json_str);
        
        JsonRpcRequest request;
        
        // Extract required fields
        if (j.contains("jsonrpc")) {
            request.jsonrpc_ = j["jsonrpc"].get<std::string>();
        }
        
        if (j.contains("id")) {
            // Handle both string and numeric IDs
            if (j["id"].is_string()) {
                request.id_ = j["id"].get<std::string>();
            } else if (j["id"].is_number()) {
                request.id_ = std::to_string(j["id"].get<int>());
            }
        }
        
        if (j.contains("method")) {
            request.method_ = j["method"].get<std::string>();
        }
        
        // Extract params as JSON string
        if (j.contains("params")) {
            request.params_json_ = j["params"].dump();
        }
        
        return request;
    } catch (const json::exception& e) {
        throw A2AException(
            std::string("JSON parsing error: ") + e.what(),
            ErrorCode::ParseError
        );
    }
}

} // namespace a2a
