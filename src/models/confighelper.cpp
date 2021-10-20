#include "confighelper.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

using namespace google::protobuf::util;
using namespace across;
using namespace across::setting;

config::Config
ConfigHelper::defaultConfig()
{
  auto data_path =
    QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

  QDir data_dir(data_path);
  if (!data_dir.mkpath(".")) {
    qFatal("Failed to create data_path");
  }

  across::config::Config config;
  config.set_title("ACross Configuration Example");
  config.set_config_version("0.1.0");

  if (auto interface = config.mutable_interface()) {
    interface->set_language("en_US");
    interface->set_auto_connect(false);
    interface->set_auto_start(false);

    if (auto theme = interface->mutable_theme()) {
      theme->set_theme("default-light");
      theme->set_include_dir(
        (data_dir.filePath("themes") + QDir::separator()).toStdString());
    }

    if (auto tray = interface->mutable_tray()) {
      tray->set_enable(false);
      tray->set_start_from_minimize(false);
    }
  }

  if (auto update = config.mutable_update()) {
    update->set_auto_update(false);
    update->set_check_update(true);
    update->set_update_channel("stable");
    update->set_update_from_proxy(true);
  }

  if (auto database = config.mutable_database()) {
    database->set_db_path((data_dir.filePath("across.db")).toStdString());
    database->set_db_backend("sqlite3");

    if (auto auth = database->mutable_auth(); database->has_auth()) {
      auth->set_enable(false);
      auth->set_username("");
      auth->set_password("");
      auth->set_address("");
      auth->set_port(3000);
    }
  }

  if (auto core = config.mutable_core()) {
#if !defined Q_OS_WIN
    core->set_core_path("/usr/bin/v2ray");
    core->set_assets_path("/usr/share/v2ray/");
#else
    QDir working_dir(QCoreApplication::applicationDirPath());
    auto v2ray_dir = working_dir.filePath("v2ray").toStdString();
    core->set_core_path(v2ray_dir);
    core->set_assets_path(v2ray_dir);
#endif
    core->set_log_level("warning");
    core->set_log_lines(50);

    if (auto api = core->mutable_api(); core->has_api()) {
      api->set_enable(true);
      api->set_port(15491);
      api->set_listen("127.0.0.1");
    }
  }

  if (auto inbound = config.mutable_inbound()) {
    if (auto socks5 = inbound->mutable_socks5()) {
      socks5->set_enable(true);
      socks5->set_listen("127.0.0.1");
      socks5->set_port(1090);
      socks5->set_udp_enable(false);
      socks5->set_udp_ip("");
      socks5->set_user_level(0);

      if (auto auth = socks5->mutable_auth()) {
        auth->set_enable(false);
        auth->set_username("");
        auth->set_password("");
      }
    }

    if (auto http = inbound->mutable_http()) {
      http->set_enable(true);
      http->set_listen("127.0.0.1");
      http->set_port(8889);
      http->set_allow_transparent(false);
      http->set_timeout(0);
      http->set_user_level(0);

      if (auto auth = http->mutable_auth()) {
        auth->set_enable(false);
        auth->set_username("");
        auth->set_password("");
      }
    }
  }

  if (auto network = config.mutable_network()) {
    network->set_test_method("tcping");
    network->set_test_url("https://www.google.com");
    network->set_user_agent(
      "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) "
      "Chrome/99.0.7113.93 Safari/537.36");
  }

  if (auto theme = config.add_themes()) {
    theme->set_name("default-light");

    if (auto tray = theme->mutable_tray()) {
      tray->set_stylish("block");
      tray->set_color("light");
    }

    if (auto banner = theme->mutable_banner()) {
      banner->set_enable(false);
      banner->set_background_image("");
      banner->set_background_opacity(0.9);
    }

    if (auto colors = theme->mutable_colors()) {
      colors->set_text_color("#2f2f2f");
      colors->set_background_color("#fafafa");
      colors->set_highlight_color("#63a7d7");
      colors->set_highlight_text_color("#fefefe");
      colors->set_warn_color("#ff716a");
      colors->set_warn_text_color("#fefefe");
      colors->set_shadow_color("#29000000");
      colors->set_border_color("#27707070");
      colors->set_deep_color("#f2f2f2");
      colors->set_deep_text_color("#4f4f4f");
      colors->set_style_color("#94cbff");
      colors->set_style_text_color("#fefefe");
    }

    if (auto border = theme->mutable_border()) {
      border->set_radius(8);
      border->set_width(1);
    }

    if (auto item = theme->mutable_item()) {
      item->set_spacing(8);
    }

    if (auto icon = theme->mutable_icon()) {
      icon->set_style("light");
    }
  }

  if (auto theme = config.add_themes()) {
    theme->set_name("dark");

    if (auto tray = theme->mutable_tray()) {
      tray->set_stylish("block");
      tray->set_color("dark");
    }

    if (auto banner = theme->mutable_banner()) {
      banner->set_enable(false);
      banner->set_background_image("");
      banner->set_background_opacity(0.9);
    }

    if (auto colors = theme->mutable_colors()) {
      colors->set_text_color("#e3e3e3");
      colors->set_background_color("#252525");
      colors->set_highlight_color("#b7b27e");
      colors->set_highlight_text_color("#eceff4");
      colors->set_warn_color("#bf616a");
      colors->set_warn_text_color("#ffffff");
      colors->set_shadow_color("#29000000");
      colors->set_border_color("#bcb995c1");
      colors->set_deep_color("#1f1f1f");
      colors->set_deep_text_color("#c3c3c3");
      colors->set_style_color("#cfc29e");
      colors->set_style_text_color("#ffffff");
    }

    if (auto border = theme->mutable_border()) {
      border->set_radius(8);
      border->set_width(1);
    }

    if (auto item = theme->mutable_item()) {
      item->set_spacing(8);
    }

    if (auto icon = theme->mutable_icon()) {
      icon->set_style("dark");
    }
  }

  if (auto theme = config.add_themes()) {
    theme->set_name("nord-dark");

    if (auto tray = theme->mutable_tray()) {
      tray->set_stylish("block");
      tray->set_color("dark");
    }

    if (auto banner = theme->mutable_banner()) {
      banner->set_enable(false);
      banner->set_background_image("");
      banner->set_background_opacity(0.9);
    }

    if (auto colors = theme->mutable_colors()) {
      colors->set_text_color("#f3f3f3");
      colors->set_background_color("#3b4252");
      colors->set_highlight_color("#5e81ac");
      colors->set_highlight_text_color("#eceff4");
      colors->set_warn_color("#bf616a");
      colors->set_warn_text_color("#ffffff");
      colors->set_shadow_color("#29000000");
      colors->set_border_color("#3381a1c1");
      colors->set_deep_color("#2e3440");
      colors->set_deep_text_color("#e3e3e3");
      colors->set_style_color("#bbde5e");
      colors->set_style_text_color("#ffffff");
    }

    if (auto border = theme->mutable_border()) {
      border->set_radius(8);
      border->set_width(1);
    }

    if (auto item = theme->mutable_item()) {
      item->set_spacing(8);
    }

    if (auto icon = theme->mutable_icon()) {
      icon->set_style("dark");
    }
  }

  if (auto theme = config.add_themes()) {
    theme->set_name("pure-pink");

    if (auto tray = theme->mutable_tray()) {
      tray->set_stylish("block");
      tray->set_color("light");
    }

    if (auto banner = theme->mutable_banner()) {
      banner->set_enable(false);
      banner->set_background_image("");
      banner->set_background_opacity(0.9);
    }

    if (auto colors = theme->mutable_colors()) {
      colors->set_text_color("#5e4270");
      colors->set_background_color("#fefefe");
      colors->set_highlight_color("#f4b3cb");
      colors->set_highlight_text_color("#ffffff");
      colors->set_warn_color("#f1a983");
      colors->set_warn_text_color("#ffffff");
      colors->set_shadow_color("#29000000");
      colors->set_border_color("#bcb995c1");
      colors->set_deep_color("#fafafa");
      colors->set_deep_text_color("#5e4270");
      colors->set_style_color("#f9d7e5");
      colors->set_style_text_color("#ffffff");
    }

    if (auto border = theme->mutable_border()) {
      border->set_radius(8);
      border->set_width(1);
    }

    if (auto item = theme->mutable_item()) {
      item->set_spacing(8);
    }

    if (auto icon = theme->mutable_icon()) {
      icon->set_style("light");
    }
  }

  return config;
}

void
ConfigHelper::saveToFile(const std::string& content, const QString& file_path)
{
  QFile file(file_path);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    file.write(content.c_str());
    file.close();
  }
}

std::string
ConfigHelper::readFromFile(const QString& file_path)
{
  std::string content_str;

  QFile file(file_path);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    auto content = file.readAll();
    content_str = content.toStdString();
    file.close();
  }

  return content_str;
}
