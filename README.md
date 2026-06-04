# Software Rasterizer: Shadow Mapping Demo

![C++](https://img.shields.io/badge/C++-11-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

这是一个基于 C++ 编写的轻量级软件光栅化渲染器，旨在不依赖任何图形 API（如 OpenGL/DirectX）的情况下，从底层模拟 GPU 的渲染管线。本项目核心实现了**阴影映射 (Shadow Mapping)** 技术。

## ✨ 核心特性
- **双通路渲染 (Two-pass Rendering)**: 
  - 第一遍：从光源视角渲染，生成深度缓冲区（Shadow Buffer）。
  - 第二遍：从相机视角渲染，利用逆变换矩阵进行深度对比，生成实时阴影。
- **自定义数学库**: 独立实现向量 (`vec<n, T>`)、矩阵 (`mat<n, m, T>`) 的运算，包括矩阵求逆和行列式计算。
- **Shader 抽象**: 模仿现代渲染管线，通过 `IShader` 接口实现顶点着色器和片元着色器。
- **纹理映射**: 支持 Diffuse Map、Normal Map 以及 Specular Map。

## 🛠️ 构建与运行

### 依赖
- 支持 C++11 的编译器 (GCC/Clang/MSVC)
- CMake (3.10+)

### 编译步骤
```bash
mkdir build && cd build
cmake ..
make
```
运行
```Bash
./SoftwareRasterizeRenderer
```
渲染结果将保存为 framebuffer.tga。

## 📂 项目结构说明
src/: 核心逻辑实现，包括光栅化算法（三角形填充、重心坐标计算）。

include/: 头文件，定义了 IShader、Model 及几何运算类。

geometry.cpp: 线性代数库底层实现，包括高斯消元/伴随矩阵求逆。

## 📷 渲染效果
<img width="718" height="739" alt="image" src="https://github.com/user-attachments/assets/605b59eb-b606-4e2b-b37f-d36d1a98dfab" />


## 🎓 致谢
本项目作为学习计算机图形学底层原理的练习，参考了 [TinyRenderer](https://github.com/ssloy/tinyrenderer) 的架构思想。
