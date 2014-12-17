//
//  Scene.cpp
//  worldsurfer
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#include "Object.h"
#include "MeshInstance.h"
#include "Scene.h"
#include "Player.h"
#include "Racer.h"

void Scene::initialize()
{
    // Mobius Strip ground
    Material *groundMat = new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/models/rainbow.png");
    materials.push_back(groundMat);
    worldGround = new MobiusStrip(groundMat, WORLD_RADIUS, WORLD_WIDTH);
    objects.push_back(worldGround->scale(float3(1,1,1)));
    
    materials.push_back(new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/models/balloon.png"));
    meshVector.push_back(new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/models/balloon.obj"));
    objects.push_back((new MeshInstance(materials.back(), meshVector.back())));
    
    // tigger textures and mesh
    meshVector.push_back(new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/HundredAcreWood/tigger.obj"));
    materials.push_back(new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/HundredAcreWood/tigger.png"));
    
    // creates the player tigger object
    player = new Player(materials.back(), meshVector.back(), worldGround);
    objects.push_back(player);
    
    // Creates the other car racers
    Material *carMatr = new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/chevy/chevy.png");
    materials.push_back(carMatr);
    Mesh *chassisMesh = new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/chevy/chassis.obj");
    meshVector.push_back(chassisMesh);
    for (int i = 0; i < 25; i++) {
        Racer *r =new Racer(carMatr, chassisMesh, worldGround);
        r->setStartDist(0.5*i);
        objects.push_back(r);
    }
    // Creates trees surrounding the mobius track
    Material *treeMatr = new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/HundredAcreWood/tree.png");
    materials.push_back(treeMatr);
    Mesh *treeMesh = new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/HundredAcreWood/tree.obj");
    meshVector.push_back(treeMesh);
    for (float a = 0; a < 2*M_PI; a += (LITTLE_RAND*0.3+0.3)) {
        objects.push_back((new MeshInstance(treeMatr, treeMesh))->translate(worldGround->pointForAngleOffset(a, 0)*3)->scale(float3(2,2,2)));
        objects.push_back((new MeshInstance(treeMatr, treeMesh))->translate(worldGround->pointForAngleOffset(a, 0)*3+float3(0,-10,3))->scale(float3(2,2,2))->rotate(float3(1,0,0),180));
    }
}

void Scene::draw()
{
    camera.apply();
    unsigned int iLightSource=0;
    for (; iLightSource<lightSources.size(); iLightSource++)
    {
        glEnable(GL_LIGHT0 + iLightSource);
        lightSources.at(iLightSource)->apply(GL_LIGHT0 + iLightSource);
    }
    for (; iLightSource<GL_MAX_LIGHTS; iLightSource++)
        glDisable(GL_LIGHT0 + iLightSource);
    
    // Shadows
    //        glDisable(GL_LIGHTING);
    //        glDisable(GL_TEXTURE_2D);
    //        glColor3d(0, 0, 0);
    //        for (iLightSource=0; iLightSource<lightSources.size(); iLightSource++)
    //            for (unsigned int iObject=0; iObject<objects.size(); iObject++)
    //                objects.at(iObject)->drawShadow(lightSources.at(iLightSource)->getLightDirAt(objects.at(iObject)->getPosition()));
    //        glEnable(GL_TEXTURE_2D);
    //        glEnable(GL_LIGHTING);
    
    for (unsigned int iObject=0; iObject<objects.size(); iObject++)
        objects.at(iObject)->draw();
    
    // player-oriented camera transformations
    if (!globalCamera) {
        camera.setEye(player->globalPosition());
        camera.setAhead(player->aheadDirection());
        camera.setUpDir(player->upDirection());
    }
    
    char buf[BUFSIZ];
    char name[] = "Collisions";
    
    snprintf(buf, sizeof(buf), "%s: %04d", name, killCount);
    
    //Assume we are in MODEL_VIEW already
    
    glPushMatrix ();
    glLoadIdentity ();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity();
    
    GLint viewport [4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    gluOrtho2D (0,viewport[2], viewport[3], 0);
    
    glDepthFunc (GL_ALWAYS);
    glColor3f (1,1,1);
    glRasterPos2f(0,0);
    for (char *p = buf; *p; p++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
    
    glDepthFunc (GL_LESS);
    glPopMatrix ();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix ();
}

void Scene::control(std::vector<bool>& keysPressed)
{
    if (keysPressed.at('c')) {
        if (!changingCamera) {
            changingCamera = true;
            globalCamera = !globalCamera;
            camera.globalView = globalCamera;
        }
    } else {
        changingCamera = false;
    }
    // shoot action
    if (keysPressed.at(' ')) {
        if (!isShooting) {
            isShooting = true;
            objects.push_back(((Player*)player)->newBullet());
            killCount++;
        }
    } else {
        isShooting = false;
    }
    for (int i = 0; i < objects.size(); i++) {
        objects.at(i)->control(keysPressed, spawns, objects);
    }
}

void Scene::move(double t, double dt)
{
    for (int i = 0; i < objects.size(); i++) {
        objects.at(i)->move(t, dt);
        int c = objhasCollided(i);
        if (c > 0) {
            // the current entity is behind the object it collided with
            if (((Entity*)objects.at(i))->worldPosition().x < ((Entity*)objects.at(c))->worldPosition().x) {
                objects.at(i)->collide(true);
            } else {
                objects.at(i)->collide(false);
            }
        }
    }
}

int Scene::objhasCollided(int objIndex)
{
    Object *curObj = objects.at(objIndex);
    for (int i = 0; i < objects.size(); i++) {
        if (i == objIndex) {
            continue;
        }
        Entity *testObj = (Entity*)objects.at(i);
        // can't collide with dead objects
        if (!testObj->isAlive()) {
            continue;
        }
        float dist = (curObj->getPosition() - testObj->getPosition()).norm();
        if (dist < 1) {
            return i;
        }
    }
    return -1;
}


