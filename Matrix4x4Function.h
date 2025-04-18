#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include<assert.h>
#include <iostream>

//�s��̉��Z
Matrix4x4 AddMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

//�s��̌��Z
Matrix4x4 SubtractMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

//�s��̐�
Matrix4x4 MultiplyMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

//�t�s��
Matrix4x4 InverseMatrix(const Matrix4x4& m);

//�]�u�s��
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

//�P�ʍs��
Matrix4x4 IdentityMatrix();

//�ړ��s��
Matrix4x4 MakeTranslateMatrix(const  Vector3& translate);

//�g��k���s��
Matrix4x4 MakeScaleMatrix(const  Vector3& scale);

//���W�ϊ�
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//��]�s��X
Matrix4x4 MakeRotateXMatrix(float rotate);

//��]�s��Y
Matrix4x4 MakeRotateYMatrix(float rotate);

//��]�s��Z
Matrix4x4 MakeRotateZMatrix(float rotate);

//�A�t�B���ϊ�
Matrix4x4 MakeAffineMatrixMatrix(const  Vector3& scale, const  Vector3& rotate, const  Vector3& translate);