#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
    // Store verts information
    std::vector<Vec3f> verts;
    // Store faces information, faces[i] = (point0, texture0, normal0,...)
    std::vector<std::vector<int>> faces;
    // Store texture coordinate information
    std::vector<Vec2f> diffuses;
    // Store normal vector information
    std::vector<Vec3f> normal_vector;
    // Store the diffusemap
    TGAImage diffusemap;
    // Normal mapping
    TGAImage normal_map;
    // Phong spec mapping
    TGAImage spec_map;
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    Vec3f vert(int iface, int nthvert);
    Vec2f diffuse(int i);
    Vec2f diffuse(int iface, int nthvert);
    Vec3f normal(int i);
    Vec3f normal(int iface, int ithvert);
    Vec3f normal(const Vec2f &uv);
    std::vector<int> face(int i);
    TGAColor diffuse_color(Vec2f diffuse);
    Vec3f spec(const Vec2f &uv);
};


#endif