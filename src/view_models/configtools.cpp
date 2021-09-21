#include "configtools.h"

using namespace across::setting;
using namespace across::core;
using namespace across::config;
using namespace across::utils;

void
Interface::Language::fromNodeView(toml::v2::node_view<toml::node> language,const toml::v2::node_view<toml::node>& default_config)
{
  if (auto temp = *language.value<std::string>(); !temp.empty())
    this->language = QString::fromStdString(temp);
}

void
Interface::Language::toNodeView(const toml::v2::node_view<toml::node>& language,const toml::v2::node_view<toml::node>& default_config)
{
  *language.as_string() = this->language.toStdString();
}

void
Interface::Theme::fromNodeView(toml::v2::node_view<toml::node> theme,const toml::v2::node_view<toml::node>& default_config)
{
  if(theme["theme"].as_string()==nullptr)
    theme.as_table()->insert("theme",default_config["theme"]);
  if (auto temp = theme["theme"].value<std::string>(); temp.has_value())
    this->theme = QString::fromStdString(temp.value());

  if(theme["include_dir"].as_string()==nullptr)
    theme.as_table()->insert("include_dir",default_config["include_dir"]);
  if (auto temp = theme["include_dir"].value<std::string>(); temp.has_value())
    this->include_dir = QString::fromStdString(temp.value());
}

void
Interface::Theme::toNodeView(const toml::v2::node_view<toml::node>& theme,const toml::v2::node_view<toml::node>& default_config)
{
  auto table = *theme.as_table();
  if(theme["theme"].as_string()!=nullptr)
    *theme["theme"].as_string() = this->theme.toStdString();
  else{
    table.insert("theme",default_config["theme"]);
  }
  if(theme["include_dir"].as_string()!=nullptr)
    *theme["include_dir"].as_string() = this->include_dir.toStdString();
  else{
    table.insert("include_dir",default_config["include_dir"]);
  }
}

void
Interface::Tray::fromNodeView(toml::v2::node_view<toml::node> tray,const toml::v2::node_view<toml::node>& default_config)
{
  if(tray["enable"].as_boolean()==nullptr)
    tray.as_table()->insert("enable",default_config["enable"]);
  if (auto temp = tray["enable"].value<bool>(); temp.has_value())
    this->enable = temp.value();
}

void
Interface::Tray::toNodeView(const toml::v2::node_view<toml::node>& tray,const toml::v2::node_view<toml::node>& default_config)
{
  if(tray["enable"].as_boolean()==nullptr)
    tray.as_table()->insert("enable",default_config["enable"]);
  *tray["enable"].as_boolean() = this->enable;
}

void
Interface::fromNodeView(toml::v2::node_view<toml::node> interface,const toml::v2::node_view<toml::node>& default_config)
{
  auto &table = *interface.as_table();
  if(interface["language"].as_string()!=nullptr)
    this->language.fromNodeView(interface["language"],default_config["language"]);
  else{
    table.insert("language",default_config["language"]);
  }

  if(interface["theme"].as_table()!=nullptr)
    this->theme.fromNodeView(interface["theme"],default_config["theme"]);
  else
    table.insert("theme",default_config["theme"]);

  if(interface["tray"].as_table()!=nullptr)
    this->tray.fromNodeView(interface["tray"],default_config["tray"]);
  else
    table.insert("tray",default_config["tray"]);
}

void
Interface::toNodeView(const toml::v2::node_view<toml::node>& interface,const toml::v2::node_view<toml::node>& default_config)
{
  auto &table = *interface.as_table();

  if(interface["language"].as_string()==nullptr)
    interface.as_table()->insert("language",default_config["language"]);
  this->language.toNodeView(interface["language"],default_config["language"]);

  if(interface["theme"].as_string()==nullptr)
    interface.as_table()->insert("theme",default_config["theme"]);
  this->theme.toNodeView(interface["theme"],default_config["theme"]);

  if(interface["tray"].as_string()==nullptr)
    interface.as_table()->insert("tray",default_config["tray"]);
  this->tray.toNodeView(interface["tray"],default_config["tray"]);
}

void
Network::fromNodeView(toml::v2::node_view<toml::node> network,const toml::v2::node_view<toml::node>& default_config)
{
  if(network["test_method"].as_string()==nullptr)
    network.as_table()->insert("test_method",default_config["test_method"]);
  if (auto temp = network["test_method"].value<std::string>(); temp.has_value())
    this->test_method = QString::fromStdString(temp.value());

  if(network["test_url"].as_string()==nullptr)
    network.as_table()->insert("test_url",default_config["test_url"]);
  if (auto temp = network["test_url"].value<std::string>(); temp.has_value())
    this->test_url = QString::fromStdString(temp.value());

  if(network["user_agent"].as_string()==nullptr)
    network.as_table()->insert("user_agent",default_config["user_agent"]);
  if (auto temp = network["user_agent"].value<std::string>(); temp.has_value())
    this->user_agent = QString::fromStdString(temp.value());
}

void
Network::toNodeView(const toml::v2::node_view<toml::node>& network,const toml::v2::node_view<toml::node>& default_config)
{
  if(network["test_method"].as_string()==nullptr)
    network.as_table()->insert("test_method",default_config["test_method"]);
  *network["test_method"].as_string() = this->test_method.toStdString();

  if(network["test_url"].as_string()==nullptr)
    network.as_table()->insert("test_url",default_config["test_url"]);
  *network["test_url"].as_string() = this->test_url.toStdString();

  if(network["user_agent"].as_string()==nullptr)
    network.as_table()->insert("user_agent",default_config["user_agent"]);
  *network["user_agent"].as_string() = this->user_agent.toStdString();
}

void
Update::fromNodeView(toml::v2::node_view<toml::node> update,const toml::v2::node_view<toml::node>& default_config)
{
  if(update["auto_update"].as_boolean()==nullptr)
    update.as_table()->insert("auto_update",default_config["auto_update"]);
  if (auto temp = update["auto_update"].value<bool>(); temp.has_value())
    this->auto_update = temp.value();

  if(update["check_update"].as_boolean()==nullptr)
    update.as_table()->insert("check_update",default_config["check_update"]);
  if (auto temp = update["check_update"].value<bool>(); temp.has_value())
    this->check_update = temp.value();

  if(update["update_from_proxy"].as_boolean()==nullptr)
    update.as_table()->insert("update_from_proxy",default_config["update_from_proxy"]);
  if (auto temp = update["update_from_proxy"].value<bool>(); temp.has_value())
    this->update_from_proxy = temp.value();
  
  if(update["update_channel"].as_string()==nullptr)
    update.as_table()->insert("update_channel",default_config["update_channel"]);
  if (auto temp = update["update_channel"].value<std::string>();
      temp.has_value())
    this->update_channel = QString::fromStdString(temp.value());
}

void
Update::toNodeView(const toml::v2::node_view<toml::node>& update,const toml::v2::node_view<toml::node>& default_config)
{
  if(update["auto_update"].as_boolean()==nullptr)
    update.as_table()->insert("auto_update",default_config["auto_update"]);
  *update["auto_update"].as_boolean() = this->auto_update;

  if(update["check_update"].as_boolean()==nullptr)
    update.as_table()->insert("check_update",default_config["check_update"]);
  *update["check_update"].as_boolean() = this->check_update;

  if(update["update_channel"].as_string()==nullptr)
    update.as_table()->insert("update_channel",default_config["update_channel"]);
  *update["update_channel"].as_string() = this->update_channel.toStdString();

  if(update["update_from_proxy"].as_boolean()==nullptr)
    update.as_table()->insert("update_from_proxy",default_config["update_from_proxy"]);
  *update["update_from_proxy"].as_boolean() = this->update_from_proxy;
}

void
DataBase::fromNodeView(toml::v2::node_view<toml::node> database,const toml::v2::node_view<toml::node>& default_config)
{
  if(database["db_path"].as_string()==nullptr)
    database.as_table()->insert("db_path",default_config["db_path"]);
  if (auto temp = database["db_path"].value<std::string>(); temp.has_value())
    this->path = QString::fromStdString(temp.value());

  if(database["db_backend"].as_string()==nullptr)
    database.as_table()->insert("db_backend",default_config["db_backend"]);
  if (auto temp = database["db_backend"].value<std::string>(); temp.has_value())
    this->backend = QString::fromStdString(temp.value());

  if(database["auth"].as_table()==nullptr)
    database.as_table()->insert("auth",default_config["auth"]);

  if(database["auth"]["enable"].as_boolean()==nullptr)
    database["auth"].as_table()->insert("enable",default_config["auth"]["enable"]);
  if (auto temp = database["auth"]["enable"].value<bool>();
      temp.has_value() && temp.value() == true) {
    this->auth.enable = temp.value();

    auto auth = database["auth"];

    if(database["auth"]["username"].as_string()==nullptr)
      database["auth"].as_table()->insert("username",default_config["auth"]["username"]);
    if (auto auth_temp = auth["username"].value<std::string>();
        auth_temp.has_value())
      this->auth.username = QString::fromStdString(auth_temp.value());

    if(database["auth"]["password"].as_string()==nullptr)
      database["auth"].as_table()->insert("password",default_config["auth"]["password"]);
    if (auto auth_temp = auth["password"].value<std::string>();
        auth_temp.has_value())
      this->auth.password = QString::fromStdString(auth_temp.value());

    if(database["auth"]["address"].as_string()==nullptr)
      database["auth"].as_table()->insert("address",default_config["auth"]["address"]);
    if (auto auth_temp = auth["address"].value<std::string>();
        auth_temp.has_value())
      this->auth.address = QString::fromStdString(auth_temp.value());

    if(!database["auth"]["port"].value<uint>().has_value())
      database["auth"].as_table()->insert("port",default_config["auth"]["port"]);
    if (auto auth_temp = database["auth"]["port"].value<uint>();
        auth_temp.has_value())
      this->auth.port = auth_temp.value();
  }
}

void
DataBase::toNodeView(const toml::v2::node_view<toml::node>& database,const toml::v2::node_view<toml::node>& default_config)
{
  if(database["db_path"].as_string()==nullptr)
    database.as_table()->insert("db_path",default_config["db_path"]);
  *database["db_path"].as_string() = this->path.toStdString();

  if(database["db_backend"].as_string()==nullptr)
    database.as_table()->insert("db_backend",default_config["db_backend"]);
  *database["db_backend"].as_string() = this->backend.toStdString();

  if(database["auth"].as_table()==nullptr)
    database.as_table()->insert("auth",default_config["auth"]);
  if (this->auth.enable) {
    if(database["auth"]["enable"].as_boolean()==nullptr)
      database["auth"].as_table()->insert("enable",default_config["auth"]["enable"]);
    *database["auth"]["enable"].as_boolean() = this->auth.enable;

    if(database["auth"]["username"].as_string()==nullptr)
      database["auth"].as_table()->insert("username",default_config["auth"]["username"]);
    *database["auth"]["username"].as_string() =
      this->auth.username.toStdString();

    if(database["auth"]["password"].as_string()==nullptr)
      database["auth"].as_table()->insert("password",default_config["auth"]["password"]);
    *database["auth"]["password"].as_string() =
      this->auth.password.toStdString();

    if(database["auth"]["address"].as_string()==nullptr)
      database["auth"].as_table()->insert("address",default_config["auth"]["address"]);
    *database["auth"]["address"].as_string() = this->auth.address.toStdString();

    if(database["auth"]["port"].as_integer()==nullptr)
      database["auth"].as_table()->insert("port",default_config["auth"]["port"]);
    *database["auth"]["port"].as_integer() = this->auth.port;
  }
}

void
Core::fromNodeView(toml::v2::node_view<toml::node> core,const toml::v2::node_view<toml::node>& default_config)
{
  if(core["core_path"].as_string()==nullptr)
    core.as_table()->insert("core_path",default_config["core_path"]);
  if (auto temp = core["core_path"].value<std::string>(); temp.has_value())
    this->core_path = QString::fromStdString(temp.value());

  if(core["assets_path"].as_string()==nullptr)
    core.as_table()->insert("assets_path",default_config["assets_path"]);
  if (auto temp = core["assets_path"].value<std::string>(); temp.has_value())
    this->assets_path = QString::fromStdString(temp.value());

  if(core["log_level"].as_string()==nullptr)
    core.as_table()->insert("log_level",default_config["log_level"]);
  if (auto temp = core["log_level"].value<std::string>(); temp.has_value())
    this->log_level = QString::fromStdString(temp.value());

  if(core["log_lines"].as_integer()==nullptr)
    core.as_table()->insert("log_lines",default_config["log_lines"]);
  if (auto temp = core["log_lines"].value<int>(); temp.has_value())
    this->log_lines = temp.value();

  if(core["api"].as_table()==nullptr)
    core.as_table()->insert("api",default_config["api"]);

  if(core["api"]["enable"].as_boolean()==nullptr)
    core.as_table()->insert("enable",default_config["api"]["enable"]);
  if (auto temp = core["api"]["enable"].value<bool>();
      temp.has_value() && temp.value() == true) {
    this->api.enable = temp.value();

    if(core["api"]["port"].as_integer()==nullptr)
      core.as_table()->insert("port",default_config["api"]["port"]);
    if (auto api_temp = core["api"]["port"].value<uint>();
        api_temp.has_value()) {
      this->api.port = api_temp.value();
    }
  }
}

void
Core::toNodeView(const toml::v2::node_view<toml::node>& core,const toml::v2::node_view<toml::node>& default_config)
{
  if(core["core_path"].as_string()==nullptr)
    core.as_table()->insert("core_path",default_config["core_path"]);
  *core["core_path"].as_string() = core_path.toStdString();

  if(core["assets_path"].as_string()==nullptr)
    core.as_table()->insert("assets_path",default_config["assets_path"]);
  *core["assets_path"].as_string() = assets_path.toStdString();

  if(core["log_level"].as_string()==nullptr)
    core.as_table()->insert("log_level",default_config["log_level"]);
  *core["log_level"].as_string() = log_level.toStdString();

  if(core["log_lines"].as_integer()==nullptr)
    core.as_table()->insert("log_lines",default_config["log_lines"]);
  *core["log_lines"].as_integer() = log_lines;

  if(core["api"].as_table()==nullptr)
    core.as_table()->insert("api",default_config["api"]);

  if(core["api"]["enable"].as_boolean()==nullptr)
    core.as_table()->insert("enable",default_config["api"]["enable"]);
  *core["api"]["enable"].as_boolean() = api.enable;

  if(core["api"]["port"].as_integer()==nullptr)
    core.as_table()->insert("port",default_config["api"]["port"]);
  *core["api"]["port"].as_integer() = api.port;
}

void
Theme::Colors::fromNodeView(toml::v2::node_view<toml::node> colors_node)
{
  if (auto temp = colors_node["text_color"].value<std::string>();
      temp.has_value())
    this->text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["background_color"].value<std::string>();
      temp.has_value())
    this->background_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["highlight_color"].value<std::string>();
      temp.has_value())
    this->highlight_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["highlight_text_color"].value<std::string>();
      temp.has_value())
    this->highlight_text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["warn_color"].value<std::string>();
      temp.has_value())
    this->warn_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["warn_text_color"].value<std::string>();
      temp.has_value())
    this->warn_text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["shadow_color"].value<std::string>();
      temp.has_value())
    this->shadow_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["border_color"].value<std::string>();
      temp.has_value())
    this->border_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["deep_color"].value<std::string>();
      temp.has_value())
    this->deep_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["deep_text_color"].value<std::string>();
      temp.has_value())
    this->deep_text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["style_color"].value<std::string>();
      temp.has_value())
    this->style_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["style_text_color"].value<std::string>();
      temp.has_value())
    this->style_text_color = QString::fromStdString(temp.value());
}

void
InboundSettings::SOCKS::fromNodeView(toml::v2::node_view<toml::node> socks,const toml::v2::node_view<toml::node>& default_config)
{
  if(socks["enable"].as_boolean()==nullptr)
    socks.as_table()->insert("enable",default_config["enable"]);
  if (auto temp = socks["enable"].value<bool>();
      temp.has_value() && temp.value() == true) {
    this->enable = temp.value();

    if(socks["listen"].as_string()==nullptr)
      socks.as_table()->insert("listen",default_config["listen"]);
    if (auto socks_temp = socks["listen"].value<std::string>();
        socks_temp.has_value())
      this->listen = QString::fromStdString(socks_temp.value());

    if(socks["port"].as_integer()==nullptr)
      socks.as_table()->insert("port",default_config["port"]);
    if (auto socks_temp = socks["port"].value<uint>(); socks_temp.has_value())
      this->port = socks_temp.value();

    if(socks["udp"].as_boolean()==nullptr)
      socks.as_table()->insert("udp",default_config["udp"]);
    if (auto socks_temp = socks["udp"].value<bool>(); socks_temp.has_value())
      this->udp = socks_temp.value();

    if(socks["ip"].as_string()==nullptr)
      socks.as_table()->insert("ip",default_config["ip"]);
    if (auto socks_temp = socks["ip"].value<std::string>();
        socks_temp.has_value())
      this->ip = QString::fromStdString(socks_temp.value());

    if(socks["user_level"].as_integer()==nullptr)
      socks.as_table()->insert("user_level",default_config["user_level"]);
    if (auto socks_temp = socks["user_level"].value<int>();
        socks_temp.has_value())
      this->user_level = socks_temp.value();

    if(socks["auth"].as_table()==nullptr)
      socks.as_table()->insert("auth",default_config["auth"]);

    if(socks["auth"]["username"].as_string()==nullptr)
      socks["auth"].as_table()->insert("username",default_config["auth"]["username"]);
    if (auto socks_temp = socks["auth"]["username"].value<std::string>();
        socks_temp.has_value())
      this->username = QString::fromStdString(socks_temp.value());

    if(socks["auth"]["password"].as_string()==nullptr)
      socks["auth"].as_table()->insert("password",default_config["auth"]["password"]);
    if (auto socks_temp = socks["auth"]["password"].value<std::string>();
        socks_temp.has_value())
      this->password = QString::fromStdString(socks_temp.value());
  }
}

void
InboundSettings::SOCKS::toNodeView(const toml::v2::node_view<toml::node>& socks,const toml::v2::node_view<toml::node>& default_config)
{
  if(socks["enable"].as_boolean()==nullptr)
    socks.as_table()->insert("enable",default_config["enable"]);
  *socks["enable"].as_boolean() = enable;

  if(socks["listen"].as_string()==nullptr)
    socks.as_table()->insert("listen",default_config["listen"]);
  *socks["listen"].as_string() = listen.toStdString();

  if(socks["port"].as_integer()==nullptr)
    socks.as_table()->insert("port",default_config["port"]);
  *socks["port"].as_integer() = port;

  if(socks["udp"].as_boolean()==nullptr)
    socks.as_table()->insert("udp",default_config["udp"]);
  *socks["udp"].as_boolean() = udp;
  if(socks["ip"].as_string()==nullptr)
    socks.as_table()->insert("ip",default_config["ip"]);
  *socks["ip"].as_string() = ip.toStdString();
  if(socks["user_level"].as_integer()==nullptr)
    socks.as_table()->insert("user_level",default_config["user_level"]);
  *socks["user_level"].as_integer() = user_level;

  if(socks["auth"].as_table()==nullptr)
    socks.as_table()->insert("auth",default_config["auth"]);

  if(socks["auth"]["username"].as_string()==nullptr)
    socks["auth"].as_table()->insert("username",default_config["auth"]["username"]);
  *socks["auth"]["username"].as_string() = username.toStdString();

  if(socks["auth"]["password"].as_string()==nullptr)
    socks["auth"].as_table()->insert("password",default_config["auth"]["password"]);
  *socks["auth"]["password"].as_string() = password.toStdString();
}

InboundObject
InboundSettings::SOCKS::toInboundObject()
{
  InboundObject inbound_object;

  inbound_object.listen = this->listen.toStdString();
  inbound_object.port = std::to_string(this->port);
  inbound_object.protocol = this->protocol.toStdString();

  SocksObject::InboundSettingObject inbound_setting_object;

  if (!this->username.isEmpty()) {
    SocksObject::AccountObject account_object;

    account_object.username = this->username.toStdString();
    account_object.password = this->password.toStdString();

    inbound_setting_object.appendAccountObject(account_object);
  }

  inbound_setting_object.ip = this->ip.toStdString();
  inbound_setting_object.udp = this->udp;
  inbound_setting_object.user_level = this->user_level;

  inbound_object.settings = inbound_setting_object.toObject();

  return inbound_object;
}

void
InboundSettings::HTTP::fromNodeView(toml::v2::node_view<toml::node> http,const toml::v2::node_view<toml::node>& default_config)
{
  if(http["enable"].as_boolean()==nullptr)
    http.as_table()->insert("enable",default_config["enable"]);
  if (auto temp = http["enable"].value<bool>();
      temp.has_value() && temp.value() == true) {
    this->enable = temp.value();

    if(http["listen"].as_string()==nullptr)
      http.as_table()->insert("listen",default_config["listen"]);
    if (auto http_temp = http["listen"].value<std::string>();
        http_temp.has_value())
      this->listen = QString::fromStdString(http_temp.value());

    if(http["port"].as_integer()==nullptr)
      http.as_table()->insert("port",default_config["port"]);
    if (auto http_temp = http["port"].value<uint>(); http_temp.has_value())
      this->port = http_temp.value();

    if(http["allow_transparent"].as_boolean()==nullptr)
      http.as_table()->insert("allow_transparent",default_config["allow_transparent"]);
    if (auto http_temp = http["allow_transparent"].value<bool>();
        http_temp.has_value())
      this->allow_transparent = http_temp.value();

    if(http["timeout"].as_integer()==nullptr)
      http.as_table()->insert("timeout",default_config["timeout"]);
    if (auto http_temp = http["timeout"].value<int>(); http_temp.has_value())
      this->timeout = http_temp.value();

    if(http["user_level"].as_integer()==nullptr)
      http.as_table()->insert("user_level",default_config["user_level"]);
    if (auto http_temp = http["user_level"].value<int>(); http_temp.has_value())
      this->user_level = http_temp.value();

    if(http["auth"].as_table()==nullptr)
      http.as_table()->insert("auth",default_config["auth"]);

    if(http["auth"]["username"].as_string()==nullptr)
      http["auth"].as_table()->insert("username",default_config["auth"]["username"]);
    if (auto http_temp = http["auth"]["username"].value<std::string>();
        http_temp.has_value())
      this->username = QString::fromStdString(http_temp.value());

    if(http["auth"]["password"].as_string()==nullptr)
      http["auth"].as_table()->insert("password",default_config["auth"]["password"]);
    if (auto http_temp = http["auth"]["password"].value<std::string>();
        http_temp.has_value())
      this->password = QString::fromStdString(http_temp.value());
  }
}

void
InboundSettings::HTTP::toNodeView(const toml::v2::node_view<toml::node>& http,const toml::v2::node_view<toml::node>& default_config)
{
  if(http["enable"].as_boolean()==nullptr)
    http.as_table()->insert("enable",default_config["enable"]);
  *http["enable"].as_boolean() = enable;

  if(http["listen"].as_string()==nullptr)
    http.as_table()->insert("listen",default_config["listen"]);
  *http["listen"].as_string() = listen.toStdString();

  if(http["port"].as_integer()==nullptr)
    http.as_table()->insert("port",default_config["port"]);
  *http["port"].as_integer() = port;

  if(http["allow_transparent"].as_boolean()==nullptr)
    http.as_table()->insert("allow_transparent",default_config["allow_transparent"]);
  *http["allow_transparent"].as_boolean() = allow_transparent;

  if(http["timeout"].as_integer()==nullptr)
    http.as_table()->insert("timeout",default_config["timeout"]);
  *http["timeout"].as_integer() = timeout;

  if(http["user_level"].as_integer()==nullptr)
    http.as_table()->insert("user_level",default_config["user_level"]);
  *http["user_level"].as_integer() = user_level;

  if(http["auth"].as_table()==nullptr)
    http.as_table()->insert("auth",default_config["auth"]);

  if(http["auth"]["username"].as_string()==nullptr)
    http["auth"].as_table()->insert("username",default_config["auth"]["username"]);
  *http["auth"]["username"].as_string() = username.toStdString();

  if(http["auth"]["password"].as_string()==nullptr)
    http["auth"].as_table()->insert("password",default_config["auth"]["password"]);
  *http["auth"]["password"].as_string() = password.toStdString();
}

InboundObject
InboundSettings::HTTP::toInboundObject()
{
  InboundObject inbound_object;

  inbound_object.listen = this->listen.toStdString();
  inbound_object.port = std::to_string(this->port);
  inbound_object.protocol = this->protocol.toStdString();

  HttpObject::InboundSettingObject inbound_setting_object;

  if (!this->username.isEmpty()) {
    HttpObject::AccountObject account_object;

    account_object.username = this->username.toStdString();
    account_object.password = this->password.toStdString();

    inbound_setting_object.appendAccountObject(account_object);
  }

  inbound_setting_object.timeout = this->timeout;
  inbound_setting_object.allow_transparent = this->allow_transparent;
  inbound_setting_object.user_level = this->user_level;

  inbound_object.settings = inbound_setting_object.toObject();

  return inbound_object;
}

void
InboundSettings::fromNodeView(toml::v2::node_view<toml::node> inbound,const toml::v2::node_view<toml::node>& default_config)
{
  socks.fromNodeView(inbound["socks"],default_config["socks"]);
  http.fromNodeView(inbound["http"],default_config["http"]);
}

void
InboundSettings::toNodeView(const toml::v2::node_view<toml::node>& inbound,const toml::v2::node_view<toml::node>& default_config)
{
    socks.toNodeView(inbound["socks"],default_config["socks"]);
    http.toNodeView(inbound["http"],default_config["http"]);
}

void
InboundSettings::setObject(Json::Value& root)
{
  InboundObjects inbound_objects;

  if (socks.enable) {
    inbound_objects.appendInboundObject(socks.toInboundObject());
  }

  if (http.enable) {
    inbound_objects.appendInboundObject(http.toInboundObject());
  }

  inbound_objects.setObject(root);
}

ConfigTools::ConfigTools(QObject* parent) {}

bool
ConfigTools::init(QSharedPointer<LogView> log_view, const QString& file_path)
{
  bool result = false;

  // initial config logger
  p_logger = std::make_shared<LogTools>(log_view, "setting");

  QString config_path = loadConfigPath(file_path);

  connect(
    this, &ConfigTools::configChanged, this, [&]() { this->saveConfig(); });

  do {
    result = true;

    QFile example_file(":/misc/across_example.toml");
    if (!example_file.open(QIODevice::ReadOnly | QIODevice::Text)){
      p_logger->error("Failed to read default config file");
      break;
    }
    QString default_config_text = example_file.readAll();

    this->m_default_config = toml::parse(default_config_text.toStdString());


    if (config_path.isEmpty()) {
      p_logger->warn("Failed to load config path");
      break;
    } else {
      this->m_config_path = config_path;

      this->m_config = toml::parse_file(config_path.toStdString());
      if (m_config.empty()) {
        p_logger->error("Failed to parse config");
        for(auto i : m_default_config){
          m_config.insert(i.first,i.second);
          printf("%s\n",i.first.c_str());
        }
      }
    }

    if (!(result = loadCoreConfig())) {
      p_logger->error("Failed to load core config");
    }

    if (!(result = loadDBConfig())) {
      p_logger->error("Failed to load database config");
      break;
    }

    if (!(result = loadInterfaceConfig())) {
      p_logger->error("Failed to load interface config");
    }

    if (!(result = loadThemeConfig())) {
      p_logger->error("Failed to load colors config");
    }

    if (!(result = loadInboundConfig())) {
      p_logger->error("Failed to load inbounds config");
    }

    emit configChanged();
  } while (false);

  return result;
}

QString
ConfigTools::loadConfigPath(const QString& file_path)
{
  QString config_path = "./across.toml";

  // Setting Path > Env Path > Current Path > Default Path
  do {
    if (isFileExist(file_path)) {
      config_path = file_path;
      break;
    }

    auto env_path = m_envs.getInfo().ACROSS_CONFIG_PATH;
    if (!env_path.isEmpty()) {
      config_path = env_path;

      if (isFileExist(config_path)) {
        break;
      }
    }

    auto env_config_home = m_envs.get("XDG_CONFIG_HOME");
    if (env_config_home.isEmpty()) {
      env_config_home = m_envs.get("HOME").append("/.config");
    }

    QDir xdg_path(env_config_home + "/across/");
    auto temp_config = xdg_path.filePath(m_config_name);
    if (isFileExist(temp_config)) {
      config_path = temp_config;
      break;
    } else {
      // create directory and example config file
      if (!xdg_path.exists()) {
        xdg_path.mkdir(xdg_path.path());
      }

      QFile config_file(temp_config);
      if (!config_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        break;
      }

      QFile example_config(":/misc/across_example.toml");
      if (!example_config.open(QIODevice::ReadOnly | QIODevice::Text)) {
        break;
      }

      config_file.write(example_config.readAll());
      config_file.close();
      config_path = temp_config;

      p_logger->info("Generate new config on: {}", temp_config.toStdString());
    }
  } while (false);

  return config_path;
}

bool
ConfigTools::loadInterfaceConfig()
{
  bool result = false;

  do {
    result = true;

    if (m_config.empty() || !m_config["interface"].is_table()) {
      m_config.insert_or_assign("interface",*m_default_config["interface"].as_table());
      result = false;
    }

    m_interface.fromNodeView(m_config["interface"],m_default_config["interface"]);

  } while (false);

  return result;
}

bool
ConfigTools::loadInterfaceTheme()
{
  bool result = false;

  do {
    if (m_config.empty() || !m_config["interface"]["theme"].is_table()) {
      break;
    }

    if(m_config["interface"]["theme"].as_table()==nullptr)
      m_config["interface"].as_table()->insert("theme",m_default_config["interface"]["theme"]);
    m_interface.theme.fromNodeView(m_config["interface"]["theme"],m_default_config["interface"]["theme"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadInterfaceLanguage()
{
  if (m_config.empty())
    return false;

  m_interface.fromNodeView(m_config["interface"],m_default_config["interface"]);
  return true;
}

bool
ConfigTools::loadUpdateConfig()
{
  bool result = false;

  do {
    if (m_config.empty() || !m_config["update"].is_table()) {
      break;
    }

    m_update.fromNodeView(m_config["update"],m_default_config["update"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadDBConfig()
{
  if (auto temp = m_config["database"]["db_path"].value<std::string>();
      temp.has_value() && temp != m_db.path.toStdString()) {
    setDBPath(QString::fromStdString(temp.value()), true);

    return true;
  } else {
    p_logger->warn("Failed to get the db_path, use default path `{}`",
                   m_db.path.toStdString());
  }

  return isFileExist(m_db.path);
}

bool
ConfigTools::loadThemeConfig()
{
  auto themes = m_config["themes"];
  if (!themes.as_array()||themes.as_array()->size()==0){
    m_config.as_table()->insert("themes",m_default_config["themes"]);
    themes=m_config["themes"];
  }

  if (themes.as_array()->empty()) {
    p_logger->error("No themes configuration");
    return false;
  }

  for (int i = 0; i < themes.as_array()->size(); ++i) {
    if (auto temp = themes[i]["name"].value<std::string>(); temp.has_value()) {
      if (auto temp_name = QString::fromStdString(temp.value());
          m_interface.theme.theme == temp_name) {
        m_theme.name = temp_name;

        if (auto theme_temp = themes[i]["tray"]["stylish"].value<std::string>();
            theme_temp.has_value())
          m_theme.tray.stylish = QString::fromStdString(theme_temp.value());

        if (auto theme_temp = themes[i]["tray"]["color"].value<std::string>();
            theme_temp.has_value())
          m_theme.tray.color = QString::fromStdString(theme_temp.value());

        if (auto theme_temp = themes[i]["border"]["radius"].value<int>();
            theme_temp.has_value())
          m_theme.border.radius = theme_temp.value();

        if (auto theme_temp = themes[i]["border"]["width"].value<int>();
            theme_temp.has_value())
          m_theme.border.width = theme_temp.value();

        if (auto theme_temp = themes[i]["item"]["spacing"].value<int>();
            theme_temp.has_value())
          m_theme.item.spacing = theme_temp.value();

        if (auto theme_temp = themes[i]["icon"]["style"].value<std::string>();
            theme_temp.has_value())
          m_theme.icon.style = QString::fromStdString(theme_temp.value());

        m_theme.colors.fromNodeView(themes[i]["colors"]);
        return true;
      }
    }
  }

  m_config["interface"].as_table()->insert("theme",*m_default_config["interface"]["theme"].as_table());

  return false;
}

bool
ConfigTools::loadCoreConfig()
{
  bool result = false;
  auto envs_info = m_envs.getInfo();

  do {
    result = true;

    if (m_config.empty() || !m_config["core"].is_table()) {
      m_config.insert_or_assign("core",*m_default_config["core"].as_table());
      result = false;
    }

    m_core.fromNodeView(m_config["core"],m_default_config["core"]);

    if (m_core.core_path.isEmpty()) {
      if (!envs_info.V2RAY_CORE_PATH.isEmpty()) {
        m_core.core_path = envs_info.V2RAY_CORE_PATH;
      } else {
        m_core.core_path = "/usr/bin/v2ray";
      }
    }

    if (!isFileExist(m_core.core_path)) {
      p_logger->error("Core file doesn't exist");
      m_core.core_path = ""; // reset the core path
      break;
    }

    if (m_core.assets_path.isEmpty()) {
      if (!envs_info.V2RAY_ASSETS_PATH.isEmpty()) {
        m_core.assets_path = envs_info.V2RAY_ASSETS_PATH;
      } else {
        m_core.assets_path = "/usr/share/v2ray";
      }
    }

    if (!isFileExist(m_core.assets_path + "/geosite.dat") ||
        !isFileExist(m_core.assets_path + "/geoip.dat")) {
      p_logger->error("Geographic files doesn't exist");
      break;
    }

  } while (false);

  return result;
}

bool
ConfigTools::loadInboundConfig()
{
  bool result = true;
  if (m_config.empty()){
    m_config.insert("inbound",*m_default_config["inbound"].as_table());
    result = false;
  }

  if (m_config["inbound"]["socks"].as_table()==nullptr){
    m_config.insert_or_assign("inbound",*m_default_config["inbound"].as_table());
    result = false;
  }
  if (m_config["inbound"]["http"].as_table()==nullptr){
    m_config.insert_or_assign("inbound",*m_default_config["inbound"].as_table());
    result = false;
  }

  m_inbound.fromNodeView(m_config["inbound"],m_default_config["inbound"]);
  return result;
}

bool
ConfigTools::loadNetworkConfig()
{
  if (m_config.empty())
    return false;

  m_network.fromNodeView(m_config["network"],m_default_config["network"]);
  return true;
}

InboundSettings
ConfigTools::getInboundConfig()
{
  return m_inbound;
}

DataBase
ConfigTools::getDBConfig()
{
  return m_db;
}

QString
ConfigTools::getConfigVersion()
{
  if (auto temp = m_config["config_version"].value<std::string>();
      temp.has_value()) {
    return QString::fromStdString(temp.value());
  }

  return {};
}

QString
ConfigTools::getConfigTomlVersion()
{
  auto toml_version =
    QString::fromStdString(*m_config["toml_version"].value<std::string>());

  return toml_version;
}

QString
ConfigTools::getLanguage()
{
  auto language = QString::fromStdString(
    *m_config["interface"]["language"].value<std::string>());

  return language;
}

void
ConfigTools::freshColors()
{
  if (loadThemeConfig()) {
    emit textColorChanged();
    emit backgroundColorChanged();
    emit highlightColorChanged();
    emit highlightTextColorChanged();
    emit warnColorChanged();
    emit warnTextColorChanged();
    emit shadowColorChanged();
    emit borderColorChanged();
    emit deepColorChanged();
    emit deepTextColorChanged();
    emit styleColorChanged();
    emit styleTextColorChanged();
    emit borderRadiusChanged();
    emit borderWidthChanged();
    emit itemSpacingChanged();
    emit iconStyleChanged();
  }
}

bool
ConfigTools::testApi()
{
  bool result = false;

  if (m_core.api.enable) {
    APITools client(m_core.api.port);
    auto [stats, err] = client.isOk();

    if (stats) {
      m_api_result_text = "";
    } else {
      m_api_result_text = QString::fromStdString(err);
    }

    result = stats;
  }

  emit apiResultTextChanged();

  return result;
}

bool
ConfigTools::testAndSetAddr(const QString& addr)
{
  bool result = false;
  QHostAddress temp_addr(addr);

  switch (temp_addr.protocol()) {
    case QAbstractSocket::IPv4Protocol:
    case QAbstractSocket::IPv6Protocol:
      result = true;
      setInboundAddress(addr);
      break;
    default:
      break;
  }

  return result;
}

void
ConfigTools::freshInbound()
{
  if (loadInboundConfig()) {
    emit socksEnableChanged();
    emit socksPortChanged();
    emit socksUsernameChanged();
    emit socksPasswordChanged();
    emit httpEnableChanged();
    emit httpPortChanged();
    emit httpUsernameChanged();
    emit httpPasswordChanged();
  }
}

bool
ConfigTools::saveConfig(QString config_path)
{
  bool result = false;
  QString path;
  std::ofstream config;
  QDateTime now = QDateTime::currentDateTimeUtc();

  do {
    if (!config_path.isEmpty()) {
      path = config_path;
      break;
    }

    if (!m_config_path.isEmpty()) {
      path = m_config_path;
      break;
    }
  } while (false);

  if (!path.isEmpty()) {
    m_db.toNodeView(m_config["database"],m_default_config["database"]);
    m_core.toNodeView(m_config["core"],m_default_config["core"]);
    m_inbound.toNodeView(m_config["inbound"],m_default_config["inbound"]);
    m_network.toNodeView(m_config["network"],m_default_config["network"]);
    m_interface.toNodeView(m_config["interface"],m_default_config["interface"]);

    config.open(path.toStdString(), std::ios::out | std::ios::trunc);

    config << "# Generate Time: " << now.toString().toStdString() << "\n"
           << toml::default_formatter(m_config);

    config.close();
    result = true;
  }

  return result;
}

void
ConfigTools::setDBPath(const QString& db_path, bool init)
{
  auto temp_path = db_path;

#ifdef Q_OS_LINUX
  if ((temp_path == "~") || (temp_path.startsWith("~/"))) {
    temp_path.replace(0, 1, QDir::homePath());
  }

  if (temp_path.startsWith("$")) {
    wordexp_t p;
    wordexp(db_path.toStdString().c_str(), &p, 0);
    temp_path = QString::fromStdString(*p.we_wordv);
    wordfree(&p);
  }
#endif

  if (temp_path.startsWith("file:///")) {
    temp_path = QUrl(temp_path).toLocalFile();
  }

  if (!temp_path.split("/").last().contains(".db")) {
    QDir db_dir(temp_path);
    m_db.path = db_dir.relativeFilePath("across.db");
  } else {
    QFileInfo db_file(temp_path);
    m_db.path = db_file.filePath();
  }

  auto dir = QFileInfo(m_db.path).dir();
  if (!dir.exists()) {
    QDir().mkdir(dir.path());
  }

  // auto save config
  if (!init) {
    emit configChanged();
  }

  emit dbPathChanged();
}

toml::table
ConfigTools::getConfig()
{
  return m_config;
}

QString
ConfigTools::textColor()
{
  return m_theme.colors.text_color;
}

QString
ConfigTools::backgroundColor()
{
  return m_theme.colors.background_color;
}

QString
ConfigTools::highlightColor()
{
  return m_theme.colors.highlight_color;
}

QString
ConfigTools::highlightTextColor()
{
  return m_theme.colors.highlight_text_color;
}

QString
ConfigTools::warnColor()
{
  return m_theme.colors.warn_color;
}

QString
ConfigTools::warnTextColor()
{
  return m_theme.colors.warn_text_color;
}

QString
ConfigTools::shadowColor()
{
  return m_theme.colors.shadow_color;
}

QString
ConfigTools::borderColor()
{
  return m_theme.colors.border_color;
}

QString
ConfigTools::deepColor()
{
  return m_theme.colors.deep_color;
}

QString
ConfigTools::deepTextColor()
{
  return m_theme.colors.deep_text_color;
}

QString
ConfigTools::styleColor()
{
  return m_theme.colors.style_color;
}

QString
ConfigTools::styleTextColor()
{
  return m_theme.colors.style_text_color;
}

QString
ConfigTools::trayStylish()
{
  return m_theme.tray.stylish;
}

QString
ConfigTools::trayColor()
{
  return m_theme.tray.color;
}

int
ConfigTools::borderRadius()
{
  return m_theme.border.radius;
}

int
ConfigTools::borderWidth()
{
  return m_theme.border.width;
}

int
ConfigTools::itemSpacing()
{
  return m_theme.item.spacing;
}

const QString&
ConfigTools::iconStyle() const
{
  return m_theme.icon.style;
}

const QString&
ConfigTools::currentTheme() const
{
  return m_interface.theme.theme;
}

bool
ConfigTools::socksEnable()
{
  return m_inbound.socks.enable;
}

QString
ConfigTools::socksPort()
{
  return QString::fromStdString(std::to_string(m_inbound.socks.port));
}

QString
ConfigTools::socksUsername()
{
  return m_inbound.socks.username;
}

QString
ConfigTools::socksPassword()
{
  return m_inbound.socks.password;
}

bool
ConfigTools::httpEnable()
{
  return m_inbound.http.enable;
}

QString
ConfigTools::httpPort()
{
  return QString::fromStdString(std::to_string(m_inbound.http.port));
}

QString
ConfigTools::httpUsername()
{
  return m_inbound.http.username;
}

QString
ConfigTools::httpPassword()
{
  return m_inbound.http.password;
}

void
ConfigTools::setCorePath(const QUrl& val)
{
  QString path = val.toLocalFile();

  if (path == m_core.core_path) {
    return;
  }

  if (isFileExist(path)) {
    m_core.core_path = path;

    emit configChanged();
    emit corePathChanged();
  }
}

void
ConfigTools::setAssetsPath(const QUrl& val)
{
  QString path = val.toLocalFile();

  if (path == m_core.assets_path || path.isEmpty()) {
    return;
  }

  m_core.assets_path = path;

  emit configChanged();
  emit assetsPathChanged();
}

void
ConfigTools::setLogLevel(const QString& log_level)
{
  if (log_level == m_core.log_level) {
    return;
  }

  m_core.log_level = log_level;

  emit configChanged();
  emit logLevelChanged();
}

void
ConfigTools::setLogLines(int log_lines)
{
  if (log_lines == m_core.log_lines) {
    return;
  }

  m_core.log_lines = log_lines;

  emit configChanged();
  emit logLinesChanged(m_core.log_lines);
}

void
ConfigTools::setApiEnable(bool val)
{
  if (val == m_core.api.enable) {
    return;
  }

  m_core.api.enable = val;

  emit configChanged();
  emit apiEnableChanged();
}

void
ConfigTools::setApiPort(QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == m_core.api.port || portStr.isEmpty()) {
    return;
  }

  if (port >= 0 && port <= 65535) {
    m_core.api.port = port;

    emit configChanged();
    emit apiPortChanged();
  }
}

void
ConfigTools::setInboundAddress(const QString& addr)
{
  if (addr.isEmpty() || addr == m_inbound.socks.listen) {
    return;
  }

  // Keep the listening address the same
  m_inbound.socks.listen = addr;
  m_inbound.http.listen = addr;

  emit configChanged();
  emit inboundAddressChanged();
}

void
ConfigTools::setTextColor(const QString& val)
{
  if (val == m_theme.colors.text_color) {
    return;
  }

  m_theme.colors.text_color = val;
  emit textColorChanged();
}

void
ConfigTools::setBackgroundColor(const QString& val)
{
  if (val == m_theme.colors.background_color) {
    return;
  }

  m_theme.colors.background_color = val;
  emit backgroundColorChanged();
}

void
ConfigTools::setHighlightColor(const QString& val)
{
  if (val == m_theme.colors.highlight_color) {
    return;
  }

  m_theme.colors.highlight_color = val;
  emit highlightColorChanged();
}

void
ConfigTools::setHighlightTextColor(const QString& val)
{
  if (val == m_theme.colors.highlight_text_color) {
    return;
  }

  m_theme.colors.highlight_text_color = val;
  emit highlightTextColorChanged();
}

void
ConfigTools::setWarnColor(const QString& val)
{
  if (val == m_theme.colors.warn_color) {
    return;
  }

  m_theme.colors.warn_color = val;
  emit warnColorChanged();
}

void
ConfigTools::setWarnTextColor(const QString& val)
{
  if (val == m_theme.colors.warn_text_color) {
    return;
  }

  m_theme.colors.warn_text_color = val;
  emit warnTextColorChanged();
}

void
ConfigTools::setShadowColor(const QString& val)
{
  if (val == m_theme.colors.shadow_color) {
    return;
  }

  m_theme.colors.shadow_color = val;
  emit shadowColorChanged();
}

void
ConfigTools::setBorderColor(const QString& val)
{
  if (val == m_theme.colors.border_color) {
    return;
  }

  m_theme.colors.border_color = val;
  emit borderColorChanged();
}

void
ConfigTools::setDeepColor(const QString& val)
{
  if (val == m_theme.colors.deep_color) {
    return;
  }

  m_theme.colors.deep_color = val;
  emit deepColorChanged();
}

void
ConfigTools::setDeepTextColor(const QString& val)
{
  if (val == m_theme.colors.deep_text_color) {
    return;
  }

  m_theme.colors.deep_text_color = val;
  emit deepTextColorChanged();
}

void
ConfigTools::setStyleColor(const QString& val)
{
  if (val == m_theme.colors.style_color) {
    return;
  }

  m_theme.colors.style_color = val;
  emit styleColorChanged();
}

void
ConfigTools::setStyleTextColor(const QString& val)
{
  if (val == m_theme.colors.style_text_color) {
    return;
  }

  m_theme.colors.style_text_color = val;
  emit styleTextColorChanged();
}

void
ConfigTools::setTrayStylish(const QString& val)
{
  if (val == m_theme.tray.stylish) {
    return;
  }

  m_theme.tray.stylish = val;
  emit trayStylishChanged();
}

void
ConfigTools::setTrayColor(const QString& val)
{
  if (val == m_theme.tray.color) {
    return;
  }

  m_theme.tray.color = val;
  emit trayColorChanged();
}

void
ConfigTools::setBorderRadius(int radius)
{
  if (radius == m_theme.border.radius) {
    return;
  }

  m_theme.border.radius = radius;
  emit borderRadiusChanged();
}

void
ConfigTools::setBorderWidth(int width)
{
  if (width == m_theme.border.width) {
    return;
  }

  m_theme.border.width = width;
  emit borderWidthChanged();
}

void
ConfigTools::setItemSpacing(int spacing)
{
  if (spacing == m_theme.item.spacing) {
    return;
  }

  m_theme.item.spacing = spacing;
  emit itemSpacingChanged();
}

void
ConfigTools::setIconStyle(const QString& newIconStyle)
{
  if (m_theme.icon.style == newIconStyle)
    return;

  m_theme.icon.style = newIconStyle;

  emit iconStyleChanged();
}

void
ConfigTools::setCurrentTheme(const QString& newCurrentTheme)
{
  if (m_interface.theme.theme == newCurrentTheme ||
      newCurrentTheme == "current" || newCurrentTheme.isEmpty())
    return;

  *m_config["interface"]["theme"]["theme"].as_string() =
    newCurrentTheme.toStdString();

  if (loadInterfaceTheme()) {
    freshColors();

    emit configChanged();
    emit currentThemeChanged();
    emit trayColorChanged();
    emit trayStylishChanged();
    emit borderColorChanged();
    emit borderRadiusChanged();
  }
}

void
ConfigTools::setSocksEnable(bool val)
{
  if (val == m_inbound.socks.enable) {
    return;
  }

  m_inbound.socks.enable = val;

  emit configChanged();
  emit socksEnableChanged();
}

void
ConfigTools::setSocksPort(const QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == m_inbound.socks.port || portStr.isEmpty()) {
    return;
  }

  m_inbound.socks.port = port;

  emit configChanged();
  emit socksPortChanged();
}

void
ConfigTools::setSocksUsername(const QString& name)
{
  if (name == m_inbound.socks.username) {
    return;
  }

  m_inbound.socks.username = name;

  emit configChanged();
  emit socksUsernameChanged();
}

void
ConfigTools::setSocksPassword(const QString& pass)
{
  if (pass == m_inbound.socks.password) {
    return;
  }

  m_inbound.socks.password = pass;

  emit configChanged();
  emit socksPasswordChanged();
}

void
ConfigTools::setHttpEnable(bool val)
{
  if (val == m_inbound.http.enable) {
    return;
  }

  m_inbound.http.enable = val;

  emit configChanged();
  emit httpEnableChanged();
}

void
ConfigTools::setHttpPort(QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == m_inbound.http.port || portStr.isEmpty()) {
    return;
  }

  m_inbound.http.port = port;

  emit configChanged();
  emit httpPortChanged();
}

void
ConfigTools::setHttpUsername(const QString& name)
{
  if (name == m_inbound.http.username) {
    return;
  }

  m_inbound.http.username = name;

  emit configChanged();
  emit httpUsernameChanged();
}

void
ConfigTools::setHttpPassword(const QString& pass)
{
  if (pass == m_inbound.http.password) {
    return;
  }

  m_inbound.http.password = pass;

  emit configChanged();
  emit httpPasswordChanged();
}

bool
ConfigTools::isFileExist(QString file_path)
{
  return QFile(file_path).exists();
}

QString
ConfigTools::guiVersion()
{
  return getVersion();
}

QString
ConfigTools::coreInfo()
{
  QProcess core_process;
  QString info;

  if (m_core.core_path.isEmpty()) {
    return info;
  }

  core_process.start(m_core.core_path, { "-version" });
  core_process.waitForFinished();

  if (core_process.exitStatus() == QProcess::NormalExit) {
    QString raw_info = core_process.readAllStandardOutput();

    if (!raw_info.isEmpty()) {
      info = raw_info;
    }
  }

  return info;
}

QString
ConfigTools::coreVersion()
{
  auto temp_info = coreInfo();
  if (temp_info.isEmpty()) {
    return "not found";
  }

  QString version =
    temp_info.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).at(1);
  if (!version.isEmpty()) {
    m_core.core_version = version;
  }

  return m_core.core_version;
}

QString
ConfigTools::corePath()
{
  return m_core.core_path;
}

QString
ConfigTools::assetsPath()
{
  return m_core.assets_path;
}

QString
ConfigTools::logLevel()
{
  return m_core.log_level;
}

int
ConfigTools::logLines()
{
  return m_core.log_lines;
}

QString
ConfigTools::dbPath()
{
  return m_db.path;
}

Core
ConfigTools::getCore()
{
  return m_core;
}

bool
ConfigTools::apiEnable()
{
  return m_core.api.enable;
}

QString
ConfigTools::apiPort()
{
  return QString::fromStdString(std::to_string(m_core.api.port));
}

QString
ConfigTools::apiResultText()
{
  return m_api_result_text;
}

QString
ConfigTools::inboundAddress()
{
  return m_inbound.socks.listen;
}

const QString&
ConfigTools::currentLanguage() const
{
  return m_interface.language.language;
}

bool
ConfigTools::enableTray()
{
  return m_interface.tray.enable;
}

const QString&
ConfigTools::networkTestMethod() const
{
  return m_network.test_method;
}

const QString&
ConfigTools::networkTestURL() const
{
  return m_network.test_url;
}

const QString&
ConfigTools::networkUserAgent() const
{
  return m_network.user_agent;
}

void
ConfigTools::setCurrentLanguage(const QString& newCurrentLanguage)
{
  if (m_interface.language.language == newCurrentLanguage ||
      newCurrentLanguage == "current" || newCurrentLanguage.isEmpty())
    return;

  m_interface.language.language = newCurrentLanguage;

  emit configChanged();
  emit currentLanguageChanged(m_interface.language.language);
}

void
ConfigTools::setEnableTray(bool val)
{
  if (m_interface.tray.enable == val)
    return;

  m_interface.tray.enable = val;

  emit configChanged();
  emit enableTrayChanged();
}

void
ConfigTools::setNetworkTestMethod(const QString& val)
{
  if (m_network.test_method == val)
    return;

  m_network.test_method = val;

  emit configChanged();
  emit networkTestMethodChanged();
}

void
ConfigTools::setNetworkTestURL(const QString& val)
{
  if (m_network.test_url == val)
    return;

  m_network.test_url = val;

  emit configChanged();
  emit networkTestURLChanged();
}

void
ConfigTools::setNetworkUserAgent(const QString& val)
{
  if (m_network.user_agent == val)
    return;

  m_network.user_agent = val;

  emit configChanged();
  emit networkUserAgentChanged();
}

QString
ConfigTools::buildInfo()
{
  return getBuildInfo();
}

QString
ConfigTools::extraInfo()
{
  return getExtraInfo();
}

QString
ConfigTools::buildTime()
{
  return getBuildTime();
}

QString
ConfigTools::sourceCodeURL()
{
  return getSourceCodeURL();
}

QString
ConfigTools::reportURL()
{
  return getReportURL();
}

QString
ConfigTools::licenseURL()
{
  return getLicenseURL();
}
