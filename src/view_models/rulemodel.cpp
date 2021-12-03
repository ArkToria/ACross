#include "rulemodel.h"

using namespace across;

RuleModel::RuleModel(QObject *parent) : QAbstractListModel(parent) {}

int RuleModel::rowCount(const QModelIndex &parent) const {
    return m_rules.size();
}

QVariant RuleModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || m_rules.isEmpty() || index.row() >= rowCount())
        return QVariant();

    const auto item = m_rules.at(index.row());

    switch (role) {
    case PolicyRole:
        return item.policy;
    case TypeRole:
        return item.type;
    case RuleRole:
        return item.rule;
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> RuleModel::roleNames() const {
    const QHash<int, QByteArray> roles = {
        {PolicyRole, "policy"},
        {TypeRole, "type"},
        {RuleRole, "rule"},
    };

    return roles;
}

const QString &RuleModel::raw() const { return m_raw; }

std::optional<QList<RuleMeta>> RuleModel::parse(const QString &raw) {
    Json root;
    QList<RuleMeta> rules;

    try {
        root = Json::parse(raw.toStdString());
    } catch (Json::exception e) {
        qDebug() << e.what();
        return {};
    }

    if (root.contains("rules") && root["rules"].is_array()) {
        for (const auto &item : root["rules"].array()) {
            PolicyType policy;

            if (item.contains("outboundTag") &&
                item["outboundTag"].is_string()) {
                QString tag = QString::fromStdString(
                    item["outboundTag"].get<std::string>());

                if (tag.isEmpty()) {
                    continue;
                }

                if (tag.toLower() == "direct") {
                    policy = PolicyType::Direct;
                }

                if (tag.toLower() == "proxy") {
                    policy = PolicyType::Proxy;
                }

                if (tag.toLower() == "block") {
                    policy = PolicyType::Block;
                }
            }

            if (item.contains("domains") && item["domains"].is_array() &&
                !item["domains"].empty()) {

                RuleMeta meta = {
                    .policy = policy,
                    .type = RuleType::Domain,
                };
            }
        }
    }

    return {};
}

void RuleModel::setRaw(const QString &newRaw) {
    if (m_raw == newRaw)
        return;

    m_raw = newRaw;

    emit beginResetModel();
    if (auto temp = RuleModel::parse(m_raw); temp.has_value()) {
        m_rules = temp.value();
    }
    emit endResetModel();

    emit rawChanged(m_raw);
}
