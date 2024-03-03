#include "Peer.h"

Peer::Peer() { }

Peer::Peer(std::string name, std::string ip, long udpPort) {
    this->name = name;
    this->ip = ip;
    this->udpPort = udpPort;
}

void from_json(const json& j, Peer& p) {
    j.at("name").get_to(p.name);
    j.at("ip").get_to(p.ip);
    j.at("udpPort").get_to(p.udpPort);
}

void from_json(const json& j, ServerPeerResponse& s) {
    j.at("peers").get_to(s.peers);
}

Peer DefaultPeer = Peer("No peers!", "127.0.0.1", 0);