#ifndef ENVTOOLS_H
#define ENVTOOLS_H

#include <QObject>
#include <QProcessEnvironment>

namespace across {
struct SystemEnvs {
    QString ACOLORS_PATH;
    QString ACROSS_CONFIG_PATH;
    QString ACROSS_DATA_DIR;
    QString V2RAY_CORE_PATH;
    QString V2RAY_ASSETS_PATH;
};

class EnvTools {
  public:
    explicit EnvTools();

    QString get(const QString &key);

    SystemEnvs getInfo();

  private:
    static QString getEnvs(const QProcessEnvironment &envs, const QString &key);

    QProcessEnvironment m_process_envs;
    SystemEnvs m_info;
};
} // namespace across

#endif
