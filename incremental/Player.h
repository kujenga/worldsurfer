//
//  Player.h
//  worldsurfer
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__Player__
#define __incremental__Player__

#include <stdio.h>

#include "MeshEntity.h"
#include "Bullet.h"

class Player : public MeshEntity {
    
protected:
    
    Material *bulletMat;
    Mesh *bulletMesh;
    
public:
    Player(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        baseRotation = 90.0;
        
        bulletMat = new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/models/balloon.png");
        bulletMesh = new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/models/balloon.obj");
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
        
        return keysUsed;
    }
    
    Bullet* newBullet()
    {
        Bullet *b = new Bullet(bulletMat, bulletMesh, worldSurface);
        b->setWorldPos(worldPos + float3(0.1i,0,0));
        b->setVelocity(velocity.x*1.1);
        return b;
    }
    
    // player is invincible
    virtual void collide(bool myFault) {}
};

#endif /* defined(__incremental__Player__) */
