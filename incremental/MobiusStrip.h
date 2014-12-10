//
//  MobiusStrip.h
//  incremental
//
//  Created by Aaron Taylor on 12/8/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_MobiusStrip_h
#define incremental_MobiusStrip_h

#include "Object.h"

class MobiusStrip : public Object {
    float3 origin = float3(0,0,0);
    float radius = 1.0;
    float width = 1.0;
public:
    MobiusStrip(Material* material):Object(material){}
    MobiusStrip(Material* material, float r, float w):Object(material), radius(r), width(2*w) {}
    
    const float getRadius() { return radius; }
    
    void fillPointForAngleOffset(float a, float r, float3 &pt)
    {
        pt.x = cos(a)* ( radius+ (r/2 * cos(a/2)) );
        pt.y = r/2 *sin(a/2);
        pt.z = sin(a)* ( radius+ (r/2 * cos(a/2)) );
    }
    float3 pointForAngleOffset(float a, float r)
    {
        float3 pt;
        fillPointForAngleOffset(a, r, pt);
        return pt;
    }
    float3 normalForAngle(float a)
    {
        float3 base;
        fillPointForAngleOffset(a, -width/2, base);
        float3 p1;
        fillPointForAngleOffset(a-0.001, width/2, p1);
        float3 p2;
        fillPointForAngleOffset(a+0.001, width/2, p2);
        return (p1 - base).cross(base - p2);
    }
    // the forward direction of that point on the strip
    float3 dirVector(float a)
    {
        return float3(-sinf(a), 0, cosf(a));
    }
    // the rotation amount on the specified strip location
    float worldRotation(float a) { return 90*a/M_PI; }
    // performs an openGL rotation on the current matrix, used by entities to adjust their position to the world surface
    void glRotateForObjAtAngle(float a)
    {
        glRotatef(worldRotation(a), -sinf(a), 0, cosf(a));
    }
    
    void drawModel()
    {
        // based on: https://www.opengl.org/discussion_boards/showthread.php/159496-Moebius-Strip
        glBegin(GL_TRIANGLE_STRIP);
        float3 p1 = float3();
        for(float a = 0; a < 4*M_PI; a += 0.1)
        {
            for(float r = -width/2; r <= width/2; r += width-0.001)
            {
                fillPointForAngleOffset(a, r, p1);
                glTexCoord2f(a, r);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 100);
                glVertex3f(p1.x, p1.y, p1.z);
            }
        }
        glEnd();
        // draws the normal line around the track
//        glBegin(GL_LINE_LOOP);
//        p1 = float3();
//        for(float a = 0; a < 4*M_PI; a += 0.1)
//        {
//            fillPointForAngleOffset(a, 0, p1);
//            float3 normPt = p1 - normalForAngle(a);
//            glVertex3f(normPt.x, normPt.y, normPt.z);
//        }
//        glEnd();
    }
    virtual void drawShadow(float3 lightDir) {}
};


#endif
