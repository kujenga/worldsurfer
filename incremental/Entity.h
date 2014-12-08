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

class Entity : public Object {
protected:
    float3 velocity = float3(0,0,0);
    float3 acceleration = float3(0,0,0);
    float restitution = 0.9;
    float drag = 0.8;
    
    // dimensions are x forward, y left and right, z up
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
    Entity(Material* material, MobiusStrip *surface):Object(material), worldSurface(surface) {}
    
    void drawModel()
    {
        worldSurface->glRotateForObjAtAngle(worldPos.x);
        glutSolidTeapot(0.5f);
    }
    
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects) {
        // forward and back movement
        if (keysPressed.at('i')) {
            acceleration.x = 1;
            return true;
        } else if (keysPressed.at('k')) {
            acceleration.x = -1;
            return true;
        } else {
            acceleration.x = 0;
        }
        // left and right movement
        if (keysPressed.at('j')) {
            acceleration.y = 1;
            return true;
        } else if (keysPressed.at('l')) {
            acceleration.y = -1;
            return true;
        } else {
            acceleration.y = 0;
        }
        // hop movement
        if (keysPressed.at(' ')) {
            acceleration.z = 1;
            return true;
        } else {
            acceleration.z = 0;
        }
        return false;
    }
    
    virtual void move(double t, double dt)
    {
//        velocity += gravity()*dt; // gravitational acceleration
        velocity += acceleration*dt;
        velocity *= pow(drag, dt); // drag
        
        worldPos += velocity*dt; // movement
        if (abs(worldPos.y) > worldSurface->getRadius()) {
            worldPos.y = worldSurface->getRadius() * (worldPos.y > 0 ? 1 : -1);
            velocity.y *= -restitution;
        }
        if (worldPos.z < 0) {
            worldPos.z = 0;
            velocity.z *= -restitution;
        }
        
        setGlobalPos();
    }
};

#endif