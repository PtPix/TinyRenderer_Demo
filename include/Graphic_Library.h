#ifndef GRAPHIC_LIBRARY_H
#define GRAPHIC_LIBRARY_H

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

extern Matrix ModelViewMatrix;
extern Matrix ProjectionMatrix;
extern Matrix ViewPortMatrix;

void viewport(int x, int y, int width, int height);
void projection(float coeff);
void lookat(Vec3f eye, Vec3f center, Vec3f up);

class IShader {
public:
    virtual ~IShader();
    virtual Vec4f vertex(int face, int vert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
};

void triangle(Vec4f *pts, IShader& shader, TGAImage& image, TGAImage& zbuffer);
void triangle(Vec4f *pts, IShader &shader, TGAImage &image, float *zbuffer);


#endif