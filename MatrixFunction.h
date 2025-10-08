#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include"MathFunction.h"
#include<assert.h>
#include <iostream>

//行列の加算
Matrix4x4 AddMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

//行列の減算
Matrix4x4 SubtractMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

//行列の積
Matrix4x4 MultiplyMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

//逆行列
Matrix4x4 InverseMatrix(const Matrix4x4& m);

//転置行列
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

//単位行列
Matrix4x4 IdentityMatrix();

//移動行列
Matrix4x4 MakeTranslateMatrix(const  Vector3& translate);

//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const  Vector3& scale);

//座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//回転行列X
Matrix4x4 MakeRotateXMatrix(float rotate);

//回転行列Y
Matrix4x4 MakeRotateYMatrix(float rotate);

//回転行列Z
Matrix4x4 MakeRotateZMatrix(float rotate);

//アフィン変換
Matrix4x4 MakeAffineMatrix(const  Vector3& scale, const  Vector3& rotate, const  Vector3& translate);

// 正射影行列
Matrix4x4 MakeOrthGraphicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// 透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 任意軸回転行列
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

// from方向 → to方向 に向ける回転行列
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);