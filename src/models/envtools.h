#ifndef ENVTOOLS_H
#define ENVTOOLS_H

#include <QObject>
#include <QProcessEnvironment>

namespace across {
struct SystemEnvs {
    QString ACROSS_CONFIG_PATH;
    QString V2RAY_CORE_PATH;
    QString V2RAY_ASSETS_PATH;

    SystemEnvs()
        : ACROSS_CONFIG_PATH()
        , V2RAY_CORE_PATH()
        , V2RAY_ASSETS_PATH()
    {
    }

    SystemEnvs(QString config_path, QString core_path, QString assets_path)
        : ACROSS_CONFIG_PATH(config_path)
        , V2RAY_CORE_PATH(core_path)
        , V2RAY_ASSETS_PATH(assets_path)
    {
    }
};

class EnvTools
{
public:
  explicit EnvTools();

  QString get(const QString& key);

  SystemEnvs getInfo();

private:
    static QString getEnvs(const QProcessEnvironment& envs, const QString& key);

    QProcessEnvironment m_process_envs;
    SystemEnvs m_info;
};
}

#endif
