//
// Created by Иван Ильин on 06.09.2022.
//

#include "Server.h"

void Server::broadcast() {
    sf::Packet updatePacket;
    updatePacket << MsgType::ServerUpdate;

    for(const auto&[playerId, player] : _players){
        updatePacket << playerId << player->position().x() << player->position().y() << player->position().z();
    }

    for(const auto&[playerId, player] : _players){
        _socket.send(updatePacket, playerId);
    }
}

void Server::processConnect(sf::Uint16 senderId) {
        sf::Packet packetToNewClient;
    sf::Packet packetToOldClients;

    packetToNewClient << MsgType::Init << senderId;
    packetToOldClients << MsgType::NewClient << senderId;

    _players.insert({senderId, std::make_shared<Object>(ObjectNameTag("Player_" + std::to_string(senderId)))});

    for(const auto&[playerId, player] : _players){
        packetToOldClients << playerId << player->position().x() << player->position().y() << player->position().z();
        if(playerId != senderId){
            _socket.sendRely(packetToOldClients, playerId);
        }
    }
    _socket.sendRely(packetToNewClient, senderId);

}

void Server::processClientUpdate(sf::Uint16 senderId, sf::Packet &packet) {
    double x, y, z;
    packet >> x >> y >> z;

    _players.at(senderId)->translateToPoint(Vec3D(x, y, z));
}

void Server::processDisconnect(sf::Uint16 senderId) {
       sf::Packet deleteClient;

    deleteClient << MsgType::Disconnect << senderId;
    _players.erase(senderId);

    for(const auto&[playerId, player] : _players){
        _socket.sendRely(deleteClient, playerId);
    }
}
