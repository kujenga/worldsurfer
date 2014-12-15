//
//  Wheel.h
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__Wheel__
#define __incremental__Wheel__

#include <stdio.h>

#include "MeshEntity.h"

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

#endif /* defined(__incremental__Wheel__) */
