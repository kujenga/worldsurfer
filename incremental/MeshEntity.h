//
//  MeshEntity.h
//  incremental
//
//  Created by Aaron Taylor on 12/8/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_MeshEntity_h
#define incremental_MeshEntity_h

#include "Entity.h"
#include "Mesh.h"

#define PLAYER_ACCELERATION 0.3
#define NPC_ACCELERATION 0.2

class MeshEntity : public Entity {
protected:
    Mesh *objMesh;
public:
    MeshEntity(Material* material, Mesh *mesh, MobiusStrip *surface):Entity(material, surface), objMesh(mesh) {}
    
    virtual void drawModel() {
        Entity::drawModel();
        objMesh->draw();
    }
};

class Player : public MeshEntity {
    
public:
    Player(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        baseRotation = 90.0;
    }
    
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects) {
        bool keysUsed = false;
        // forward and back movement
        if (keysPressed.at('i')) {
            acceleration.x = PLAYER_ACCELERATION;
            keysUsed = true;
        } else if (keysPressed.at('k')) {
            acceleration.x = -PLAYER_ACCELERATION;
            keysUsed = true;
        } else {
            acceleration.x = 0;
        }
        // left and right movement
        if (keysPressed.at('j')) {
            acceleration.y = PLAYER_ACCELERATION*50;
            keysUsed = true;
        } else if (keysPressed.at('l')) {
            acceleration.y = -PLAYER_ACCELERATION*50;
            keysUsed = true;
        } else {
            acceleration.y = 0;
        }
        // hop movement
        if (keysPressed.at(' ')) {
            acceleration.z = 1;
            keysUsed = true;
        } else {
            acceleration.z = 0;
        }
        return keysUsed;
    }
};

class Wheel : public MeshEntity {
    
public:
    Wheel(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        scale(float3(5,5,5));
    }
    
    void drawModel() {
        glMatrixMode(GL_MODELVIEW);
//        glTranslatef(position.x, position.y, position.z);
        objMesh->draw();
//        glTranslatef(-position.x, -position.y, -position.z);
    }
    
    Entity* setBaseOffset(float3 pos) { baseOffset = pos; return this; }
    
    void spin()
    {
        // needs implementation
    }
};

class Racer : public MeshEntity {
    Mesh *wheelMesh;
public:
    Racer(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        acceleration.x = NPC_ACCELERATION;
        
        wheelMesh = new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/chevy/wheel.obj");
        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(2,4,0)));
        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(2,-4,0)));
        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(-2,4,0)));
        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(-2,-4,0)));
    }
    ~Racer()
    {
        
        delete wheelMesh;
    }
    void setStartDist(float dist) { worldPos.x = dist; }
    
    virtual void drawModel() {
//        MeshEntity::drawModel();
        for (int i = 0; i < subEntities.size(); i++) {
            subEntities.at(i)->drawModel();
        }
    }
};

#endif
