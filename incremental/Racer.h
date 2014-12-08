//
//  Racer.h
//  incremental
//
//  Created by Aaron Taylor on 12/8/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_Racer_h
#define incremental_Racer_h

#include "MeshEntity.h"

class Racer : public MeshEntity {
    float speed = 0.5;
public:
    Racer(Material* material, Mesh *mesh, MobiusStrip *surface) : MeshEntity(material, mesh, surface) {
        //acceleration.x = 0.5;
    }
};

#endif
