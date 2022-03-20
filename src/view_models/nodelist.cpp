#include "nodelist.h"

#include <utility>

using namespace across;
using namespace across::core;
using namespace across::config;
using namespace across::setting;
using namespace across::utils;
using namespace across::network;

NodeList::NodeList(QObject *parent) : QObject(parent) {}

NodeList::~NodeList() {
    while (!m_tasks.isEmpty())
        m_tasks.dequeue().cancel();
}

void NodeList::init(QSharedPointer<across::setting::ConfigTools> config,
                    QSharedPointer<CoreTools> core,
                    QSharedPointer<across::acolorsapi::AColoRSAPITools> acolors,
                    QSharedPointer<across::NotificationModel> notifications,
                    const QSharedPointer<QSystemTrayIcon> &tray) {
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("nodes");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    p_notifications = std::move(notifications);
    p_acolors = std::move(acolors);
    p_config = std::move(config);
    p_core = std::move(core);

    auto pair_result = p_acolors->core()->currentNode();

    if (pair_result.second.ok()) {
        m_node = pair_result.first;
    }

    if (tray != nullptr) {
        p_tray = tray;
    }

    connect(p_acolors->notifications(),
            &acolorsapi::AColoRSNotifications::stateChanged, this,
            &NodeList::reloadItems);

    connect(p_config.get(), &ConfigTools::apiEnableChanged, this, [&]() {
        if (!p_config->apiEnable() && p_api != nullptr)
            p_api->stopMonitoring();
        else {
            if (p_core->isRunning() && p_api != nullptr) {
                p_api->restartMonitoring();
                m_traffic_last.clear();
                m_traffic_sum.clear();
            }
        }
    });

    connect(p_config.get(), &ConfigTools::apiPortChanged, this, [&]() {
        if (p_api != nullptr) {
            p_api->stopMonitoring();
            p_api.reset(new APITools(p_config->apiPort().toUInt()));
            m_traffic_last.clear();
            m_traffic_sum.clear();
        }
    });

    connect(p_core.get(), &CoreTools::isRunningChanged, this, [&]() {
        if (p_api != nullptr) {
            if (p_core->isRunning()) {
                p_api->startMonitoring("PROXY");
            } else {
                p_api->stopMonitoring();
                m_traffic_last.clear();
                m_traffic_sum.clear();
            }
        }
    });

    connect(this, &NodeList::itemLatencyChanged, this,
            &NodeList::handleLatencyChanged);

    if (p_config->apiEnable()) {
        p_api = QSharedPointer<APITools>::create(p_config->apiPort().toUInt());

        connect(p_api.get(), &APITools::trafficChanged, this,
                [this](const QVariant &data) {
                    auto traffic = data.value<TrafficInfo>();

                    m_traffic_sum.download += traffic.download -
                                              m_traffic_last.download -
                                              m_traffic_last_rate.download;

                    m_traffic_sum.upload += traffic.upload -
                                            m_traffic_last.upload -
                                            m_traffic_last_rate.upload;

                    setDownloadTraffic(std::max(m_traffic_sum.download, 0LL));
                    setUploadTraffic(std::max(m_traffic_sum.upload, 0LL));

                    m_traffic_last_rate = {
                        .upload = traffic.upload - m_traffic_last.upload,
                        .download = traffic.download - m_traffic_last.download,
                    };

                    m_traffic_last = traffic;
                });
    }

    reloadItems();
}

bool NodeList::run() {
    bool res = false;
    do {
        if (p_config == nullptr) {
            p_logger->error("Failed to load config");
            break;
        }

        if (p_core == nullptr) {
            p_logger->error("Failed to load core tools");
        }

        p_core->setConfigByNodeID(m_node.id);
        if (p_core->run() < 0) {
            p_logger->error("Failed to start core process");
            res = false;
        } else {
            res = true;
        }
    } while (false);

    return res;
}

void NodeList::setFilter(const QMap<qint64, QList<qint64>> &search_results) {
    if (search_results.isEmpty())
        return;

    m_search_results = search_results;
    setDisplayGroupID(m_search_results.firstKey());
}

void NodeList::clearFilter() {
    m_search_results.clear();

    emit preItemsReset();
    m_nodes = m_origin_nodes;
    emit postItemsReset();
}

void NodeList::clearItems() {
    emit preItemsReset();
    m_nodes.clear();
    emit postItemsReset();
}

QList<NodeInfo> NodeList::items() { return m_nodes; }

void NodeList::reloadItems() {
    emit preItemsReset();
    m_nodes = p_acolors->profile()->listAllNodes(displayGroupID()).first;
    m_origin_nodes = m_nodes;

    if (!m_search_results.isEmpty() &&
        m_search_results.contains(m_display_group_id)) {
        QList<NodeInfo> temp_nodes;
        auto nodes_id = m_search_results.value(m_display_group_id);
        auto iter = m_origin_nodes.begin();
        for (auto &node_id : nodes_id) {
            for (; iter != m_origin_nodes.end(); ++iter) {
                if (iter->id == node_id) {
                    temp_nodes.append(*iter);
                    ++iter;
                    break;
                }
            }
        }

        m_nodes = temp_nodes;
    }
    emit postItemsReset();
}

QString NodeList::generateConfig() {
    v2ray::config::V2RayConfig node_config;

    p_config->setLogObject(node_config);
    p_config->setAPIObject(node_config);
    p_config->setInboundObject(node_config);

    QString json_str;
    if (!m_node.url.contains("://")) {
        json_str = QString::fromStdString(
            across::SerializeTools::ConfigToJson(node_config, m_node.raw));
    } else {
        auto outbound = node_config.add_outbounds();
        auto temp_outbound =
            across::SerializeTools::JsonToOutbound(m_node.raw.toStdString());

        outbound->CopyFrom(temp_outbound);

        if (outbound->tag().empty()) {
            outbound->set_tag("PROXY");
        }

        json_str = QString::fromStdString(
            across::SerializeTools::ConfigToJson(node_config));
    }

    if (p_config->enableAutoExport()) {
        QDir dir = p_config->dataDir();

        QFile file(dir.filePath(p_config->CURRENT_CONFIG));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(json_str.toUtf8());
            file.close();
        }
    }

    return json_str;
}

void NodeList::appendNode(NodeInfo node) {
    node.group_id = displayGroupID();
    node.group_name =
        p_acolors->profile()->getGroupById(node.group_id).first.name;
    node.routing_id = 0;
    node.routing_name = "default_routings";

    if (auto status =
            p_acolors->profile()->appendNode(node.group_id, node).second;
        !status.ok()) {
        p_logger->error("Failed to add node: {}", node.name.toStdString());
    } else {
        reloadItems();
        emit groupSizeChanged(node.group_id, m_nodes.size());
    }
}

void NodeList::updateNode(NodeInfo node) {
    node.modified_time = QDateTime::currentDateTime();

    if (auto status = p_acolors->profile()->setNodeById(node.id, node);
        !status.ok()) {
        p_logger->error("Failed to update node info: {}",
                        node.name.toStdString());
    } else {
        reloadItems();
    }
}

void NodeList::removeNodeByID(int id) {
    for (const auto &node : m_nodes) {
        if (id == node.id) {
            auto group_id = node.group_id;
            if (auto result = p_acolors->profile()->removeNodeById(id);
                !result.ok()) {
                p_logger->error("Failed to remove node: {}", id);
            } else {
                reloadItems();
            }
            emit groupSizeChanged(group_id, m_nodes.size());
            break;
        }
    }
}

QVariantMap NodeList::getNodeInfoByIndex(int index) {
    if (index >= m_nodes.size())
        return {};

    auto node = m_nodes.at(index);

    return node.toVariantMap();
}

QString NodeList::getQRCode(int node_id, int group_id) {
    auto nodes = p_acolors->profile()->listAllNodes(group_id).first;
    for (auto &node : nodes) {
        if (node.id == node_id) {
            emit updateQRCode(node.name, node.url);
            return node.name;
        }
    }

    return "";
}

qint64 NodeList::currentNodeID() const { return m_node.id; }

qint64 NodeList::currentGroupID() const { return m_node.group_id; }

qint64 NodeList::displayGroupID() const { return m_display_group_id; }

Q_INVOKABLE qint64 NodeList::getIndexByNode(qint64 node_id, qint64 group_id) {
    auto nodes = p_acolors->profile()->listAllNodes(group_id).first;
    for (qint64 index = 0; index < nodes.size(); index++) {
        if (node_id == nodes.at(index).id) {
            return index;
        }
    }
    return -1;
}

QString NodeList::jsonFormat(const QString &json_str) {
    if (json_str.isEmpty())
        return "";

    auto json = Json::parse(json_str.toStdString(), nullptr, false, false);
    if (json == Json::value_t::null)
        return json_str;

    std::string &&res = json.dump(2);

    return QString::fromStdString(res);
}

void NodeList::copyURLToClipboard(const QString &node_name,
                                  const QString &node_url) {
    auto message = QString(tr("Copy [%1] URL to clipboard")).arg(node_name);

    if (p_tray != nullptr) {
        NotifyTools::send(node_url, message, p_tray);
    } else {
        NotifyTools::send(node_url, message);
    }

    ClipboardTools::send(node_url);
}

void NodeList::copyCurrentNodeURLToClipboard() {
    if (m_node.name.isEmpty())
        return;

    copyURLToClipboard(m_node.name, m_node.url);
}

void NodeList::setDisplayGroupID(int group_id) {
    if (group_id <= 0 || group_id == m_display_group_id) {
        return;
    }
    m_display_group_id = group_id;

    reloadItems();
    emit displayGroupIDChanged();
}

void NodeList::setCurrentNodeByID(int id) {
    p_acolors->core()->setConfigByNodeId(id);
    auto pair_result = p_acolors->core()->currentNode();

    if (pair_result.second.ok()) {
        m_node = pair_result.first;
        emit currentGroupIDChanged();
        emit currentNodeIDChanged();
        emit currentNodeInfoChanged(m_node.toVariantMap());
        emit currentNodeChanged(m_node);
        if (!run()) {
            p_logger->error("Failed to start current node: ID{} ", id);
        }
    }
}

void NodeList::handleLatencyChanged(qint64 group_id, int index,
                                    const NodeInfo &node) {
    auto db_future = QtConcurrent::run([&, node] {
        auto t_node = node;
        p_acolors->profile()->setNodeById(t_node.id, t_node);
    });
    if (group_id == displayGroupID()) {
        if (index < m_nodes.size()) {
            m_nodes[index] = node;
            emit itemReset(index);
        }
    }

    while (!m_tasks.isEmpty() && m_tasks.head().isFinished())
        m_tasks.dequeue();
}

void NodeList::saveQRCodeToFile(int id, const QUrl &url) {
    auto iter = std::find_if(m_nodes.begin(), m_nodes.end(),
                             [&](NodeInfo &item) { return item.id == id; });
    if (iter == m_nodes.end()) {
        p_logger->error("Failed to load node info: {}", id);
        return;
    }

    if (auto filename = url.fileName(); !filename.isEmpty()) {
        if (filename.contains("*")) {
            filename = iter->name;
        }

        if (!filename.contains(".png")) {
            filename.append(".png");
        }

        auto path = QUrl(url.toString(QUrl::RemoveFilename)).toLocalFile();
        auto file_path = path.append(filename);
        auto img = QRCodeTools().write(iter->url);

        if (!img.save(file_path) || img.isNull()) {
            p_logger->error("Failed to save image: {}",
                            file_path.toStdString());
        }
    }
}

void NodeList::setAsDefault(int id) {
    this->p_acolors->core()->setDefaultConfigByNodeId(id);
}

void NodeList::setDocument(QVariant &v) {
    const auto ptr = qvariant_cast<QQuickTextDocument *>(v)->textDocument();

    jsonHighlighter.setTheme(p_config->currentTheme());
    jsonHighlighter.init();
    jsonHighlighter.setDocument(ptr);
}

Q_INVOKABLE void NodeList::testLatency(int id) {
    auto iter = std::find_if(m_nodes.begin(), m_nodes.end(),
                             [&](NodeInfo &item) { return item.id == id; });
    if (iter == m_nodes.end()) {
        p_logger->error("Failed to load node info: {}", id);
        return;
    }
    int index = iter - m_nodes.begin();

    if (auto &node = *iter; !node.address.isEmpty()) {
        testLatency(node, index);
    }
}

void NodeList::testLatency(const NodeInfo &node, int index,
                           std::function<void()> after) {
    m_tasks.enqueue(QtConcurrent::run([this, index, node,
                                       after{std::move(after)}] {
        auto current_node = node;
        auto latency = p_acolors->tools()->tcping(
            QString("%1:%2").arg(node.address).arg(node.port).toStdString());
        current_node.latency = latency.second.ok() ? latency.first : -1;

        after();
        emit itemLatencyChanged(node.group_id, index, current_node);
    }));
}

bool NodeList::isRunning() {
    if (p_core != nullptr)
        return p_core->isRunning();

    return false;
}

void NodeList::setDownloadProxy(network::DownloadTask &task) {
    if (isRunning()) {
        if (QUrl proxy = p_config->getInboundProxy(); !proxy.isEmpty()) {
            task.proxy = proxy.toString();
        }
    }
}

QString NodeList::uploadTraffic() const {
    return APITools::unitConvert(m_traffic.upload);
}

void NodeList::setUploadTraffic(double newUploadTraffic) {
    if (m_traffic.upload == newUploadTraffic)
        return;
    m_traffic.upload = newUploadTraffic;
    emit uploadTrafficChanged(APITools::unitConvert(m_traffic.upload));
}

QString NodeList::downloadTraffic() const {
    return APITools::unitConvert(m_traffic.download);
}

void NodeList::setDownloadTraffic(double newDownloadTraffic) {
    if (m_traffic.download == newDownloadTraffic)
        return;
    m_traffic.download = newDownloadTraffic;
    emit downloadTrafficChanged(APITools::unitConvert(m_traffic.download));
}

QVariantMap NodeList::currentNodeInfo() { return m_node.toVariantMap(); }
