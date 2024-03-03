#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Everything we need to know about other friends to talk to them
struct Peer {
	std::string name;
	std::string ip;
	long udpPort;

	Peer();
	Peer(std::string name, std::string ip, long udpPort);
};

extern Peer DefaultPeer;

struct ServerPeerResponse {
	std::vector<Peer> peers;
};

void from_json(const json& j, Peer& p);
void from_json(const json& j, ServerPeerResponse& s);
