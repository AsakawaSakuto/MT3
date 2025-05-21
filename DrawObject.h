#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"Matrix4x4Function.h"
#include <imgui.h>

struct Sphere {
    Vector3 center;//中心点
    float radius;//半径
};

//グリッドを表示する
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//スフィアを表示する
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);