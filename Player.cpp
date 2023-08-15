//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "engine/utils/Log.h"

Player::Player() : RigidBody(ObjectNameTag("Player")) {
    //setAcceleration(Vec3D(0, -MinecraftConsts::GRAVITY, 0));
    setCollision(true);
    setColor({240, 168, 168});
    setVisible(true);
    scale(Vec3D(1.2, 1.9, 1.2)); 
    //recalculateHitBox();
}

void Player::nextBlock() {
    _selectedBlock = static_cast<Cube::Type>(((int)_selectedBlock + 1) % (int) Cube::Type::none); 
}

void Player::previousBlock() {
    if((int)_selectedBlock >0){
        _selectedBlock = static_cast<Cube::Type>((int)_selectedBlock - 1);
    }
   _selectedBlock = static_cast<Cube::Type>(Cube::Type::none - 1); 
}

void Player::collisionCallBack(const CollisionInfo &info) {
    // setting vertical velocity to zero after collision

    Vec3D velocity_parallel = info.normal * velocity().dot(info.normal);
    Vec3D velocity_perpendicular = velocity() - velocity_parallel;

    if (velocity().dot(info.normal) > 0) {
        setVelocity(velocity_perpendicular);
    }
}
