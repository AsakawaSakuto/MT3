#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"Matrix4x4Function.h"
#include <imgui.h>

struct Sphere {
    Vector3 center;//���S�_
    float radius;//���a
};

//�O���b�h��\������
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//�X�t�B�A��\������
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);