#include "../include/model.h"
#include "../include/tgaimage.h"
#include <vector>
#include <iostream>
#include <sstream>

Model::Model(const char *filename) {
    std::ifstream in(filename, std::ifstream::in);
    if (!in.is_open()) {return;}
    std::string line;
    while (std::getline(in, line)) {
        char trash;
        std::istringstream iss(line.c_str());
        // Get the verts information
        if (line.substr(0, 2) == "v ") {
            iss >> trash;
            Vec3f vert;
            for (int i = 0; i < 3; i++) {iss >> vert[i];}
            verts.push_back(vert);
        }
        // Get the verts to the diffuse information
        else if (line.substr(0, 2) == "vt") {
            iss >> trash >> trash;
            Vec2f diffuse;
            for (int i = 0; i < 2; i++) {iss >> diffuse[i];}
            diffuses.push_back(diffuse);
        }
        // Get the normal vectors information
        else if (line.substr(0, 2) == "vn") {
            iss >> trash >> trash;
            Vec3f normal_vec;
            for (int i = 0; i < 3; i++) {iss >> normal_vec[i];}
            normal_vector.push_back(normal_vec);
        }
        // Get the faces information
        else if (line.substr(0, 2) == "f ") {
            iss >> trash;
            std::vector<int> f;
            int index_point, index_text, index_normal;
            while (iss >> index_point >> trash >> index_text >> trash >> index_normal) {
                // Index in f starts at 1
                index_point--;
                index_text--;
                index_normal--;
                f.push_back(index_point);
                f.push_back(index_text);
                f.push_back(index_normal);
            }
            // After above, f = (point0, texture0, normal0,...)
            faces.push_back(f);
        }
    }
    load_texture(filename, "_diffuse.tga", diffusemap);
    load_texture(filename, "_nm.tga", normal_map);
    load_texture(filename, "_spec.tga", spec_map);
    in.close();
}

Model::~Model() {}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string file_text(filename);
    size_t dot_index = file_text.find_last_of(".");
    if (dot_index == std::string::npos) {return;}
    file_text = file_text.substr(0, dot_index) + std::string(suffix);
    img.read_tga_file(file_text.c_str());
    img.flip_vertically();
}

int Model::nverts() {
    return verts.size();
}

int Model::nfaces() {
    return faces.size();
}

Vec3f Model::vert(int i) {
    return verts[i];
}

Vec3f Model::vert(int iface, int nthvert) {
    return verts[faces[iface][nthvert * 3]];
}

Vec3f Model::spec(const Vec2f &uv) {
    TGAColor color = spec_map.get(uv[0] * spec_map.get_width(), uv[1] * spec_map.get_height());
    Vec3f result;
    result[0] = color.bgra[0];
    result[1] = color.bgra[1];
    result[2] = color.bgra[2];
    return result;
}

Vec2f Model::diffuse(int i) {
    return diffuses[i];
}

Vec2f Model::diffuse(int iface, int nthvert) {
    return diffuses[faces[iface][nthvert * 3 + 1]];
}

Vec3f Model::normal(int i) {
    return normal_vector[i];
}

Vec3f Model::normal(int iface, int ithvert) {
    return normal_vector[faces[iface][ithvert * 3 + 2]];
}

Vec3f Model::normal(const Vec2f &uv) {
    TGAColor color = normal_map.get(uv[0] * normal_map.get_width(), uv[1] * normal_map.get_height());
    Vec3f result;
    result[0] = color.bgra[0] / 255.f * 2 -1.f;
    result[1] = color.bgra[1] / 255.f * 2 -1.f;
    result[2] = color.bgra[2] / 255.f * 2 -1.f;
    return result;
}

std::vector<int> Model::face(int i) {
    return faces[i];
}

TGAColor Model::diffuse_color(Vec2f diffuse) {
    return diffusemap.get(diffuse[0] * diffusemap.get_width(), diffuse[1] * diffusemap.get_height());
}
