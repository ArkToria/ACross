#include "envtools.h"

using namespace ::across;

EnvTools::EnvTools(QObject* parent)
{
    m_info = {
        get("ACROSS_CONFIG_PATH"),
        get("V2RAY_CORE_PATH"),
        get("V2RAY_ASSETS_PATH")
    };
}

QString EnvTools::get(const QString& key)
{
    auto sys_envs = m_process_envs.systemEnvironment();

    return getEnvs(sys_envs, key);
}

SystemEnvs EnvTools::getInfo()
{
    return m_info;
}

QString EnvTools::getEnvs(const QProcessEnvironment& envs, const QString& key)
{
    QString val;

    if (envs.contains(key)) {
        val = envs.value(key);
    }

    return val;
}
