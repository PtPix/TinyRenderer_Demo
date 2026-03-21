#include <cmath>
#include <limits>
#include "../include/Graphic_Library.h"
#include <iostream>

Matrix ModelViewMatrix;
Matrix ViewPortMatrix;
Matrix ProjectionMatrix;

IShader::~IShader() {}


// So confusing
// put [-1, 1]^3 to [x, x + width] * [y, y + height] * [0, depth = 255]
void viewport(int x, int y, int width, int height) {
    float element[4][4] = {
        {width / 2.f,            0,         0,  x + width / 2.f},
        {          0, height / 2.f,         0, y + height / 2.f},
        {          0,            0, 255 / 2.f,        255 / 2.f},
        {          0,            0,         0,                1}
    };
    ViewPortMatrix = Matrix(element);
}

// confused too.
// project from (0, 0, c) to xOy.
void projection(float coeff) {
    float element[4][4] = {
        {1, 0,     0, 0},
        {0, 1,     0, 0},
        {0, 0,     1, 0},
        {0, 0, coeff, 1}
    };
    ProjectionMatrix = Matrix(element);
}

// Easy.
// Camera Transform.
void lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();
    float element[4][4] = {
        {x[0], x[1], x[2], -center[0]},
        {y[0], y[1], y[2], -center[1]},
        {z[0], z[1], z[2], -center[2]},
        {   0,    0,    0,          1}
    };
    ModelViewMatrix = Matrix(element);
}

// Input 3 points, get the barycentric coordinates for P.
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    Vec3f a(C[0] - A[0], B[0] - A[0], A[0] - P[0]);
    Vec3f b(C[1] - A[1], B[1] - A[1], A[1] - P[1]);
    Vec3f u = cross(a, b);
    if (std::abs(u[2]) > 1e-2) {
        return {1.f - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2]};
    }
    return {-1, 1, 1};
}

// Draw Triangle On image.
void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer) {
    // Get bounding box.
    Vec2f bbox_min, bbox_max;
    bbox_min[0] = std::min(pts[0][0] / pts[0][3], std::min(pts[1][0] / pts[1][3], pts[2][0] / pts[2][3]));
    bbox_max[0] = std::max(pts[0][0] / pts[0][3], std::max(pts[1][0] / pts[1][3], pts[2][0] / pts[2][3]));
    bbox_min[1] = std::min(pts[0][1] / pts[0][3], std::min(pts[1][1] / pts[1][3], pts[2][1] / pts[2][3]));
    bbox_max[1] = std::max(pts[0][1] / pts[0][3], std::max(pts[1][1] / pts[1][3], pts[2][1] / pts[2][3]));

    // Loop the bounding box.
    Vec2f P;
    TGAColor color;
    for (P[0] = bbox_min[0]; P[0] < bbox_max[0]; P[0]++) {
        for (P[1] = bbox_min[1]; P[1] < bbox_max[1]; P[1]++) {
            // Get the barycentric coordinates.
            Vec3f bar = barycentric((pts[0] / pts[0][3]).projectTo2(), (pts[1] / pts[1][3]).projectTo2(), (pts[2] / pts[2][3]).projectTo2(), P);
            // Optimization: judge before 
            if (bar[0] < 0 || bar[1] < 0 || bar[2] < 0) continue;
            // Interpolate the depth
            float z = pts[0][2] * bar[0] + pts[1][2] * bar[1] + pts[2][2] * bar[2];
            float w = pts[0][3] * bar[0] + pts[1][3] * bar[1] + pts[2][3] * bar[2];
            int fragment_depth = std::max(0, std::min(255, int(z / w + 0.5f)));
            // Bigger z means closer.
            if (zbuffer.get(P[0], P[1])[0] > fragment_depth) continue;
            bool discard = shader.fragment(bar, color);
            if (!discard) {
                zbuffer.set(P[0], P[1], TGAColor(fragment_depth));
                image.set(P[0], P[1], color);
            }
        }
    }
}


void triangle(Vec4f *pts, IShader &shader, TGAImage &image, float *zbuffer) {
    // Get bounding box.
    Vec2f bbox_min, bbox_max;
    bbox_min[0] = std::min(pts[0][0] / pts[0][3], std::min(pts[1][0] / pts[1][3], pts[2][0] / pts[2][3]));
    bbox_max[0] = std::max(pts[0][0] / pts[0][3], std::max(pts[1][0] / pts[1][3], pts[2][0] / pts[2][3]));
    bbox_min[1] = std::min(pts[0][1] / pts[0][3], std::min(pts[1][1] / pts[1][3], pts[2][1] / pts[2][3]));
    bbox_max[1] = std::max(pts[0][1] / pts[0][3], std::max(pts[1][1] / pts[1][3], pts[2][1] / pts[2][3]));

    // Loop the bounding box.
    Vec2f P;
    TGAColor color;
    for (P[0] = bbox_min[0]; P[0] < bbox_max[0]; P[0]++) {
        for (P[1] = bbox_min[1]; P[1] < bbox_max[1]; P[1]++) {
            // Get the barycentric coordinates.
            Vec3f bar = barycentric((pts[0] / pts[0][3]).projectTo2(), (pts[1] / pts[1][3]).projectTo2(), (pts[2] / pts[2][3]).projectTo2(), P);
            // Optimization: judge before 
            if (bar[0] < 0 || bar[1] < 0 || bar[2] < 0) continue;
            // Interpolate the depth
            float z = pts[0][2] * bar[0] + pts[1][2] * bar[1] + pts[2][2] * bar[2];
            float w = pts[0][3] * bar[0] + pts[1][3] * bar[1] + pts[2][3] * bar[2];
            int fragment_depth = std::max(0, std::min(255, int(z / w + 0.5f)));
            // Bigger z means closer.
            if (zbuffer[int(P[0] + P[1] * image.get_width())] > fragment_depth) continue;
            bool discard = shader.fragment(bar, color);
            if (!discard) {
                zbuffer[int(P[0] + P[1] * image.get_width())] = fragment_depth;
                image.set(P[0], P[1], color);
            }
        }
    }
}