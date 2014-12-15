//
//  MeshEntity.cpp
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#include "MeshEntity.h"

void MeshEntity::drawModel()
{
    Entity::drawModel();
    objMesh->draw();
}