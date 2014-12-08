//
//  Object.h
//  incremental
//
//  Created by Aaron Taylor on 12/4/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef __incremental__Object__
#define __incremental__Object__

#include "float4.h"
#include "Mesh.h"
#include "Material.h"

const float3 GRAVITY = float3(0,-10, 0);


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
    const float3 getPosition() { return position; }
    virtual void draw()
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
    virtual void drawModel()=0;
    virtual void move(double t, double dt) {}
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects)
    {
        return false;
    }
    virtual void drawShadow(float3 lightDir)
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
};

class Teapot : public Object
{
public:
    Teapot(Material* material):Object(material){}
    void drawModel()
    {
        glutSolidTeapot(1.0f);
    }
};

// simple flat ground
class Ground : public Object
{
protected:
    float4 origin = float4(0,0,0,0);
public:
    Ground(Material* material):Object(material) {}
    
    void drawModel()
    {
//        glBegin(GL_TRIANGLE_FAN);
//        glVertex4d(0, 0, 0, 1);
//        glVertex4d(1, 0, 0, 0.01);
//        glVertex4d(0, 0, 1, 0.01);
//        glVertex4d(-1, 0, -1, 0.01);
//        glVertex4d(1, 0, 0, 0.01);
//        glEnd();
        
        glBegin(GL_QUADS);
        glNormal3d(0, 1, 0);
        glVertex3f(20, 0, 10);
        glVertex3f(20, 0, -10);
        glVertex3f(-20, 0, -10);
        glVertex3f(-20, 0, 10);
        glEnd();
    }
    virtual void drawShadow(float3 lightDir) {}
};

//////////////////////////////////////////////////////////
// Mesh-based objects
//////////////////////////////////////////////////////////

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
    float restitution = 0.9;
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
        angularVelocity *= pow(0.8, dt);
        
        velocity += GRAVITY*dt; // gravitational acceleration
        velocity += acceleration*dt;
        velocity *= pow(0.8, dt); // drag
        
        position += velocity*dt; // movement
        if (position.y < 0) {
            position.y = 0;
            velocity.y *= -restitution;
        }
    }
    
};


#endif /* defined(__incremental__Object__) */
