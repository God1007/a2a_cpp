// 概述: 定义 Agent 产物模型及 JSON 序列化接口，使用 C++17 optional 与 map 记录元数据。
#pragma once

#include <string>
#include <optional>
#include <map>

namespace a2a {

/**
 * @brief Artifact - represents output produced by an agent
 */
class Artifact {
public:
    // 默认构造产物对象。
    Artifact() = default;
    
    // 构造带 ID 与名称的产物。
    Artifact(const std::string& id, const std::string& name)
        : id_(id), name_(name) {}
    
    // 获取产物 ID。
    const std::string& id() const { return id_; }
    // 获取产物名称。
    const std::string& name() const { return name_; }
    // 获取产物描述（可选）。
    const std::optional<std::string>& description() const { return description_; }
    // 获取 MIME 类型（可选）。
    const std::optional<std::string>& mime_type() const { return mime_type_; }
    // 获取资源 URL（可选）。
    const std::optional<std::string>& url() const { return url_; }
    // 获取内容正文（可选）。
    const std::optional<std::string>& content() const { return content_; }
    // 获取元数据映射。
    const std::map<std::string, std::string>& metadata() const { return metadata_; }
    
    // 设置产物 ID。
    void set_id(const std::string& id) { id_ = id; }
    // 设置产物名称。
    void set_name(const std::string& name) { name_ = name; }
    // 设置产物描述。
    void set_description(const std::string& desc) { description_ = desc; }
    // 设置 MIME 类型。
    void set_mime_type(const std::string& type) { mime_type_ = type; }
    // 设置 URL。
    void set_url(const std::string& url) { url_ = url; }
    // 设置内容正文。
    void set_content(const std::string& content) { content_ = content; }
    // 添加/更新元数据键值。
    void add_metadata(const std::string& key, const std::string& value) {
        metadata_[key] = value;
    }
    
    /**
     * @brief Serialize to JSON
     */
    // 序列化为 JSON 字符串。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON
     */
    // 从 JSON 字符串反序列化产物。
    static Artifact from_json(const std::string& json);
    
    /**
     * @brief Create a new Artifact
     */
    // 创建空产物对象。
    static Artifact create() {
        return Artifact();
    }
    
    /**
     * @brief Fluent API methods
     */
    // 链式设置 ID。
    Artifact& with_id(const std::string& id) {
        id_ = id;
        return *this;
    }
    
    // 链式设置名称。
    Artifact& with_name(const std::string& name) {
        name_ = name;
        return *this;
    }
    
    // 链式设置描述。
    Artifact& with_description(const std::string& desc) {
        description_ = desc;
        return *this;
    }
    
    // 链式设置 MIME 类型。
    Artifact& with_mime_type(const std::string& type) {
        mime_type_ = type;
        return *this;
    }
    
    // 链式设置 URL。
    Artifact& with_url(const std::string& url) {
        url_ = url;
        return *this;
    }
    
    // 链式设置内容正文。
    Artifact& with_content(const std::string& content) {
        content_ = content;
        return *this;
    }
    
    // 链式添加元数据。
    Artifact& with_metadata(const std::string& key, const std::string& value) {
        metadata_[key] = value;
        return *this;
    }

private:
    std::string id_;
    std::string name_;
    std::optional<std::string> description_;
    std::optional<std::string> mime_type_;
    std::optional<std::string> url_;
    std::optional<std::string> content_;
    std::map<std::string, std::string> metadata_;
};

} // namespace a2a
