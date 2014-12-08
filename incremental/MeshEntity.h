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

class MeshEntity : public Entity {
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
        // forward and back movement
        if (keysPressed.at('i')) {
            acceleration.x = 1;
            return true;
        } else if (keysPressed.at('k')) {
            acceleration.x = -1;
            return true;
        } else {
            acceleration.x = 0;
        }
        // left and right movement
        if (keysPressed.at('j')) {
            acceleration.y = 1;
            return true;
        } else if (keysPressed.at('l')) {
            acceleration.y = -1;
            return true;
        } else {
            acceleration.y = 0;
        }
        // hop movement
        if (keysPressed.at(' ')) {
            acceleration.z = 1;
            return true;
        } else {
            acceleration.z = 0;
        }
        return false;
    }
};

class Racer : public MeshEntity {
    
public:
    Racer(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        acceleration.x = 0.5;
    }
    void setStartDist(float dist) { worldPos.x = dist; }
};

#endif
