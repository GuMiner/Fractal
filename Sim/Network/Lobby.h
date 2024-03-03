#pragma once
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "Peer.h"

class Lobby {
	bool stopSync;
	std::thread* lobbySyncer;

	std::mutex peerLock;
	std::vector<Peer> peers;
public:
	Lobby();
	~Lobby();

	void Render();
	void SyncLobbyDetails();
};

