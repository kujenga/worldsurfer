//
//  main.cpp
//  incremental
//
//  Created by Aaron Taylor on 11/27/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// Needed on MsWindows
#include <windows.h>
#endif // Win32 platform

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// Download glut from: http://www.opengl.org/resources/libraries/glut/
#include <GLUT/glut.h>

#include "float2.h"
#include "float3.h"
#include <vector>
#include <map>

#include "Object.h"
#include "Material.h"
#include "Mesh.h"



class LightSource
{
public:
    virtual float3 getpowerDensityAt  ( float3 x )=0;
    virtual float3 getLightDirAt  ( float3 x )=0;
    virtual float  getDistanceFrom( float3 x )=0;
    virtual void   apply( GLenum openglLightName )=0;
};

class DirectionalLight : public LightSource
{
    float3 dir;
    float3 powerDensity;
public:
    DirectionalLight(float3 dir, float3 powerDensity)
    :dir(dir), powerDensity(powerDensity){}
    float3 getpowerDensityAt  ( float3 x ){return powerDensity;}
    float3 getLightDirAt  ( float3 x ){return dir;}
    float  getDistanceFrom( float3 x ){return 900000000;}
    void   apply( GLenum openglLightName )
    {
        float aglPos[] = {dir.x, dir.y, dir.z, 0.0f};
        glLightfv(openglLightName, GL_POSITION, aglPos);
        float aglZero[] = {0.0f, 0.0f, 0.0f, 0.0f};
        glLightfv(openglLightName, GL_AMBIENT, aglZero);
        float aglIntensity[] = {powerDensity.x, powerDensity.y, powerDensity.z, 1.0f};
        glLightfv(openglLightName, GL_DIFFUSE, aglIntensity);
        glLightfv(openglLightName, GL_SPECULAR, aglIntensity);
        glLightf(openglLightName, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(openglLightName, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(openglLightName, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
};

class PointLight : public LightSource
{
    float3 pos;
    float3 power;
public:
    PointLight(float3 pos, float3 power)
    :pos(pos), power(power){}
    float3 getpowerDensityAt  ( float3 x ){return power*(1/(x-pos).norm2()*4*3.14);}
    float3 getLightDirAt  ( float3 x ){return (pos-x).normalize();}
    float  getDistanceFrom( float3 x ){return (pos-x).norm();}
    void   apply( GLenum openglLightName )
    {
        float aglPos[] = {pos.x, pos.y, pos.z, 1.0f};
        glLightfv(openglLightName, GL_POSITION, aglPos);
        float aglZero[] = {0.0f, 0.0f, 0.0f, 0.0f};
        glLightfv(openglLightName, GL_AMBIENT, aglZero);
        float aglIntensity[] = {power.x, power.y, power.z, 1.0f};
        glLightfv(openglLightName, GL_DIFFUSE, aglIntensity);
        glLightfv(openglLightName, GL_SPECULAR, aglIntensity);
        glLightf(openglLightName, GL_CONSTANT_ATTENUATION, 0.0f);
        glLightf(openglLightName, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(openglLightName, GL_QUADRATIC_ATTENUATION, 0.25f / 3.14f);
    }
};


// Skeletal Camera class. Feel free to add custom initialization, set aspect ratio to fit viewport dimensions, or animation.
class Camera
{
    float3 eye;
    
    float3 ahead;
    float3 right;
    float3 up;
    
    float fov;
    float aspect;
    
    float2 lastMousePos;
    float2 mouseDelta;
public:
    float3 getEye()
    {
        return eye;
    }
    Camera()
    {
        eye = float3(0, 1, 20);
        ahead = float3(0, 0, -1);
        right = float3(1, 0, 0);
        up = float3(0, 1, 0);
        fov = 1.5;
        aspect = 1;
    }
    
    void apply()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov / 3.14 * 180, aspect, 0.1, 200);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye.x, eye.y, eye.z, eye.x+ahead.x, eye.y+ahead.y, eye.z+ahead.z, 0.0, 1.0, 0.0);
    }
    
    void startDrag(int x, int y) {
        lastMousePos = float2(x, y);
    }
    void drag(int x, int y){
        float2 mousePos(x, y);
        mouseDelta = mousePos - lastMousePos;
        lastMousePos = mousePos;
    }
    void endDrag(){
        mouseDelta = float2(0, 0);
    }
    void move(float dt, std::vector<bool>& keysPressed) {
        if(keysPressed.at('w'))
            eye += ahead * dt * 20;
        if(keysPressed.at('s'))
            eye -= ahead * dt * 20;
        if(keysPressed.at('a'))
            eye -= right * dt * 20;
        if(keysPressed.at('d'))
            eye += right * dt * 20;
        if(keysPressed.at('q'))
            eye -= float3(0,1,0) * dt * 20;
        if(keysPressed.at('e'))
            eye += float3(0,1,0) * dt * 20;
        
        float yaw = atan2f( ahead.x, ahead.z );
        float pitch = -atan2f( ahead.y,
                              sqrtf(ahead.x * ahead.x + ahead.z * ahead.z) );
        
        yaw -= mouseDelta.x * 0.02f;
        pitch += mouseDelta.y * 0.02f;
        if(pitch > 3.14/2) pitch = 3.14/2;
        if(pitch < -3.14/2) pitch = -3.14/2;
        
        mouseDelta = float2(0, 0);
        
        ahead = float3(sin(yaw)*cos(pitch), -sin(pitch),
                       cos(yaw)*cos(pitch) );
        right = ahead.cross(float3(0, 1, 0)).normalize();
        up = right.cross(ahead);
    }
    void setAspectRatio(float ar)  {
        aspect = ar;
    }

};


class Scene
{
    Camera camera;
    std::vector<LightSource*> lightSources;
    std::vector<Object*> objects;
    std::vector<Material*> materials;
    std::vector<Mesh*> meshVector;
public:
    Scene() {}
    Scene(std::vector<Mesh*> meshes) : meshVector(meshes)
    {
        // BUILD YOUR SCENE HERE
        lightSources.push_back(new DirectionalLight(
                                                    float3(0, 1, 0),
                                                    float3(1, 0.5, 1)));
        lightSources.push_back(new PointLight(
                                              float3(-1, -1, 1),
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
        objects.push_back(new Ground(groundMaterial));
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
        
        for (unsigned int iObject=0; iObject<objects.size(); iObject++)
            objects.at(iObject)->draw();
    }
    
    void control() {
        
    }
    
    void move(double t, double dt) {
        for (int i = 0; i < objects.size(); i++) {
            objects.at(i)->move(t, dt);
        }
    }
};

// global application data

// screen resolution
const int screenWidth = 600;
const int screenHeight = 600;

//scene object
Scene scene = Scene(std::vector<Mesh*>());

// vector of the keys that kaye been pressed
std::vector<bool> keysPressed;

// Displays the image.
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen
    
    scene.draw();
    
    glutSwapBuffers(); // drawing finished
}

void onIdle()
{
    double t = glutGet(GLUT_ELAPSED_TIME) * 0.001;
    static double lastTime = 0.0;
    double dt = t - lastTime;
    lastTime = t;
    
    scene.getCamera().move(dt, keysPressed);
    scene.control();
    scene.move(t, dt);
    glutPostRedisplay();
}

void onKeyboard(unsigned char key, int x, int y) {
    keysPressed.at(key) = true;
}

void onKeyboardUp(unsigned char key, int x, int y) {
    keysPressed.at(key) = false;
}

void onMouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON)
        if(state == GLUT_DOWN) {
            scene.getCamera().startDrag(x, y);
        } else {
            scene.getCamera().endDrag();
        }
}

void onMouseMotion(int x, int y) {
    scene.getCamera().drag(x, y);
}

void onReshape(int winWidth, int winHeight) {
    glViewport(0, 0, winWidth, winHeight);
    scene.getCamera().setAspectRatio(
                                     (float)winWidth/winHeight);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);						// initialize GLUT
    glutInitWindowSize(screenWidth, screenHeight);				// startup window size 
    glutInitWindowPosition(100, 100);           // where to put window on screen
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // 8 bit R,G,B,A + double buffer + depth buffer
    
    glutCreateWindow("OpenGL teapots");				// application window is created and displayed
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    glutDisplayFunc(onDisplay);					// register callback
    glutIdleFunc(onIdle);						// register callback
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    for(int i=0; i<256; i++)
        keysPressed.push_back(false);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMouseMotion);
    glutReshapeFunc(onReshape);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    scene.initialize();
    
    glutMainLoop();								// launch event handling loop
    
    return 0;
}
