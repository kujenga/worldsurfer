//
//  Racer.h
//  worldsurfer
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__Racer__
#define __incremental__Racer__

#include <stdio.h>

#include "MeshEntity.h"

class Racer : public MeshEntity {
    Mesh *wheelMesh;
public:
    Racer(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        acceleration.x = NPC_ACCELERATION;
        
        worldPos.y = LITTLE_RAND*40 - 20;
        
        wheelMesh = new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/chevy/wheel.obj");
        //        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(2,4,0)));
        //        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(2,-4,0)));
        //        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(-2,4,0)));
        //        subEntities.push_back((Entity*)(new Wheel(material, wheelMesh, surface))->translate(float3(-2,-4,0)));
    }
    ~Racer()
    {
        delete wheelMesh;
    }
    void setStartDist(float dist) { worldPos.x = dist; }
    
    virtual void drawModel() {
        MeshEntity::drawModel();
        for (int i = 0; i < subEntities.size(); i++) {
            subEntities.at(i)->drawModel();
        }
    }
};


#endif /* defined(__incremental__Racer__) */
