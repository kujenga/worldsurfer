//
//  Entity.cpp
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#include "Entity.h"

void Entity::drawModel()
{
    if (alive) {
        worldSurface->glRotateForObjAtAngle(worldPos.x);
        for (int i = 0; i < subEntities.size(); i++) {
            subEntities.at(i)->drawModel();
        }
    }
}

void Entity::move(double t, double dt)
{
    if (!alive) {
        return;
    }
    //        velocity += gravity()*dt; // gravitational acceleration
    velocity += acceleration*dt;
    velocity *= pow(drag, dt); // drag
    
    worldPos += velocity*dt; // movement
    
    // bouncing off the side
    if (abs(worldPos.y) > worldSurface->getRadius()*M_PI*0.24) {
        worldPos.y = worldSurface->getRadius() * (worldPos.y > 0 ? M_PI*0.24 : -M_PI*0.24);
        velocity.y *= -0.5;
        velocity *= restitution;
    }
    // bouncing off the ground
    if (worldPos.z < 0) {
        worldPos.z = 0;
        velocity.z *= -restitution;
    }
    
    setGlobalPos();
}