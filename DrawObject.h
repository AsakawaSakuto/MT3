#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"MatrixFunction.h"
#include <imgui.h>

// 球体
struct Sphere {
    Vector3 center; // 中心点
    float radius;   // 半径
};

// 直線
struct Line {
	Vector3 origin;// 始点
	Vector3 diff;  // 終点への差分ベクトル
};

// 半直線
struct Ray {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

// 線分
struct Segment {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

//グリッドを表示する
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//スフィアを表示する
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);