//
//  Object.cpp
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#include "Object.h"

void Object::draw()
{
    material->apply();
    // apply scaling, translation and orientation
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(orientationAngle, orientationAxis.x, orientationAxis.y, orientationAxis.z);
    glScalef(scaleFactor.x, scaleFactor.y, scaleFactor.z);
    drawModel();
    glPopMatrix();
}

void Object::drawShadow(float3 lightDir)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // scale down to shadow shape
    
    lightDir.x = lightDir.x * position.y/lightDir.y;
    lightDir.z = lightDir.z * position.y/lightDir.y;
    glTranslatef(lightDir.x, 0.2, lightDir.z);
    glScalef(1, 0, 1);
    
    glTranslatef(position.x, position.y, position.z);
    glRotatef(orientationAngle, orientationAxis.x, orientationAxis.y, orientationAxis.z);
    glScalef(scaleFactor.x, scaleFactor.y, scaleFactor.z);
    drawModel();
    glPopMatrix();
}