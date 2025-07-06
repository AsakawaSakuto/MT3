#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "MatrixFunction.h"
#include "MathFunction.h"
#include <imgui.h>

//グリッドを表示する
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//スフィアを表示する
//void DrawSphere(const Ball& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector);

// 平面
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);