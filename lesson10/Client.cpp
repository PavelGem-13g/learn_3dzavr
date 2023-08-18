//
// Created by Иван Ильин on 06.09.2022.
//

#include "Client.h"

void Client::updatePacket() {
    sf::Packet updatePacket;
    updatePacket << MsgType::ClientUpdate << _player->position().x() << _player->position().y() << _player->position().z();
    _socket.send(updatePacket, _socket.serverId()); 
}

void Client::processInit(sf::Packet &packet) {
    sf::Uint16 targetId;
    double x, y, z;

    while(packet >> targetId >> x >> y >> z){
        if(_spawnPlayerCallBack != nullptr){
            _spawnPlayerCallBack(targetId);
        }

        _players[targetId]->translateToPoint(Vec3D(x, y, z));
    }
}

void Client::processUpdate(sf::Packet &packet) {
    sf::Uint16 id;
    double x, y, z;
    while(packet >> id >> x >> y >> z){
        if(_players.count(id)){
            _players[id]->translateToPoint(Vec3D(x, y, z));
        }
    }
}

void Client::processNewClient(sf::Packet &packet) {
    sf::Uint16 id;
    packet >> id;
    
    if(_spawnPlayerCallBack != nullptr){
        _spawnPlayerCallBack(id);
    }
}

void Client::processDisconnect(sf::Uint16 targetId) {
    if(_players.count(targetId) && targetId != _socket.ownId()){
        _players.erase(targetId);
        if(_removePlayerCallBack != nullptr){
            _removePlayerCallBack(targetId);
        }
    }
}

void Client::addPlayer(sf::Uint16 id, std::shared_ptr<RigidBody> player) {
    _players.insert({id, player});
} 

void Client::removePlayer(sf::Uint16 id) {
    _players.erase(id);
}

void Client::setSpawnPlayerCallBack(std::function<void(sf::Uint16)> spawn) {
    _spawnPlayerCallBack = std::move(spawn);
}

void Client::setRemovePlayerCallBack(std::function<void(sf::Uint16)> remove) {
    _removePlayerCallBack = std::move(remove);
}