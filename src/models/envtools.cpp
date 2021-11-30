#include "envtools.h"

using namespace across;

EnvTools::EnvTools() {
    m_info = {
        .ACROSS_CONFIG_PATH = get("ACROSS_CONFIG_PATH"),
        .ACROSS_DATA_DIR = get("ACROSS_DATA_DIR"),
        .V2RAY_CORE_PATH = get("V2RAY_CORE_PATH"),
        .V2RAY_ASSETS_PATH = get("V2RAY_ASSETS_PATH"),
    };
}

QString EnvTools::get(const QString &key) {
    auto sys_envs = m_process_envs;

    return getEnvs(sys_envs, key);
}

SystemEnvs EnvTools::getInfo() { return m_info; }

QString EnvTools::getEnvs(const QProcessEnvironment &envs, const QString &key) {
    QString val;

    if (envs.contains(key))
        val = envs.value(key);

    return val;
}
