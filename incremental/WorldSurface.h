//
//  WorldSurface.h
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__WorldSurface__
#define __incremental__WorldSurface__

#include <stdio.h>

#include "Object.h"

class WorldSurface : public Object {
    
public:
    WorldSurface(Material* material):Object(material){}
    
    virtual void fillPointForAngleOffset(float a, float r, float3 &pt)=0;
    // wrapper for fill function that returns a new point
    virtual float3 pointForAngleOffset(float a, float r) { float3 pt; fillPointForAngleOffset(a, r, pt); return pt; }
    
    // the rotation amount on the specified strip location
    virtual float worldRotation(float a)=0;
    
    // performs an openGL rotation on the current matrix, used by entities to adjust their position to the world surface
    virtual void glRotateForObjAtAngle(float a)=0;
};

#endif /* defined(__incremental__WorldSurface__) */
