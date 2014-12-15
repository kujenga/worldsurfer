//
//  Bullet.h
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__Bullet__
#define __incremental__Bullet__

#include <stdio.h>

#include "MeshEntity.h"

class Bullet : public MeshEntity {
    
public:
    Bullet(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        acceleration.x = 3*NPC_ACCELERATION;
        worldPos.y = 10;
    }
    
    void setVelocity(float v) { velocity.x = v; }
    
    // bullet is invincible
    virtual void collide(bool myFault) {}
};

#endif /* defined(__incremental__Bullet__) */
