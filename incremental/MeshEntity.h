//
//  MeshEntity.h
//  worldsurfer
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
    
    virtual void drawModel();
};

#endif
