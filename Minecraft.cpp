//
// Created by Иван Ильин on 03.10.2021.
//

#include "Minecraft.h"
#include "engine/utils/ResourceManager.h"

using namespace std;

void Minecraft::start() {
    // This code executed once in the beginning:

    screen->setMouseCursorVisible(false);
    setGlEnable(true);

    // loading map
    map->loadMap(MinecraftConsts::MAP_NAME);

    // setting callbacks for playerController
    playerController->setAddCubeCallBack([this](){ addCube(); });
    playerController->setRemoveCubeCallBack([this](){ return removeCube(); });
    playerController->setUpdateCubeInHandCallBack([this](){ updateCubeInHandColor(); });
    world->addBody(player);

    // orientating and attaching camera
    camera->translateToPoint(player->position() + Vec3D(0, 2.8, 0));
    player->attach(camera);
    player->translate(Vec3D(0, 6, 0 ));

    // adding cube in hand
    auto cube_in_hand = world->addBody(std::make_shared<RigidBody>(Mesh::Cube(ObjectNameTag("cube_in_hand"), 3)));
    cube_in_hand->setCollider(false); 
    cube_in_hand->translateToPoint(player->position() + Vec3D{-3, -1, 1.4});
    cube_in_hand->rotate(Vec3D(0, M_PI/10, 0)); 

    camera->attach(cube_in_hand);
    updateCubeInHandColor();
    
    world->addBody(player);
}

void Minecraft::update() {
    // This code executed every time step:

    // exit the game
    if(keyboard->isKeyTapped(sf::Keyboard::Escape)) {
        exit();
    }

    // switch  OpenGL on/off
    if (keyboard->isKeyTapped(sf::Keyboard::O)) {
        setGlEnable(!glEnable());
    }

    // display debug info
    if (keyboard->isKeyTapped(sf::Keyboard::Tab)) {
        setDebugInfo(!showDebugInfo());
    }

    // save map
    if (keyboard->isKeyTapped(sf::Keyboard::R)) {
        map->saveMap(MinecraftConsts::MAP_NAME);
    }

    playerController->update();
}

void Minecraft::updateCubeInHandColor() {
    world->body(ObjectNameTag("cube_in_hand"))->setColor(Cube::cubeColor(player->selectedBlock()));
}

void Minecraft::gui() {
    auto t = ResourceManager::loadTexture(MinecraftConsts::AIM_TEXTURE);
    if(t != nullptr){
        sf::Sprite sprite;
        sprite.setTexture(*t);
        sprite.setPosition(screen->width()/2 - 27, screen->height()/2-27);
        sprite.setColor(sf::Color(0, 0, 0, 150));
        screen->drawSprite(sprite);
    }
}

void Minecraft::addCube() {
    auto rayCast = world->rayCast(camera->position(), camera->position()+camera->lookAt()*10, "Player");

    if((rayCast.pointOfIntersection-camera->position()).abs() < 50 ){
        Vec3D cubePoint = (rayCast.intersectedTriangle.position() + rayCast.intersectedTriangle.norm()*2);
        
        map->addCube(Vec3D(round(cubePoint.x()/4), round(cubePoint.y()/4), round(cubePoint.z()/4)), player->selectedBlock());
    }
}

void Minecraft::removeCube() {
        auto rayCast = world->rayCast(camera->position(), camera->position()+camera->lookAt()*10, "Player");

    if((rayCast.pointOfIntersection-camera->position()).abs() < 50 ){
        Vec3D cubePoint = (rayCast.intersectedTriangle.position() - rayCast.intersectedTriangle.norm()*2);
        
        map->removeCube (Vec3D(round(cubePoint.x()/4), round(cubePoint.y()/4), round(cubePoint.z()/4)));
    }
}
