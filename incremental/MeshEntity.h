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
    MeshEntity(Mesh *mesh, Material* material, MobiusStrip *surface):Entity(material, surface), objMesh(mesh) {}
    
    virtual void drawModel() {
        Entity::drawModel();
        objMesh->draw();
    }
};

#endif
