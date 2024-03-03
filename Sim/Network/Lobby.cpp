#include <chrono>
#include <iostream>
#include <sstream>
#include <imgui.h>
#include <cpr/cpr.h>
#include <SFML/Network.hpp>
#include "../Time.h"
#include "Lobby.h"

using json = nlohmann::json;


Lobby::Lobby(): stopSync(false), lobbySyncer(nullptr), peerLock(), peers() {
    peers.push_back(DefaultPeer);
}

void Lobby::Render() {
    ImGui::Begin("Lobby", nullptr);

    ImGui::SetWindowPos(ImVec2(300, 10), ImGuiCond_Once);
    ImGui::SetWindowSize(ImVec2(280, 200), ImGuiCond_Once);
    ImGui::SetCursorPos(ImVec2(5, 20));

    if (ImGui::Button("Begin server sync")) {
        if (lobbySyncer == nullptr) {
            lobbySyncer = new std::thread(&Lobby::SyncLobbyDetails, this);
        }
    }

    peerLock.lock();
    for (Peer& peer : peers) {
        ImGui::Text("  %s @ %s (%d)", peer.name.c_str(), peer.ip.c_str(), peer.udpPort);
    }
    peerLock.unlock();

    std::stringstream time;
    time << Time::GlobalTime->RunTime();
    ImGui::Text(time.str().c_str());
    ImGui::End();
}

void Lobby::SyncLobbyDetails() {
    // Get a local UDP port to receive from
    sf::UdpSocket socket;

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        std::cout << "Unable to bind to *any* UDP port. Catastrophic failure!" << std::endl;
        return;
    }

    unsigned short port = socket.getLocalPort();
    if (port == 0) {
        std::cout << "Expected to be bound to a UDP port, not 0 (no port). Catastrophic failure!" << std::endl;
        return;
    }

    std::stringstream client;
    client << "{ \"receivePort\": \"" << port << "\" }";
    while (!stopSync) {
        bool foundPeersList = false;

        // Mark ourselves as active in the lobby + check the server for active users
        cpr::Response response = cpr::Post(cpr::Url("https://helium24.net/SimServer"),
            cpr::Body(client.str().c_str()),
            cpr::Header{ {"Content-Type", "application/json"} });

        if (response.status_code == 200) {
            // Success, parse out the peers list from the response message
            try {
                ServerPeerResponse serverPeers = json::parse(response.text).template get<ServerPeerResponse>();
                
                peerLock.lock();
                peers = std::move(serverPeers.peers);
                peerLock.unlock();
            } catch (json::parse_error& ex) {
                std::cout << "  Lobby server did not return valid JSON. Error at: " << ex.byte << ": " << response.text << std::endl;
            }

        } else {
            std::cout << "Unable to communicate to the lobby server!" << std::endl;
            std::cout << "  " << response.status_code << std::endl;
            std::cout << "  " << response.text << std::endl;
        }

        if (!foundPeersList) {
            peerLock.lock();
            peers.clear();
            peers.push_back(DefaultPeer);
            peerLock.unlock();
        }

        // Check periodically
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

Lobby::~Lobby() {
    stopSync = true;
    if (lobbySyncer != nullptr) {
        lobbySyncer->join();
        delete lobbySyncer;
    }
}