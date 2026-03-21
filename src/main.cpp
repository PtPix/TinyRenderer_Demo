#include "../include/tgaimage.h"
#include "../include/geometry.h"
#include "../include/Test.h"
#include "../include/Graphic_Library.h"
#include "../include/model.h"

#include <iostream>
#include <vector>
#include <limits>

// Initialization
Model *model = NULL;
float *shadowbuffer = NULL;
const int width = 800;
const int height = 800;

// Put the camera and light
Vec3f eye   (1,  1, 4);
Vec3f center(0,  0, 0);
Vec3f up    (0,  1, 0);

Vec3f light_direction(1, 1, 1);

// Implement Shader 
// Shadowmapping Shader
class DepthShader : public IShader {
public:
    mat<3, 3, float> varying_tri;

    DepthShader() : varying_tri() {}

    virtual Vec4f vertex(int iface, int nthvert) {
        Vec4f gl_vertex = model->vert(iface, nthvert).projectTo4();
        gl_vertex = ViewPortMatrix * ProjectionMatrix * ModelViewMatrix * gl_vertex;
        varying_tri[0][nthvert] = gl_vertex[0] / gl_vertex[3];
        varying_tri[1][nthvert] = gl_vertex[1] / gl_vertex[3];
        varying_tri[2][nthvert] = gl_vertex[2] / gl_vertex[3];
        gl_vertex = gl_vertex.renormalize();
        return gl_vertex;
    }
    virtual bool fragment(Vec3f bar, TGAColor &color) {
        Vec3f p = varying_tri * bar;
        color = TGAColor(255, 255, 255) * (p[2] / 500.f);
        return false;
    }
};

class ShadowShader : public IShader {
public:
    mat<3, 3, float> varying_tri;
    mat<2, 3, float> varying_uv;
    mat<4, 4, float> Mshadow;

    ShadowShader(Matrix Mshadow) : varying_tri(), varying_uv(), Mshadow(Mshadow) {}

    virtual Vec4f vertex(int iface, int nthvert) {
        varying_uv[0][nthvert] = model->diffuse(iface, nthvert)[0];
        varying_uv[1][nthvert] = model->diffuse(iface, nthvert)[1];
        Vec4f gl_vertex = model->vert(iface, nthvert).projectTo4();
        gl_vertex = ViewPortMatrix * ProjectionMatrix * ModelViewMatrix * gl_vertex;
        varying_tri[0][nthvert] = gl_vertex[0] / gl_vertex[3];
        varying_tri[1][nthvert] = gl_vertex[1] / gl_vertex[3];
        varying_tri[2][nthvert] = gl_vertex[2] / gl_vertex[3];
        gl_vertex = gl_vertex.renormalize();
        return gl_vertex;
    }
    virtual bool fragment(Vec3f bar, TGAColor &color) {
        Vec4f p = (Mshadow * (varying_tri * bar).projectTo4()).renormalize();
        // std::cout << p << std::endl;
        int index = int(p[0]) + int(p[1]) * width;
        // if (shadowbuffer[index] < p[2]) {
        //     std::cout << shadowbuffer[index] << " " ;
        // }
        
        float shadow = 0.3f + 0.7f * (shadowbuffer[index] < p[2] + 20.f);
        // float shadow = 0.3f + 0.7f;
        Vec2f uv = varying_uv * bar;

        Vec3f normal = model->normal(uv);
        Vec3f reflect_direction = (normal * (normal * light_direction * 2.f) - light_direction).normalize();
        float spec = pow(std::max(reflect_direction[2], 0.0f), 5 + model->spec(uv)[0]);
        // std::cout << spec <<std::endl;
        float diff = std::max(0.f, normal * light_direction);
        color = model->diffuse_color(uv);
        for (int i = 0; i < 3; i++) {
            color[i] = std::min<float>(255, 20 + color[i] * shadow * (1.2f * diff + 0.6f * spec));
        }
        return false;
    }
};

int main(int argc, char** argv) {
    // Test_all();
    if (2 == argc) {model = new Model(argv[1]);}
    else {model = new Model("obj/diablo3_pose/diablo3_pose.obj");}
    // else {model = new Model("obj/african_head/african_head.obj");}

    float *zbuffer = new float [width * height];
    shadowbuffer = new float [width * height];

    for (int i = width * height; --i;) {
        zbuffer[i] = shadowbuffer[i] = -std::numeric_limits<float>::max();
    }

    light_direction.normalize();

    // Get the shadow buffer
    {
        TGAImage depth(width, height, TGAImage::RGB);
        lookat(light_direction, center, up);
        viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        projection(0);

        DepthShader depthshader;
        Vec4f screen_coordinates[3];
        for (int i = 0; i < model->nfaces(); i++) {
            for (int j = 0; j < 3; j++) {
                // screen_coordinates[j] = depthshader.vertex(i, j);
                for (int k = 0; k < 4; k++) {
                    screen_coordinates[j][k] = (int)(depthshader.vertex(i, j)[k] + 0.5f);
                }
            }
            triangle(screen_coordinates, depthshader, depth, shadowbuffer);
        }
        depth.flip_vertically();
        depth.write_tga_file("depth.tga");
    }

    Matrix M = ViewPortMatrix * ProjectionMatrix * ModelViewMatrix;

    {
        TGAImage frame(width, height, TGAImage::RGB);
        lookat(eye, center, up);
        viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        projection(-1.f / (eye - center).norm());
        Matrix temp = ViewPortMatrix * ProjectionMatrix * ModelViewMatrix;
        ShadowShader shadowmapping(M * inverse(temp));
        // std::cout << temp << std::endl;
        // std::cout << inverse(temp) << std::endl;
        // ShadowShader shadowmapping(M);
        Vec4f screen_coordinates[3];
        for (int i = 0; i < model->nfaces(); i++) {
            for (int j = 0; j < 3; j++) {
                // screen_coordinates[j] = shadowmapping.vertex(i, j);
                for (int k = 0; k < 4; k++) {
                    screen_coordinates[j][k] = (int)(shadowmapping.vertex(i, j)[k] + 0.5f);
                }
            }
            triangle(screen_coordinates, shadowmapping, frame, zbuffer);
        }
        frame.flip_vertically();
        frame.write_tga_file("framebuffer.tga");
    }

    delete model;
    delete[] zbuffer;
    delete[] shadowbuffer;
    return 0;
}