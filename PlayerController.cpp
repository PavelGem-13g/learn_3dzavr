//
// Created by Иван Ильин on 19.09.2021.
//

#include "PlayerController.h"
#include "engine/utils/Log.h"
#include "engine/animation/Animations.h"
#include "engine/io/SoundController.h"

PlayerController::PlayerController(std::shared_ptr<Player> player,
                                   std::shared_ptr<Keyboard> keyboard,
                                   std::shared_ptr<Mouse> mouse) : _player(player), _keyboard(keyboard), _mouse(mouse) {
}

void PlayerController::update() {
    auto camera = _player->attached(ObjectNameTag("Camera"));

    // Left and right movement
    if (Keyboard::isKeyPressed(sf::Keyboard::A)){
        _player->translate(_player->left()*Time::deltaTime()*MinecraftConsts::WALK_SPEED);
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::D)){
        _player->translate(-_player->left()*Time::deltaTime()*MinecraftConsts::WALK_SPEED);
    }

    // Forward and backward movement
    if (Keyboard::isKeyPressed(sf::Keyboard::W)){
        _player->translate(_player->lookAt()*Time::deltaTime()*MinecraftConsts::WALK_SPEED);
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::S)){
        _player->translate(-_player->lookAt() *Time::deltaTime()*MinecraftConsts::WALK_SPEED);
    }

    // Jump
        if (Keyboard::isKeyPressed(sf::Keyboard::Space) && _player->inCollision()){
        _player->setVelocity(Vec3D(0, sqrt(2*MinecraftConsts::GRAVITY*MinecraftConsts::JUMP_HEIGHT), 0));
    }

    // Horizontal player rotation
    Vec2D disp = _mouse->getMouseDisplacement();

    _player->rotate(Vec3D(0, -disp.x()*MinecraftConsts::MOUSE_SENSITIVITY, 0));

    // Vertical player rotation
    double rotationLeft = disp.y()*MinecraftConsts::MOUSE_SENSITIVITY;
    camera->rotateLeft(rotationLeft);

    // Change selected block
    if (Keyboard::isKeyPressed(sf::Keyboard::Right)){
        _player->nextBlock();
        _updateCubeInHandCallBack(); 
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::Left)){
        _player->previousBlock();
        _updateCubeInHandCallBack(); 
    }

    // Add or remove block
    // TODO: implement (lesson 3)

    animateCameraMotion();
}

void PlayerController::animateCameraMotion() {
    bool inRunning_old = _inRunning;
    _inRunning = Keyboard::isKeyPressed(sf::Keyboard::A) || Keyboard::isKeyPressed(sf::Keyboard::D) || Keyboard::isKeyPressed(sf::Keyboard::W) || Keyboard::isKeyPressed(sf::Keyboard::S);
    auto camera = _player->attached(ObjectNameTag("Camera"));

    // Camera movement during run
    // TODO: implement (lesson 3)
}

// setting callbacks
void PlayerController::setAddCubeCallBack(std::function<void()> addCube) {
    _addCubeCallBack = std::move(addCube);
}
void PlayerController::setRemoveCubeCallBack(std::function<void()> removeCube) {
    _removeCubeCallBack = std::move(removeCube);
}
void PlayerController::setUpdateCubeInHandCallBack(std::function<void()> updateCube) {
    _updateCubeInHandCallBack = std::move(updateCube);
}
