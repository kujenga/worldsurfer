//
//  Racer.h
//  incremental
//
//  Created by Aaron Taylor on 12/8/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_Racer_h
#define incremental_Racer_h

#include "Object.h"

class Racer : public Object {
protected:
    float3 velocity = float3(1,0,0);
    float acceleration = 0;
    float restitution = 0.9;
    float drag = 0.8;
    
    // dimensions are x forward, y left and right, z forward
    float3 worldPos = float3(0,0,1);
    MobiusStrip *worldSurface;
    
    float3 gravity()
    {
        return worldSurface->normalForAngle(worldPos.x);
    }
    
    void setGlobalPos()
    {
        worldSurface->fillPointForAngleOffset(worldPos.x, worldPos.y, position);
        orientationAxis = worldSurface->normalForAngle(worldPos.x);
        orientationAngle = -180*worldPos.x/M_PI;
    }
public:
    Racer(Material* material, MobiusStrip *surface):Object(material), worldSurface(surface) {}
    
    void drawModel()
    {
        worldSurface->glRotateForObjAtAngle(worldPos.x);
        glutSolidTeapot(1.0f);
    }
    
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects) {
        if (keysPressed.at('[')) {
            acceleration = 1;
        } else if (keysPressed.at('[')) {
            acceleration = -1;
        } else {
            acceleration = 0;
        }
        return false;
    }
    
    virtual void move(double t, double dt)
    {
        //        velocity += gravity()*dt; // gravitational acceleration
        velocity.x += acceleration*dt;
        velocity *= pow(drag, dt); // drag
        
        worldPos.x += velocity.x*dt; // movement
        if (worldPos.y < 0) {
            worldPos.y = 0;
            velocity.y *= -restitution;
        }
        
        setGlobalPos();
    }
};

#endif
