// 概述: 定义消息分片基类及文本/文件/结构化数据实现，提供 JSON 序列化与克隆接口。
#pragma once

#include "../core/types.hpp"
#include <string>
#include <memory>
#include <vector>

namespace a2a {

/**
 * @brief Base class for message parts (polymorphic)
 */
class Part {
public:
    // 虚析构，保证多态释放。
    virtual ~Part() = default;
    
    // 获取分片类型。
    virtual PartKind kind() const = 0;
    // 序列化分片为 JSON。
    virtual std::string to_json() const = 0;
    // 克隆当前分片对象。
    virtual std::unique_ptr<Part> clone() const = 0;
    
    // 从 JSON 反序列化为对应分片类型（非常重要）。
    static std::unique_ptr<Part> from_json(const std::string& json);
};

/**
 * @brief Text message part
 */
class TextPart : public Part {
public:
    // 默认构造文本分片。
    TextPart() = default;
    // 构造携带文本内容的分片。
    explicit TextPart(const std::string& text) : text_(text) {}
    
    // 返回文本分片类型。
    PartKind kind() const override { return PartKind::Text; }
    
    // 获取文本内容。
    const std::string& text() const { return text_; }
    // 设置文本内容。
    void set_text(const std::string& text) { text_ = text; }
    
    // 序列化文本分片为 JSON。
    std::string to_json() const override;
    // 克隆文本分片。
    std::unique_ptr<Part> clone() const override {
        return std::make_unique<TextPart>(text_);
    }

private:
    std::string text_;
};

/**
 * @brief File message part
 */
class FilePart : public Part {
public:
    // 默认构造文件分片。
    FilePart() = default;
    // 构造携带文件名、MIME 和数据内容的分片。
    FilePart(const std::string& filename, const std::string& mime_type, 
             const std::vector<uint8_t>& data)
        : filename_(filename)
        , mime_type_(mime_type)
        , data_(data) {}
    
    // 返回文件分片类型。
    PartKind kind() const override { return PartKind::File; }
    
    // 获取文件名。
    const std::string& filename() const { return filename_; }
    // 获取 MIME 类型。
    const std::string& mime_type() const { return mime_type_; }
    // 获取二进制数据。
    const std::vector<uint8_t>& data() const { return data_; }
    
    // 设置文件名。
    void set_filename(const std::string& name) { filename_ = name; }
    // 设置 MIME 类型。
    void set_mime_type(const std::string& type) { mime_type_ = type; }
    // 设置文件二进制数据。
    void set_data(const std::vector<uint8_t>& data) { data_ = data; }
    
    // 序列化文件分片为 JSON。
    std::string to_json() const override;
    // 克隆文件分片。
    std::unique_ptr<Part> clone() const override {
        return std::make_unique<FilePart>(filename_, mime_type_, data_);
    }

private:
    std::string filename_;
    std::string mime_type_;
    std::vector<uint8_t> data_;
};

/**
 * @brief Data message part (structured data)
 */
class DataPart : public Part {
public:
    // 默认构造结构化数据分片。
    DataPart() = default;
    // 构造携带 JSON 数据的分片。
    explicit DataPart(const std::string& data_json) : data_json_(data_json) {}
    
    // 返回结构化数据分片类型。
    PartKind kind() const override { return PartKind::Data; }
    
    // 获取数据 JSON。
    const std::string& data_json() const { return data_json_; }
    // 设置数据 JSON。
    void set_data_json(const std::string& json) { data_json_ = json; }
    
    // 序列化数据分片为 JSON。
    std::string to_json() const override;
    // 克隆数据分片。
    std::unique_ptr<Part> clone() const override {
        return std::make_unique<DataPart>(data_json_);
    }

private:
    std::string data_json_;
};

} // namespace a2a
