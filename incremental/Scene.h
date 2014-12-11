//
//  Scene.h
//  incremental
//
//  Created by Aaron Taylor on 12/4/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_Scene_h
#define incremental_Scene_h

#include "Camera.h"
#include "LightSource.h"
#include "Object.h"
#include "MobiusStrip.h"
#include "Entity.h"
#include "MeshEntity.h"

// used to generate random points
#define ARC4RANDOM_MAX 0x100000000
#define LITTLE_RAND ((double)arc4random() / ARC4RANDOM_MAX)

#define WORLD_RADIUS 50
#define WORLD_WIDTH 40

class Scene
{
    Camera camera;
    std::vector<LightSource*> lightSources;
    std::vector<Object*> objects;
    std::vector<Object*> spawns;
    std::vector<Material*> materials;
    std::vector<Mesh*> meshVector;
    
    MobiusStrip *worldGround;
    Entity *player;
    
    bool globalCamera = false;
    
    bool isShooting = false;
    
public:
    Scene() {}
    Scene(std::vector<Mesh*> meshes) : meshVector(meshes)
    {
        // BUILD YOUR SCENE HERE
        lightSources.push_back(new DirectionalLight(float3(5, 10, 5),
                                                    float3(3, 3, 3)));
        lightSources.push_back(new DirectionalLight(float3(-5, -10, -5),
                                                    float3(3, 3, 3)));
        lightSources.push_back(new PointLight(float3(0, -5, 0),
                                              float3(40, 50, 40)));
        Material* yellowDiffuseMaterial = new Material();
        materials.push_back(yellowDiffuseMaterial);
        yellowDiffuseMaterial->kd = float3(1, 1, 0);
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
    }
    ~Scene()
    {
        for (std::vector<LightSource*>::iterator iLightSource = lightSources.begin(); iLightSource != lightSources.end(); ++iLightSource)
            delete *iLightSource;
        for (std::vector<Material*>::iterator iMaterial = materials.begin(); iMaterial != materials.end(); ++iMaterial)
            delete *iMaterial;
        for (std::vector<Object*>::iterator iObject = objects.begin(); iObject != objects.end(); ++iObject)
            delete *iObject;
    }
    
public:
    Camera& getCamera()
    {
        return camera;
    }
    
    void initialize() {
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
    
    void draw()
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
    }
    
    void control(std::vector<bool>& keysPressed) {
        if (keysPressed.at('c')) {
            globalCamera = !globalCamera;
            camera.globalView = globalCamera;
        }
        // shoot action
        if (keysPressed.at(' ')) {
            if (!isShooting) {
                isShooting = true;
                objects.push_back(((Player*)player)->newBullet());
            }
        } else {
            isShooting = false;
        }
        for (int i = 0; i < objects.size(); i++) {
            objects.at(i)->control(keysPressed, spawns, objects);
        }
    }
    
    void move(double t, double dt) {
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
    
protected:
    int objhasCollided(int objIndex)
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
};


#endif
