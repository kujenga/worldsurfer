//
//  Scene.h
//  incremental
//
//  Created by Aaron Taylor on 12/4/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_Scene_h
#define incremental_Scene_h

#include "OpenGL.h"

#include "MobiusStrip.h"
#include "Entity.h"
#include "Camera.h"
#include "LightSource.h"

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
    
    // fills the scene with all OpenGL dependant objects
    void initialize();
    
    // draws all the light sources and objects in the scene and performs camera movement
    void draw();
    
    // passes the status of key presses to all objects in the scene
    // TODO: get rid of a few hacky issues with handling shooting at this high of a level. should be handled by Player class
    void control(std::vector<bool>& keysPressed);
    
    // moves all the objects in the world and performs collision detection as well
    void move(double t, double dt);
    
protected:
    // method to determine if an object has collided with any other objects
    // returns the index of the object is true, otherwise returns -1
    int objhasCollided(int objIndex);
    
};


#endif
