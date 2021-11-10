#include "nodelist.h"

using namespace across;
using namespace across::core;
using namespace across::config;
using namespace across::setting;
using namespace across::utils;
using namespace across::network;

NodeList::NodeList(QObject *parent) : QObject(parent) {}

NodeList::~NodeList() {
    while (!work_tasks.isEmpty())
        work_tasks.dequeue().waitForFinished();
}

void NodeList::init(QSharedPointer<across::setting::ConfigTools> config,
                    QSharedPointer<CoreTools> core,
                    QSharedPointer<DBTools> db
#ifdef __MINGW32__
                    ,QSharedPointer<QSystemTrayIcon> tray) {
#else
                    ) {
#endif
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("nodes");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    p_db = db;

    p_config = config;

    p_core = core;

#ifdef __MINGW32__
    p_tray = tray;
#endif

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

    if (p_config->enableAutoConnect()) {
        NodeInfo node;
        if (auto id = p_db->getCurrentNodeID(); id) {
            auto nodes =
                p_db->listAllNodesFromGroupID(p_db->getCurrentGroupID());
            for (auto &nodeItem : nodes) {
                if (id == nodeItem.id) {
                    node = nodeItem;
                }
            }
        }
        if (auto id = p_db->getDefaultNodeID(); id) {
            auto nodes =
                p_db->listAllNodesFromGroupID(p_db->getDefaultGroupID());
            for (auto &nodeItem : nodes) {
                if (id == nodeItem.id) {
                    node = nodeItem;
                }
            }
        }
        m_node = node;
        if (!run()) {
            p_logger->error("Failed to start current node: {} {}", node.id,
                            node.name.toStdString());
        }
    }

    reloadItems();
}

bool NodeList::run() {
    bool res = false;
    do {
        if (m_node.raw.isEmpty()) {
            p_logger->error("Failed to load current node");
            break;
        }

        if (p_config == nullptr) {
            p_logger->error("Failed to load config");
            break;
        }

        if (p_core == nullptr) {
            p_logger->error("Failed to load core tools");
        };

        p_core->setConfig(generateConfig());
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
    m_nodes = p_db->listAllNodesFromGroupID(displayGroupID());
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
    v2ray::config::V2rayConfig config;

    auto log_object = config.mutable_log();
    log_object->set_loglevel(p_config->logLevel().toStdString());

    auto api_object = config.mutable_api();
    api_object->set_tag("ACROSS_API");
    api_object->add_services("LoggerService");
    api_object->add_services("StatsService");

    auto routing = config.mutable_routing();
    auto rule = routing->add_rules();
    rule->set_type("field");
    rule->set_outboundtag("ACROSS_API");
    rule->add_inboundtag("ACROSS_API_INBOUND");

    auto _ = config.mutable_stats();

    auto policy = config.mutable_policy();
    auto system = policy->mutable_system();
    system->set_statsinbounddownlink(true);
    system->set_statsinbounduplink(true);
    system->set_statsoutbounddownlink(true);
    system->set_statsoutbounduplink(true);

    p_config->setInboundObject(config);

    QString json_str;
    if (!m_node.url.contains("://")) {
        json_str = QString::fromStdString(
            across::SerializeTools::ConfigToJson(config, m_node.raw));
    } else {
        auto outbound = config.add_outbounds();
        auto temp_outbound =
            across::SerializeTools::JsonToOutbound(m_node.raw.toStdString());
        outbound->CopyFrom(temp_outbound);
        json_str = QString::fromStdString(
            across::SerializeTools::ConfigToJson(config));
    }

#ifdef QT_DEBUG
    QFile file("generation_test.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(json_str.toUtf8());
        file.close();
    }
#endif

    return json_str;
}

void NodeList::appendNode(NodeInfo node) {
    node.group_id = displayGroupID();
    node.group_name = p_db->getGroupFromID(node.group_id).value().name;

    if (auto err = p_db->insert(node); err.type() != QSqlError::NoError) {
        p_logger->error("Failed to add node: {}", node.name.toStdString());
    } else {
        reloadItems();
        emit groupSizeChanged(node.group_id, m_nodes.size());
    }
}

void NodeList::updateNode(NodeInfo node) {
    node.modified_time = QDateTime::currentDateTime();

    if (auto err = p_db->update(node); err.type() != QSqlError::NoError) {
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
            if (auto result = p_db->removeNodeFromID(id);
                result.type() != QSqlError::NoError) {
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

QString NodeList::getQRCode(int node_id) {
    for (auto &node : m_nodes) {
        if (node.id == node_id) {
            emit updateQRCode(node.name, node.url);
            return node.name;
        }
    }

    return "";
}

qint64 NodeList::currentNodeID() { return m_node.id; }

qint64 NodeList::currentGroupID() { return m_node.group_id; }

qint64 NodeList::displayGroupID() { return m_display_group_id; }

Q_INVOKABLE qint64 NodeList::getIndexByNode(qint64 node_id, qint64 group_id) {
    auto nodes = p_db->listAllNodesFromGroupID(group_id);
    for (qint64 index = 0; index < nodes.size(); index++) {
        if (node_id == nodes.at(index).id) {
            return index;
        }
    }
    return -1;
}

QString NodeList::jsonFormat(const QString &json_str) {
    auto json =
        nlohmann::json::parse(json_str.toStdString(), nullptr, false, false);
    if (json == nlohmann::json::value_t::null) {
        return json_str;
    }
    std::string &&res = json.dump(2);
    QString result = QString::fromStdString(res);
    return result;
}

void NodeList::copyURLToClipboard(const QString &node_name,
                                  const QString &node_url) {
    NotifyTools::send(node_url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(node_name)
#ifdef __MINGW32__
                      , p_tray);
#else
                      );
#endif

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
    for (auto &node : m_nodes) {
        if (id == node.id) {
            m_node = node;

            p_db->updateRuntimeValue(
                RuntimeValue(RunTimeValues::CURRENT_NODE_ID, node.id));
            p_db->updateRuntimeValue(
                RuntimeValue(RunTimeValues::CURRENT_GROUP_ID, node.group_id));

            emit currentGroupIDChanged();
            emit currentNodeIDChanged();
            emit currentNodeInfoChanged(m_node.toVariantMap());

            if (!run()) {
                p_logger->error("Failed to start current node: {} {}", node.id,
                                node.name.toStdString());
            }

            break;
        }
    }
}

void NodeList::handleLatencyChanged(qint64 group_id, int index, NodeInfo node) {
    p_db->update(node);
    if (group_id == displayGroupID()) {
        if (index < m_nodes.size()) {
            m_nodes[index] = node;
            emit itemReset(index);
        }
    }

    while (!work_tasks.isEmpty() && work_tasks.head().isFinished())
        work_tasks.dequeue();
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
    p_db->updateRuntimeValue(RuntimeValue(RunTimeValues::DEFAULT_NODE_ID, id));
    p_db->updateRuntimeValue(
        RuntimeValue(RunTimeValues::DEFAULT_GROUP_ID, displayGroupID()));
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

void NodeList::testLatency(NodeInfo node, int index) {
    work_tasks.enqueue(QtConcurrent::run([this, index, node] {
        auto current_node = node;
        TCPPing ping;
        ping.setAddr(node.address);
        ping.setPort(node.port);
        current_node.latency = ping.getAvgLatency();
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

QString NodeList::uploadTraffic() {
    return APITools::unitConvert(m_traffic.upload);
}

void NodeList::setUploadTraffic(double newUploadTraffic) {
    if (m_traffic.upload == newUploadTraffic)
        return;
    m_traffic.upload = newUploadTraffic;
    emit uploadTrafficChanged(APITools::unitConvert(m_traffic.upload));
}

QString NodeList::downloadTraffic() {
    return APITools::unitConvert(m_traffic.download);
}

void NodeList::setDownloadTraffic(double newDownloadTraffic) {
    if (m_traffic.download == newDownloadTraffic)
        return;
    m_traffic.download = newDownloadTraffic;
    emit downloadTrafficChanged(APITools::unitConvert(m_traffic.download));
}

QVariantMap NodeList::currentNodeInfo() { return m_node.toVariantMap(); }
