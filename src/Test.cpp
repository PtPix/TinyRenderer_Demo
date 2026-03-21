#include "../include/Test.h"
#include "../include/geometry.h"
#include <iostream>

void Test_all() {
    Test_Vec_f();
    Test_Vec_i();
    Test_Mat_f();
}

void Test_Vec_i() {
    Vec2i a(1, 3);
    Vec2i b(2, 4);
    vec<4, int> e;
    e[0] = 1; e[1] = 2; e[2] = 3; e[3] = 4;
    std::cout << "Test for Vec_i: " << std::endl;
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "2 * a = " << a * 2 << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / b = " << a / 2 << std::endl;
    // std::cout << "e.norm() = " << e.norm() << std::endl;
    Vec3i c(1, 2, 3);
    Vec3i d(2, 4, 7);
    std::cout << "c cross d = " << cross(c, d) << std::endl;
    std::cout << "c project4 = " << c.projectTo4() << std::endl;
}

void Test_Vec_f() {
    Vec2f a(1.3f, 3.5f);
    Vec2f b(2.7f, 4.9f);
    std::cout << "Test for Vec_f: " << std::endl;
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "2 * a = " << a * 2 << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / b = " << a / 2 << std::endl;
    Vec3f c(1.2f, 2.5f, 3.8f);
    Vec3f d(2.1f, 4.8f, 7.4f);
    std::cout << "c cross d = " << cross(c, d) << std::endl;
    std::cout << "a project to 2 = " << c.projectTo2() << std::endl;
}

void Test_Mat_f() {
    std::cout << "Test for float Matrix:" << std::endl;
    mat<3, 3, float> temp = mat<3, 3, float>::identity();
    std::cout << "Identity:\n" << temp << std::endl;
    float arr1[2][3] = {{2.4f, 2.3f, 1.6f}, {1.3f, 5.6f, 2.8f}};
    mat<2, 3, float> m1(arr1);
    std::cout << "Mat1 = \n" << m1 << std::endl;
    float arr2[3][2] = {{2.4f, 2.3f}, {1.6f, 1.3f}, {5.6f, 2.8f}};
    mat<3, 2, float> m2(arr2);
    std::cout << "Mat2 = \n" << m2 << std::endl;
    std::cout << "Mat1 * Mat2 = \n" << m1 * m2 << std::endl;
    std::cout << "Mat2 * Mat1 = \n" << m2 * m1 << std::endl;
    vec<3, float> v1(2.3f, 4.5f, 9.8f);
    std::cout << "m1 * v1 = " << m1 * v1 << std::endl;
    float arr3[4][4] = {{4,7,2,3}, {3,6,4,2}, {2,5,1,8}, {1,2,3,4}};
    mat<4, 4, float> m3(arr3);
    std::cout << deter(temp) << std::endl;
    std::cout << inverse(m3) << std::endl;
}