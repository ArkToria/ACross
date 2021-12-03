#ifndef RULEMODEL_H
#define RULEMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include <nlohmann/json.hpp>

namespace across {
using Json = nlohmann::json;

enum PolicyType : int {
    Direct = 0,
    Proxy,
    Block,
};

enum RuleType : int {
    IP = 0,
    Domain,
};

struct RuleMeta {
    PolicyType policy;
    RuleType type;
    QString rule;
};

class RuleModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString raw READ raw WRITE setRaw NOTIFY rawChanged)

  public:
    explicit RuleModel(QObject *parent = nullptr);

    enum RuleRoles {
        PolicyRole = Qt::UserRole,
        TypeRole,
        RuleRole,
    };

    Q_ENUM(RuleRoles);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    const QString &raw() const;

    static std::optional<QList<RuleMeta>> parse(const QString &raw);

  public slots:
    void setRaw(const QString &newRaw);

  signals:
    void rawChanged(const QString &raw);

  private:
    QString m_raw;
    QList<RuleMeta> m_rules;
    int m_old_rows;
};
} // namespace across

#endif // RULEMODEL_H
