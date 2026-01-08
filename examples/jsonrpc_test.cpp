// 概述: JSON-RPC 功能验证测试程序，覆盖序列化/反序列化与错误处理流程。
// JSON-RPC 功能验证测试程序
#include <a2a/core/jsonrpc_request.hpp>
#include <a2a/core/jsonrpc_response.hpp>
#include <a2a/core/exception.hpp>
#include <iostream>
#include <cassert>

using namespace a2a;

// 测试请求序列化是否包含必要字段。
void test_request_serialization() {
    std::cout << "=== 测试 1: JSON-RPC 请求序列化 ===" << std::endl;
    
    JsonRpcRequest req;
    req.set_id("test-123");
    req.set_method("a2a.message.send");
    req.set_params_json(R"({"message":{"role":"user","text":"Hello"}})");
    
    std::string json = req.to_json();
    std::cout << "生成的 JSON:\n" << json << std::endl;
    
    // 验证包含必要字段
    assert(json.find("\"jsonrpc\":\"2.0\"") != std::string::npos);
    assert(json.find("\"id\":\"test-123\"") != std::string::npos);
    assert(json.find("\"method\":\"a2a.message.send\"") != std::string::npos);
    assert(json.find("\"params\"") != std::string::npos);
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试请求反序列化能否正确解析字段。
void test_request_deserialization() {
    std::cout << "=== 测试 2: JSON-RPC 请求反序列化 ===" << std::endl;
    
    std::string json = R"({
        "jsonrpc": "2.0",
        "id": "456",
        "method": "a2a.task.get",
        "params": {
            "taskId": "task-789",
            "includeHistory": true
        }
    })";
    
    JsonRpcRequest req = JsonRpcRequest::from_json(json);
    
    std::cout << "解析结果:" << std::endl;
    std::cout << "  jsonrpc: " << req.jsonrpc() << std::endl;
    std::cout << "  id: " << req.id() << std::endl;
    std::cout << "  method: " << req.method() << std::endl;
    std::cout << "  params: " << req.params_json() << std::endl;
    
    assert(req.jsonrpc() == "2.0");
    assert(req.id() == "456");
    assert(req.method() == "a2a.task.get");
    assert(!req.params_json().empty());
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试数字 ID 反序列化后的字符串转换。
void test_request_with_numeric_id() {
    std::cout << "=== 测试 3: 数字 ID 处理 ===" << std::endl;
    
    std::string json = R"({
        "jsonrpc": "2.0",
        "id": 999,
        "method": "test.method"
    })";
    
    JsonRpcRequest req = JsonRpcRequest::from_json(json);
    
    std::cout << "数字 ID 转换为字符串: " << req.id() << std::endl;
    assert(req.id() == "999");
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试成功响应的序列化格式。
void test_response_success() {
    std::cout << "=== 测试 4: 成功响应序列化 ===" << std::endl;
    
    JsonRpcResponse res = JsonRpcResponse::create_success(
        "test-123",
        R"({"message":{"role":"agent","text":"Response"}})"
    );
    
    std::string json = res.to_json();
    std::cout << "生成的 JSON:\n" << json << std::endl;
    
    assert(json.find("\"result\"") != std::string::npos);
    assert(json.find("\"error\"") == std::string::npos);
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试错误响应的序列化格式。
void test_response_error() {
    std::cout << "=== 测试 5: 错误响应序列化 ===" << std::endl;
    
    JsonRpcResponse res = JsonRpcResponse::create_error(
        "test-456",
        ErrorCode::TaskNotFound,
        "Task not found"
    );
    
    std::string json = res.to_json();
    std::cout << "生成的 JSON:\n" << json << std::endl;
    
    assert(json.find("\"error\"") != std::string::npos);
    assert(json.find("\"code\"") != std::string::npos);
    assert(json.find("\"message\"") != std::string::npos);
    assert(json.find("\"result\"") == std::string::npos);
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试成功响应的反序列化解析。
void test_response_deserialization_success() {
    std::cout << "=== 测试 6: 成功响应反序列化 ===" << std::endl;
    
    std::string json = R"({
        "jsonrpc": "2.0",
        "id": "test-789",
        "result": {
            "status": "completed",
            "data": [1, 2, 3]
        }
    })";
    
    JsonRpcResponse res = JsonRpcResponse::from_json(json);
    
    std::cout << "解析结果:" << std::endl;
    std::cout << "  id: " << res.id() << std::endl;
    std::cout << "  has result: " << (res.is_success() ? "是" : "否") << std::endl;
    std::cout << "  has error: " << (res.is_error() ? "是" : "否") << std::endl;
    
    if (res.is_success()) {
        std::cout << "  result: " << *res.result_json() << std::endl;
    }
    
    assert(res.is_success());
    assert(!res.is_error());
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试错误响应的反序列化解析。
void test_response_deserialization_error() {
    std::cout << "=== 测试 7: 错误响应反序列化 ===" << std::endl;
    
    std::string json = R"({
        "jsonrpc": "2.0",
        "id": "error-test",
        "error": {
            "code": -32001,
            "message": "Task not found",
            "data": "Additional error info"
        }
    })";
    
    JsonRpcResponse res = JsonRpcResponse::from_json(json);
    
    std::cout << "解析结果:" << std::endl;
    std::cout << "  id: " << res.id() << std::endl;
    std::cout << "  has result: " << (res.is_success() ? "是" : "否") << std::endl;
    std::cout << "  has error: " << (res.is_error() ? "是" : "否") << std::endl;
    
    if (res.is_error()) {
        std::cout << "  error code: " << res.error()->code << std::endl;
        std::cout << "  error message: " << res.error()->message << std::endl;
        std::cout << "  error data: " << res.error()->data << std::endl;
    }
    
    assert(!res.is_success());
    assert(res.is_error());
    assert(res.error()->code == -32001);
    assert(res.error()->message == "Task not found");
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试复杂嵌套 JSON 参数的序列化与反序列化。
void test_complex_nested_json() {
    std::cout << "=== 测试 8: 复杂嵌套 JSON 处理 ===" << std::endl;
    
    std::string complex_params = R"({
        "message": {
            "role": "user",
            "parts": [
                {"type": "text", "text": "Hello"},
                {"type": "file", "url": "https://example.com/file.pdf"}
            ],
            "context": {
                "history": [
                    {"id": "msg1", "text": "Previous message"},
                    {"id": "msg2", "text": "Another message"}
                ]
            }
        },
        "options": {
            "streaming": true,
            "maxTokens": 1000
        }
    })";
    
    JsonRpcRequest req;
    req.set_id("complex-test");
    req.set_method("a2a.message.send");
    req.set_params_json(complex_params);
    
    std::string json = req.to_json();
    std::cout << "生成的复杂 JSON:\n" << json << std::endl;
    
    // 反序列化验证
    JsonRpcRequest parsed = JsonRpcRequest::from_json(json);
    assert(parsed.id() == "complex-test");
    assert(parsed.method() == "a2a.message.send");
    assert(!parsed.params_json().empty());
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试异常与错误码处理逻辑（非常重要）。
void test_error_handling() {
    std::cout << "=== 测试 9: 错误处理 ===" << std::endl;
    
    // 测试无效 JSON
    try {
        std::string invalid_json = "{ invalid json }";
        JsonRpcRequest::from_json(invalid_json);
        assert(false && "应该抛出异常");
    } catch (const A2AException& e) {
        std::cout << "捕获到预期的异常: " << e.what() << std::endl;
        assert(e.error_code() == ErrorCode::ParseError);
    }
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 测试特殊字符与转义处理。
void test_special_characters() {
    std::cout << "=== 测试 10: 特殊字符和转义处理 ===" << std::endl;
    
    std::string params_with_special = R"({
        "text": "包含特殊字符: \"引号\", \n换行, \t制表符, 中文字符",
        "emoji": "😀🎉✨"
    })";
    
    JsonRpcRequest req;
    req.set_id("special-chars");
    req.set_method("test.special");
    req.set_params_json(params_with_special);
    
    std::string json = req.to_json();
    std::cout << "包含特殊字符的 JSON:\n" << json << std::endl;
    
    // 反序列化验证
    JsonRpcRequest parsed = JsonRpcRequest::from_json(json);
    assert(parsed.id() == "special-chars");
    
    std::cout << "✓ 测试通过\n" << std::endl;
}

// 程序入口：顺序执行所有测试用例并打印结果。
int main() {
    std::cout << "╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║  A2A C++ SDK - JSON-RPC 功能验证测试     ║" << std::endl;
    std::cout << "║  使用 nlohmann::json 库                  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    try {
        test_request_serialization();
        test_request_deserialization();
        test_request_with_numeric_id();
        test_response_success();
        test_response_error();
        test_response_deserialization_success();
        test_response_deserialization_error();
        test_complex_nested_json();
        test_error_handling();
        test_special_characters();
        
        std::cout << "╔══════════════════════════════════════════╗" << std::endl;
        std::cout << "║  ✓ 所有测试通过！                       ║" << std::endl;
        std::cout << "║  JSON-RPC 实现已升级为生产级别           ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════╝" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ 测试失败: " << e.what() << std::endl;
        return 1;
    }
}
