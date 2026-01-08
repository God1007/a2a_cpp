// 概述: 实现 JSON-RPC 响应的序列化与反序列化，使用 nlohmann::json 处理 JSON 结构。
#include <a2a/core/jsonrpc_response.hpp>
#include <a2a/core/exception.hpp>
#include <json.hpp>

using json = nlohmann::json;

namespace a2a {

std::string JsonRpcResponse::to_json() const {
    // 将响应对象序列化为 JSON 字符串（非常重要）。
    try {
        json j;
        j["jsonrpc"] = jsonrpc_;
        j["id"] = id_;
        
        if (result_json_.has_value()) {
            // Parse result_json_ as JSON and add it
            j["result"] = json::parse(*result_json_);
        } else if (error_.has_value()) {
            json error_obj;
            error_obj["code"] = error_->code;
            error_obj["message"] = error_->message;
            
            if (!error_->data.empty()) {
                error_obj["data"] = error_->data;
            }
            
            j["error"] = error_obj;
        }
        
        return j.dump();
    } catch (const json::exception& e) {
        throw A2AException(
            std::string("JSON serialization error: ") + e.what(),
            ErrorCode::InternalError
        );
    }
}

JsonRpcResponse JsonRpcResponse::from_json(const std::string& json_str) {
    // 将 JSON 字符串解析为响应对象，处理 result/error 分支（非常重要）。
    try {
        json j = json::parse(json_str);
        
        JsonRpcResponse response;
        
        // Extract required fields
        if (j.contains("jsonrpc")) {
            response.jsonrpc_ = j["jsonrpc"].get<std::string>();
        }
        
        if (j.contains("id")) {
            // Handle both string and numeric IDs
            if (j["id"].is_string()) {
                response.id_ = j["id"].get<std::string>();
            } else if (j["id"].is_number()) {
                response.id_ = std::to_string(j["id"].get<int>());
            } else if (j["id"].is_null()) {
                response.id_ = "";
            }
        }
        
        // Check for result
        if (j.contains("result")) {
            response.result_json_ = j["result"].dump();
        }
        
        // Check for error
        if (j.contains("error") && j["error"].is_object()) {
            JsonRpcError error;
            
            if (j["error"].contains("code")) {
                error.code = j["error"]["code"].get<int>();
            }
            
            if (j["error"].contains("message")) {
                error.message = j["error"]["message"].get<std::string>();
            }
            
            if (j["error"].contains("data")) {
                if (j["error"]["data"].is_string()) {
                    error.data = j["error"]["data"].get<std::string>();
                } else {
                    error.data = j["error"]["data"].dump();
                }
            }
            
            response.error_ = error;
        }
        
        return response;
    } catch (const json::exception& e) {
        throw A2AException(
            std::string("JSON parsing error: ") + e.what(),
            ErrorCode::ParseError
        );
    }
}

JsonRpcResponse JsonRpcResponse::create_error(const std::string& id,
                                             ErrorCode code,
                                             const std::string& message) {
    // 构造错误响应的便捷工厂。
    return JsonRpcResponse(id, JsonRpcError(code, message));
}

JsonRpcResponse JsonRpcResponse::create_success(const std::string& id,
                                               const std::string& result_json) {
    // 构造成功响应的便捷工厂。
    return JsonRpcResponse(id, result_json);
}

} // namespace a2a
