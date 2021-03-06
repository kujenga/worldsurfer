//
//  Object.h
//  worldsurfer
//
//  Created by Aaron Taylor on 12/4/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__Object__
#define __incremental__Object__

#include "float4.h"
#include "Mesh.h"
#include "Material.h"

// used to generate random points
#define ARC4RANDOM_MAX 0x100000000
#define LITTLE_RAND ((double)arc4random() / ARC4RANDOM_MAX)

#define DRAG_COEFF 0.6
#define RESTITUTION 0.9

const float3 VERT_GRAVITY = float3(0,-10, 0);

// Object abstract base class.
class Object
{
protected:
    Material* material;
    float3 scaleFactor;
    float3 position;
    float3 orientationAxis;
    float orientationAngle;
public:
    Object(Material* material):material(material),position(0, 0, 0),orientationAngle(0.0f),scaleFactor(1.0,1.0,1.0),orientationAxis(0.0,1.0,0.0){}
    virtual ~Object(){}
    Object* translate(float3 offset){
        position += offset; return this;
    }
    Object* scale(float3 factor){
        scaleFactor *= factor; return this;
    }
    Object* rotate(float angle){
        orientationAngle += angle; return this;
    }
    Object*rotate(float3 axis, float angle) {
        orientationAxis = axis;
        orientationAngle = angle;
        return this;
    }
    const float3 getPosition() { return position; }
    virtual void draw();
    
    virtual void drawModel()=0;
    virtual void move(double t, double dt) {}
    virtual void collide(bool myFault) {}
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects)
    {
        return false;
    }
    virtual void drawShadow(float3 lightDir);
};

#endif /* defined(__incremental__Object__) */
