//
//  Camera.h
//  incremental
//
//  Created by Aaron Taylor on 12/4/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_Camera_h
#define incremental_Camera_h

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
    bool globalView = false;
    
    void setAhead(float3 dir) { ahead = dir; }
    void setEye(float3 pos) { eye = pos; }
    void setUpDir(float3 dir) { up = dir; }
    
    float3 getEye()
    {
        return eye;
    }
    Camera()
    {
        eye = float3(0, 20, 100);
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
        gluLookAt(eye.x, eye.y, eye.z, eye.x+ahead.x, eye.y+ahead.y, eye.z+ahead.z, up.x, up.y, up.z);
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
        if (globalView) {
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
    }
    void setAspectRatio(float ar)  {
        aspect = ar;
    }
    
};

#endif
