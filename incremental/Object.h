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

class Ground : public Object
{
protected:
    float4 origin = float4(0,0,0,0);
public:
    Ground(Material* material):Object(material){}
    void drawModel()
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex4d(0, 0, 0, 1);
        glVertex4d(1, 0, 0, 0);
        glVertex4d(0, 0, 1, 0);
        glVertex4d(-1, 0, -1, 0);
        glVertex4d(1, 0, 0, 0);
        glEnd();
    }
};

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

class Bouncer : public MeshInstance
{
protected:
    float3 velocity = float3(0,0,0);
    float angularVelocity = 0;
    float restitution = 0.9;
public:
    Bouncer(Material* material, Mesh* mesh) : MeshInstance(material, mesh) {}
    
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& spawn, std::vector<Object*>& objects)
    {
        if (keysPressed.at('h')) {
            angularVelocity = 100;
            return true;
        } else if (keysPressed.at('k')) {
            angularVelocity = -100;
            return true;
        }
        angularVelocity = 0;
        return false;
    }
    
    virtual void move(double t, double dt)
    {
        rotate(angularVelocity*dt);
        
        velocity += GRAVITY*dt;
        position += velocity*dt;
        if (position.y < 0) {
            position.y = 0;
            velocity.y *= -restitution;
        }
    }
    
};


#endif /* defined(__incremental__Object__) */
