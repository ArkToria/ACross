#ifndef ACOLORSAPITOOLS_H
#define ACOLORSAPITOOLS_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QVariant>
#include <QtConcurrent>

#include <atomic>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <vector>

#include "acolors.grpc.pb.h"
#include "dbtools.h"

using acolors::NodeData;
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using std::pair;

namespace across::acolorsapi {

class AColoRSNotifications : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSNotifications(const std::shared_ptr<Channel> &channel);

    ~AColoRSNotifications() override;

    void start();

    void stop();

  signals:
    void empty();
    void appendGroup();
    void updateCoreStatus();
    void updateInbounds();
    void coreConfigChanged();
    void coreChanged();
    void removeGroupByID(int32_t group_id);
    void removeNodeByID(int32_t node_id);
    void setGroupByID(int32_t group_id);
    void setNodeByID(int32_t node_id);
    void appendNode(int32_t group_id);
    void updateGroup(int32_t group_id);
    void runtimeValueChanged(std::string key);
    void emptyGroup(int32_t group_id);

  private:
    bool is_running = false;
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::Notifications::Stub> p_stub;
    std::unique_ptr<ClientContext> context;

    QFuture<void> future;
};
class AColoRSProfile : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSProfile(const std::shared_ptr<Channel> &channel);
    ~AColoRSProfile() override;

    pair<uint64_t, Status> countGroups();
    pair<QList<GroupInfo>, Status> listAllGroups();
    pair<uint64_t, Status> countNodes(int32_t group_id);
    pair<QList<NodeInfo>, Status> listAllNodes(int32_t group_id);
    pair<GroupInfo, Status> getGroupById(int32_t group_id);
    pair<NodeInfo, Status> getNodeById(int32_t node_id);
    Status setGroupById(int32_t group_id, const GroupInfo &data);
    Status setGroups(const QList<GroupInfo> &data);
    Status setNodeById(int32_t node_id, const NodeInfo &data);
    Status setNodeByUrl(int32_t node_id, std::string url);
    Status removeGroupById(int32_t group_id);
    Status removeNodeById(int32_t node_id);
    Status appendGroup(const GroupInfo &data);
    Status appendNode(int32_t group_id, const NodeInfo &data);
    Status appendNodes(int32_t group_id, const QList<NodeInfo> &data);
    Status appendNodeByUrl(int32_t group_id, std::string url);
    Status updateGroupById(int32_t group_id);
    Status emptyGroupById(int32_t group_id);

    GroupInfo groupFrom(const acolors::GroupData &data);
    static NodeInfo nodeFrom(const acolors::NodeData &data);
    QList<GroupInfo> groupListFrom(const acolors::GroupList &group_list);
    static QList<NodeInfo> nodeListFrom(const acolors::NodeList &node_list);
    acolors::GroupData groupTo(const GroupInfo &data);
    static acolors::NodeData nodeTo(const NodeInfo &data);

  private:
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::ProfileManager::Stub> p_stub;
};
class AColoRSCore : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSCore(const std::shared_ptr<Channel> &channel,
                         const std::shared_ptr<AColoRSProfile> &profile);
    ~AColoRSCore() override;

    Status run();
    Status stop();
    Status restart();
    pair<bool, Status> isRunning();
    pair<NodeInfo, Status> currentNode();
    Status setConfigByNodeId(int32_t node_id);
    Status setCoreByTag(std::string tag);
    Status setDefaultConfigByNodeId(int32_t node_id);

  private:
    std::shared_ptr<Channel> p_channel;
    std::shared_ptr<AColoRSProfile> p_profile;
    std::unique_ptr<acolors::CoreManager::Stub> p_stub;
};
class AColoRSConfig : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSConfig(const std::shared_ptr<Channel> &channel);
    ~AColoRSConfig() override;

    Status setInbounds(const acolors::Inbounds &inbounds);
    pair<acolors::Inbounds, Status> getInbounds();

  private:
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::ConfigManager::Stub> p_stub;
};
class AColoRSAPITools : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSAPITools(uint port);

    ~AColoRSAPITools() override;

    [[nodiscard]] inline AColoRSNotifications &notifications() const {
        return *this->p_notifications;
    };
    [[nodiscard]] inline AColoRSProfile &profile() const {
        return *this->p_profile;
    };
    [[nodiscard]] inline AColoRSConfig &config() const {
        return *this->p_config;
    };
    [[nodiscard]] inline AColoRSCore &core() const { return *this->p_core; };

  private:
    const std::string LOCAL_HOST = "localhost";

    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<AColoRSNotifications> p_notifications;
    std::shared_ptr<AColoRSProfile> p_profile;
    std::unique_ptr<AColoRSConfig> p_config;
    std::unique_ptr<AColoRSCore> p_core;
};
} // namespace across::acolorsapi

#endif // ACOLORSAPITOOLS_H
