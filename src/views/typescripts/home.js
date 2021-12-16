"use strict";
function hideAllTypeSetting(components) {
    for (let component of components) {
        if (component !== null) {
            component.visible = false;
        }
    }
}
function networkSelectSetting(currentIndex, components, control = null) {
    if (components === null) {
        return;
    }
    let hostLabel = components["hostLabel"];
    let hostText = components["hostText"];
    let pathLabel = components["pathLabel"];
    let pathText = components["pathText"];
    let typeLabel = components["typeLabel"];
    let typeSelect = components["typeSelect"];
    let quicSecurityLabel = components["quicSecurityLabel"];
    let quicSecuritySelect = components["quicSecuritySelect"];
    hideAllTypeSetting([hostLabel, hostText, pathLabel, pathText, typeLabel, typeSelect, quicSecurityLabel, quicSecuritySelect]);
    switch (currentIndex) {
        case 0: // none
            break;
        case 1: // tcp
            hostLabel.visible = false;
            hostText.visible = false;
            pathLabel.visible = false;
            pathText.visible = false;
            break;
        case 2: // h2
            hostLabel.visible = true;
            hostLabel.text = "Host";
            hostText.visible = true;
            pathLabel.visible = true;
            pathLabel.text = "Path";
            pathText.visible = true;
            break;
        case 3: // ws
            hostLabel.visible = true;
            hostLabel.text = "Host";
            hostText.visible = true;
            pathLabel.visible = true;
            pathLabel.text = "Path";
            pathText.visible = true;
            break;
        case 4: // grpc
            pathLabel.visible = true;
            pathLabel.text = "Service Name";
            pathText.visible = true;
            break;
        case 5: // quic
            typeLabel.visible = true;
            typeSelect.visible = true;
            quicSecurityLabel.visible = true;
            quicSecuritySelect.visible = true;
            pathLabel.visible = true;
            pathLabel.text = "Key";
            pathText.visible = true;
            break;
        default:
            break;
    }
    if (control !== null) {
        control.configChanged();
    }
}
function vmessComponentSetting(visible, components, model = null) {
    if (!visible) {
        return;
    }
    let alterIDText = components["alterIDText"];
    let securitySelect = components["securitySelect"];
    let networkSelect = components["networkSelect"];
    let pathText = components["pathText"];
    let hostText = components["hostText"];
    let typeSelect = components["typeSelect"];
    let quicSecuritySelect = components["quicSecuritySelect"];
    let tlsEnableSelect = components["tlsEnableSelect"];
    let sniText = components["sniText"];
    if (model !== null && model.raw !== null) {
        let raw = JSON.parse(model.raw);
        if (raw.hasOwnProperty("protocol") && raw["protocol"] !== "vmess") {
            return;
        }
        if (raw.hasOwnProperty("settings")) {
            let vmess = null;
            if (raw["settings"].hasOwnProperty("vmess")) {
                vmess = raw["settings"]["vmess"]["vnext"];
            }
            else {
                vmess = raw["settings"]["vnext"];
            }
            if (vmess !== null && Object.keys(vmess).length > 0) {
                let server = vmess[0];
                if (server.hasOwnProperty("users") && Object.keys(server["users"]).length > 0) {
                    let user = server["users"][0];
                    if (user.hasOwnProperty("alterId") && alterIDText !== null) {
                        alterIDText.text = user["alterId"];
                    }
                    if (user.hasOwnProperty("security") && securitySelect !== null) {
                        securitySelect.currentIndex = securitySelect.find(user["security"]);
                    }
                }
            }
        }
        if (!raw.hasOwnProperty("streamSettings")) {
            return;
        }
        if (raw["streamSettings"] !== null) {
            let streamSettings = raw["streamSettings"];
            if (streamSettings.hasOwnProperty("network") && networkSelect !== null) {
                let network = streamSettings["network"];
                if (network === "http") {
                    network = "h2";
                }
                networkSelect.currentIndex = networkSelect.find(network);
                switch (network) {
                    case "tcp":
                        break;
                    case "h2":
                        if (streamSettings.hasOwnProperty("httpSettings")) {
                            let httpSettings = streamSettings["httpSettings"];
                            if (httpSettings.hasOwnProperty("host") && hostText !== null) {
                                let hosts = "";
                                for (const host of httpSettings["host"]) {
                                    hosts = hosts.concat(host, ",");
                                }
                                hostText.text = hosts.substring(0, hosts.length - 1).trim();
                            }
                            if (httpSettings.hasOwnProperty("path") && pathText !== null) {
                                pathText.text = httpSettings["path"];
                            }
                        }
                        break;
                    case "ws":
                        if (streamSettings.hasOwnProperty("wsSettings")) {
                            let wsSettings = streamSettings["wsSettings"];
                            if (wsSettings.hasOwnProperty("path") && pathText !== null) {
                                pathText.text = wsSettings["path"];
                            }
                            if (wsSettings.hasOwnProperty("headers")
                                && wsSettings["headers"].hasOwnProperty("Host") && hostText !== null) {
                                hostText.text = wsSettings["headers"]["Host"];
                            }
                        }
                        break;
                    case "grpc":
                        if (streamSettings.hasOwnProperty("grpcSettings") && pathText !== null) {
                            let grpcSettings = streamSettings["grpcSettings"];
                            if (grpcSettings.hasOwnProperty("serviceName")) {
                                pathText.text = grpcSettings["serviceName"];
                            }
                        }
                        break;
                    case "quic":
                        if (streamSettings.hasOwnProperty("quicSettings") && typeSelect !== null) {
                            let quicSettings = streamSettings["quicSettings"];
                            if (quicSettings.hasOwnProperty("header")
                                && quicSettings["header"].hasOwnProperty("type")) {
                                typeSelect.currentIndex = typeSelect.find(quicSettings["header"]["type"]);
                            }
                            if (quicSettings.hasOwnProperty("key") && pathText !== null) {
                                pathText.text = quicSettings["key"];
                            }
                            if (quicSettings.hasOwnProperty("security") && quicSecuritySelect !== null) {
                                quicSecuritySelect.currentIndex = quicSecuritySelect.find(quicSettings["security"]);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            if (streamSettings.hasOwnProperty("security") && tlsEnableSelect !== null) {
                if (streamSettings["security"] === "tls") {
                    tlsEnableSelect.checked = true;
                }
            }
            if (streamSettings.hasOwnProperty("tlsSettings") &&
                streamSettings["tlsSettings"].hasOwnProperty("serverName")
                && sniText !== null) {
                sniText.text = streamSettings["tlsSettings"]["serverName"];
            }
        }
    }
}
function trojanComponentSetting(visible, components, model = null) {
    if (!visible) {
        return;
    }
    let securitySelect = components["securitySelect"];
    let networkSelect = components["networkSelect"];
    let serverNameText = components["serverNameText"];
    let alpnSelect = components["alpnSelect"];
    if (model !== null && model.raw !== null) {
        let raw = JSON.parse(model.raw);
        if (raw.hasOwnProperty("protocol")
            && raw["protocol"] !== "trojan") {
            return;
        }
        if (!raw.hasOwnProperty("streamSettings")) {
            return;
        }
        let streamSettings = raw["streamSettings"];
        if (streamSettings.hasOwnProperty("network")) {
            networkSelect.currentIndex = networkSelect.find(streamSettings["network"]);
        }
        if (streamSettings.hasOwnProperty("security")) {
            securitySelect.currentIndex = securitySelect.find(streamSettings["security"]);
        }
        if (streamSettings.hasOwnProperty("tlsSettings")) {
            if (streamSettings["tlsSettings"].hasOwnProperty("serverName")) {
                serverNameText.text = streamSettings["tlsSettings"]["serverName"];
            }
            if (streamSettings["tlsSettings"].hasOwnProperty("alpn")) {
                let alpn = streamSettings["tlsSettings"]["alpn"];
                let alpn_size = Object.keys(alpn).length;
                if (alpn_size > 1) {
                    alpnSelect.currentIndex = 0;
                }
                else if (alpn_size > 0) {
                    alpnSelect.currentIndex = alpnSelect.find(alpn[0]);
                }
            }
        }
    }
}
function shadowsocksComponentSetting(visible, components, model = null) {
    if (!visible) {
        return;
    }
    let securitySelect = components["securitySelect"];
    let ivCheckSelect = components["ivCheckSelect"];
    if (model !== null && model.raw !== null) {
        let raw = JSON.parse(model.raw);
        if (raw.hasOwnProperty("protocol")
            && raw["protocol"] !== "shadowsocks") {
            return;
        }
        let server = null;
        if (raw["settings"].hasOwnProperty("shadowsocks")) {
            server = raw["settings"]["shadowsocks"]["servers"][0];
        }
        else {
            server = raw["settings"]["servers"][0];
        }
        if (server !== null) {
            securitySelect.currentIndex = securitySelect.find(server["method"]);
            if (server.hasOwnProperty("ivCheck")) {
                ivCheckSelect.checked = server["ivCheck"];
            }
        }
    }
}
function displayProtocolText(protocol) {
    switch (protocol) {
        case 0:
            return "vmess";
        case 1:
            return "shadowsocks";
        case 2:
            return "trojan";
        case 4:
            return "raw";
        case 5:
            return "scheme";
        case 6:
        default:
            return "unknown";
    }
}
function isCurrentNode(nodes, groupID, nodeID, core) {
    let result = false;
    do {
        if (nodes === null || core === null)
            break;
        if (nodes.currentGroupID === null || nodes.currentNodeID === null || core.isRunning === null)
            break;
        if (nodes.currentGroupID === groupID
            && nodes.currentNodeID === Number(nodeID)
            && core.isRunning) {
            result = true;
        }
    } while (false);
    return result;
}
function getQRCode(nodes, model, config) {
    let name = nodes.getQRCode(model.nodeID, model.groupID);
    if (name === "") {
        return "qrc:/misc/icons/" + config.iconStyle + "/repo_qr_code.svg";
    }
    else {
        return "image://acrossImageProvider/" + name;
    }
}
