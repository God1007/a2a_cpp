// 概述: 定义 AgentCard 与能力/技能/提供方模型，支持 JSON 序列化与链式构建。
#pragma once

#include "../core/types.hpp"
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace a2a {

/**
 * @brief Agent Capabilities
 */
struct AgentCapabilities {
    bool streaming = false;
    bool push_notifications = false;
    bool task_management = true;
    
    // 序列化能力信息为 JSON。
    std::string to_json() const;
    // 从 JSON 反序列化能力信息。
    static AgentCapabilities from_json(const std::string& json);
};

/**
 * @brief Agent Skill - a unit of capability
 */
struct AgentSkill {
    std::string name;
    std::string description;
    std::vector<std::string> input_modes;
    std::vector<std::string> output_modes;
    
    // 序列化技能信息为 JSON。
    std::string to_json() const;
    // 从 JSON 反序列化技能信息。
    static AgentSkill from_json(const std::string& json);
};

/**
 * @brief Agent Provider information
 */
struct AgentProvider {
    std::string name;
    std::string organization;
    std::optional<std::string> url;
    
    // 序列化提供方信息为 JSON。
    std::string to_json() const;
    // 从 JSON 反序列化提供方信息。
    static AgentProvider from_json(const std::string& json);
};

/**
 * @brief Agent Card - conveys key information about an agent
 */
class AgentCard {
public:
    // 默认构造 AgentCard。
    AgentCard() = default;
    
    // 获取 Agent 名称。
    const std::string& name() const { return name_; }
    // 获取 Agent 描述。
    const std::string& description() const { return description_; }
    // 获取 Agent URL。
    const std::string& url() const { return url_; }
    // 获取版本号。
    const std::string& version() const { return version_; }
    // 获取协议版本。
    const std::string& protocol_version() const { return protocol_version_; }
    // 获取图标 URL（可选）。
    const std::optional<std::string>& icon_url() const { return icon_url_; }
    // 获取文档 URL（可选）。
    const std::optional<std::string>& documentation_url() const { return documentation_url_; }
    // 获取能力配置。
    const AgentCapabilities& capabilities() const { return capabilities_; }
    // 获取默认输入模式。
    const std::vector<std::string>& default_input_modes() const { return default_input_modes_; }
    // 获取默认输出模式。
    const std::vector<std::string>& default_output_modes() const { return default_output_modes_; }
    // 获取技能列表。
    const std::vector<AgentSkill>& skills() const { return skills_; }
    // 获取偏好传输方式。
    AgentTransport preferred_transport() const { return preferred_transport_; }
    // 获取提供方信息（可选）。
    const std::optional<AgentProvider>& provider() const { return provider_; }
    
    // 设置 Agent 名称。
    void set_name(const std::string& name) { name_ = name; }
    // 设置 Agent 描述。
    void set_description(const std::string& desc) { description_ = desc; }
    // 设置 Agent URL。
    void set_url(const std::string& url) { url_ = url; }
    // 设置版本号。
    void set_version(const std::string& version) { version_ = version; }
    // 设置协议版本。
    void set_protocol_version(const std::string& version) { protocol_version_ = version; }
    // 设置图标 URL。
    void set_icon_url(const std::string& url) { icon_url_ = url; }
    // 设置文档 URL。
    void set_documentation_url(const std::string& url) { documentation_url_ = url; }
    // 设置能力配置。
    void set_capabilities(const AgentCapabilities& caps) { capabilities_ = caps; }
    // 设置偏好传输方式。
    void set_preferred_transport(AgentTransport transport) { preferred_transport_ = transport; }
    // 设置提供方信息。
    void set_provider(const AgentProvider& provider) { provider_ = provider; }
    
    // 添加默认输入模式。
    void add_input_mode(const std::string& mode) {
        default_input_modes_.push_back(mode);
    }
    
    // 添加默认输出模式。
    void add_output_mode(const std::string& mode) {
        default_output_modes_.push_back(mode);
    }
    
    // 添加技能条目。
    void add_skill(const AgentSkill& skill) {
        skills_.push_back(skill);
    }
    
    /**
     * @brief Serialize to JSON
     */
    // 序列化 AgentCard 为 JSON（非常重要）。
    std::string to_json() const;
    
    /**
     * @brief Deserialize from JSON
     */
    // 从 JSON 反序列化 AgentCard（非常重要）。
    static AgentCard from_json(const std::string& json);
    
    /**
     * @brief Create a new AgentCard
     */
    // 创建空 AgentCard。
    static AgentCard create() {
        return AgentCard();
    }
    
    /**
     * @brief Fluent API methods
     */
    // 链式设置名称。
    AgentCard& with_name(const std::string& name) {
        name_ = name;
        return *this;
    }
    
    // 链式设置描述。
    AgentCard& with_description(const std::string& desc) {
        description_ = desc;
        return *this;
    }
    
    // 链式设置 URL。
    AgentCard& with_url(const std::string& url) {
        url_ = url;
        return *this;
    }
    
    // 链式设置版本号。
    AgentCard& with_version(const std::string& version) {
        version_ = version;
        return *this;
    }
    
    // 链式设置能力配置。
    AgentCard& with_capabilities(const AgentCapabilities& caps) {
        capabilities_ = caps;
        return *this;
    }
    
    // 链式追加输入模式。
    AgentCard& with_input_mode(const std::string& mode) {
        default_input_modes_.push_back(mode);
        return *this;
    }
    
    // 链式追加输出模式。
    AgentCard& with_output_mode(const std::string& mode) {
        default_output_modes_.push_back(mode);
        return *this;
    }
    
    // 链式追加技能。
    AgentCard& with_skill(const AgentSkill& skill) {
        skills_.push_back(skill);
        return *this;
    }

private:
    std::string name_;
    std::string description_;
    std::string url_;
    std::string version_;
    std::string protocol_version_ = "0.3.0";
    std::optional<std::string> icon_url_;
    std::optional<std::string> documentation_url_;
    AgentCapabilities capabilities_;
    std::vector<std::string> default_input_modes_ = {"text"};
    std::vector<std::string> default_output_modes_ = {"text"};
    std::vector<AgentSkill> skills_;
    AgentTransport preferred_transport_ = AgentTransport::JsonRpc;
    std::optional<AgentProvider> provider_;
};

} // namespace a2a
