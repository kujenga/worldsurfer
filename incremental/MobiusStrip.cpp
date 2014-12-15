//
//  MobiusStrip.cpp
//  incremental
//
//  Created by Aaron Taylor on 12/15/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#include "MobiusStrip.h"

///////////////////////////////
// World functions for translation of coordinates
///////////////////////////////

void MobiusStrip::fillPointForAngleOffset(float a, float r, float3 &pt)
{
    pt.x = cos(a)* ( radius+ (r/2 * cos(a/2)) );
    pt.y = r/2 *sin(a/2);
    pt.z = sin(a)* ( radius+ (r/2 * cos(a/2)) );
}

float3 MobiusStrip::normalForAngle(float a)
{
    float3 base;
    fillPointForAngleOffset(a, -width/2, base);
    float3 p1;
    fillPointForAngleOffset(a-0.001, width/2, p1);
    float3 p2;
    fillPointForAngleOffset(a+0.001, width/2, p2);
    return (p1 - base).cross(base - p2);
}

float3 MobiusStrip::dirVector(float a)
{
    return float3(-sinf(a), 0, cosf(a));
}

//////////////////////////////
// Drawing functions
//////////////////////////////

void MobiusStrip::drawModel()
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