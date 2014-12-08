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

class Scene
{
    Camera camera;
    std::vector<LightSource*> lightSources;
    std::vector<Object*> objects;
    std::vector<Object*> spawns;
    std::vector<Material*> materials;
    std::vector<Mesh*> meshVector;
public:
    Scene() {}
    Scene(std::vector<Mesh*> meshes) : meshVector(meshes)
    {
        // BUILD YOUR SCENE HERE
        lightSources.push_back(new DirectionalLight(float3(0, 1, 0),
                                                    float3(1, 0.5, 1)));
        lightSources.push_back(new PointLight(float3(-1, -1, 1),
                                              float3(0.2, 0.1, 0.1)));
        Material* yellowDiffuseMaterial = new Material();
        materials.push_back(yellowDiffuseMaterial);
        yellowDiffuseMaterial->kd = float3(1, 1, 0);
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        materials.push_back(new Material());
        
        objects.push_back( (new Teapot( yellowDiffuseMaterial))->translate(float3(2, 2, 0)) );
        objects.push_back( (new Teapot( materials.at(1) )     )->translate(float3(0, 3, 2))->scale(float3(0.6, 0.6, 0.6)) );
        objects.push_back( (new Teapot( materials.at(2) )     )->translate(float3(0, 1.2, 0.5))->scale(float3(1.3, 1.3, 1.3)) );
        
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
        meshVector.push_back(new Mesh("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/HundredAcreWood/tigger.obj"));
        materials.push_back(new TexturedMaterial("/Users/ataylor/Documents/Williams/Graphics/incremental/incremental/HundredAcreWood/tigger.png"));
        Bouncer *tigger = new Bouncer( materials.back(), meshVector.front());
        tigger->translate(float3(10, 15, 0));
        tigger->scale(float3(0.5, 0.5, 0.5));
        objects.push_back(tigger);
        
        Material* groundMaterial = new Material();
        materials.push_back(groundMaterial);
        groundMaterial->kd = float3(0.4, 1, 0.4);
        objects.push_back((new MoebiusStrip(groundMaterial))->scale(float3(50,50,50)));
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
        
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glColor3d(0, 0, 0);
        for (iLightSource=0; iLightSource<lightSources.size(); iLightSource++)
            for (unsigned int iObject=0; iObject<objects.size(); iObject++)
                objects.at(iObject)->drawShadow(lightSources.at(iLightSource)->getLightDirAt(objects.at(iObject)->getPosition()));
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        
        for (unsigned int iObject=0; iObject<objects.size(); iObject++)
            objects.at(iObject)->draw();
    }
    
    void control(std::vector<bool>& keysPressed) {
        for (int i = 0; i < objects.size(); i++) {
            objects.at(i)->control(keysPressed, spawns, objects);
        }
    }
    
    void move(double t, double dt) {
        for (int i = 0; i < objects.size(); i++) {
            objects.at(i)->move(t, dt);
        }
    }
};


#endif
