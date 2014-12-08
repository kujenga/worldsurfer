//
//  Material.h
//  incremental
//
//  Created by Aaron Taylor on 12/4/14.
//  Copyright (c) 2014 Aaron Taylor. All rights reserved.
//

#ifndef incremental_Material_h
#define incremental_Material_h

extern "C" unsigned char* stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);

class Material
{
public:
    float3 kd;			// diffuse reflection coefficient
    float3 ks;			// specular reflection coefficient
    float shininess;	// specular exponent
    Material()
    {
        kd = float3(0.5, 0.5, 0.5) + kd * 0.5;
        ks = float3(1, 1, 1);
        shininess = 15;
    }
    virtual void apply()
    {
         glDisable(GL_TEXTURE_2D);
        float aglDiffuse[] = {kd.x, kd.y, kd.z, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, aglDiffuse);
        float aglSpecular[] = {kd.x, kd.y, kd.z, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, aglSpecular);
        if(shininess <= 128)
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        else
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
    }
};

class TexturedMaterial : public Material
{
protected:
    unsigned int textureName;
    GLint filteringMode;
public:
    TexturedMaterial(const char* filename, GLint filtering = GL_LINEAR_MIPMAP_LINEAR){
        unsigned char* data;
        int width;
        int height;
        int nComponents = 4;
        
        data = stbi_load(filename, &width, &height, &nComponents, 0);
        
        if(data == NULL) return;
        
        if (filtering == GL_LINEAR_MIPMAP_LINEAR) {
            if(nComponents == 4)
                gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else if(nComponents == 3)
                gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            glGenTextures(1, &textureName);  // id generation
            glBindTexture(GL_TEXTURE_2D, textureName);      // binding
            
            if(nComponents == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //uploading
            } else if(nComponents == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //uploading
            }
        }
        
        delete data;
    }
    virtual void apply()
    {
        Material::apply();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureName);
        // when texture area is small, bilinear filter the closest mipmap
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_NEAREST );
        // when texture area is large, bilinear filter the original
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    
};

#endif
