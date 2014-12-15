//
//  MeshInstance.h
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__MeshInstance__
#define __incremental__MeshInstance__

#include <stdio.h>
#include "Object.h"

class MeshInstance : public Object
{
protected:
    Mesh *objMesh;
public:
    MeshInstance(Material* material, Mesh* mesh):Object(material)
    {
        objMesh = mesh;
    }
    virtual void drawModel()
    {
        objMesh->draw();
    }
};

// bouncing object with movement controls
class Bouncer : public MeshInstance
{
protected:
    float3 velocity = float3(0,0,0);
    float3 acceleration = float3(0,0,0);
    float angularVelocity = 0;
public:
    Bouncer(Material* material, Mesh* mesh) : MeshInstance(material, mesh) {}
    
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects)
    {
        bool didControl = false;
        if (keysPressed.at('h')) {
            angularVelocity = 100;
            didControl = true;
        }
        if (keysPressed.at('k')) {
            angularVelocity = -100;
            didControl = true;
        }
        if (keysPressed.at('u')) {
            acceleration = float3(-cos(orientationAngle/365)*10, -10, sin(orientationAngle/365) *10);
        } else if (keysPressed.at('j')) {
            acceleration = float3(cos(orientationAngle/365)*10, -10, -sin(orientationAngle/365)*10);
        } else {
            acceleration = float3(0,0,0);
        }
        return didControl;
    }
    
    virtual void move(double t, double dt)
    {
        rotate(angularVelocity*dt);
        angularVelocity *= pow(DRAG_COEFF, dt);
        
        velocity += VERT_GRAVITY*dt; // gravitational acceleration
        velocity += acceleration*dt;
        velocity *= pow(DRAG_COEFF, dt); // drag
        
        position += velocity*dt; // movement
        if (position.y < 0) {
            position.y = 0;
            velocity.y *= -RESTITUTION;
        }
    }
    
};

#endif /* defined(__incremental__MeshInstance__) */
