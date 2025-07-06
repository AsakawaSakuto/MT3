#pragma once
#include "Vector3.h"
#include "math.h"
#include "algorithm"

// 球体
//struct Ball {
//	Vector3 center; // 中心点
//	float radius;   // 半径
//};

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

struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

// クロス積（ベクトル積）
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);