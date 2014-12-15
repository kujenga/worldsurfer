//
//  MobiusStrip.h
//  incremental
//
//  Created by Aaron Taylor on 12/8/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_MobiusStrip_h
#define incremental_MobiusStrip_h

#include "Object.h"

class MobiusStrip : public Object {
    float3 origin = float3(0,0,0);
    float radius = 1.0;
    float width = 1.0;
public:
    MobiusStrip(Material* material):Object(material){}
    MobiusStrip(Material* material, float r, float w):Object(material), radius(r), width(2*w) {}
    
    const float getRadius() { return radius; }
    
    ///////////////////////////////
    // World functions for translation of coordinates
    ///////////////////////////////
    
    void fillPointForAngleOffset(float a, float r, float3 &pt);
    // wrapper for fill function that returns a new point
    float3 pointForAngleOffset(float a, float r) { float3 pt; fillPointForAngleOffset(a, r, pt); return pt; }
    
    float3 normalForAngle(float a);
    
    // the forward direction of that point on the strip
    float3 dirVector(float a);
    
    // the rotation amount on the specified strip location
    float worldRotation(float a) { return 90*a/M_PI; }
    // performs an openGL rotation on the current matrix, used by entities to adjust their position to the world surface
    void glRotateForObjAtAngle(float a)
    {
        glRotatef(worldRotation(a), -sinf(a), 0, cosf(a));
    }
    
    //////////////////////////////
    // Drawing functions
    //////////////////////////////

    void drawModel();
    
    virtual void drawShadow(float3 lightDir) {}
};


#endif
