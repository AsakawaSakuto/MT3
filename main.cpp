#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"imgui.h"
#include"algorithm"
#include"MatrixFunction.h"
#include"mathFunction.h"
#include"DrawObject.h"
#include"CameraController.h"

#include <cmath>  
#include <string>

const char kWindowTitle[] = "LE2B_02_アサカワ_サクト";

int kWindowWidth = 1280;
int kWindowHeight = 720;

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}

//3次元空間上で２次ベジェ曲線上の点を求める関数
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {

	//制御点p0,p1を線形補間
	Vector3 p01 = Lerp(p0, p1, t);
	//制御点p1,p2を線形補間
	Vector3 p12 = Lerp(p1, p2, t);
	//制御点p0p1,p1p2をさらに線形補間
	Vector3 result = Lerp(p01, p12, t);

	return result;
}

void DrawBezier(const Vector3& controlPoint, const Vector3& controlPoint1, const Vector3& controlPoint2, const int subdivision, 
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color)
{
	// 前の点を保持するための変数
	Vector3 previousScreenPos = { 0,0,0 };
	bool hasPreviousPoint = false;
	for (int i = 0; i <= subdivision; ++i) {
		// tの値を求める
		float t = static_cast<float>(i) / static_cast<float>(subdivision);
		// ベジエ曲線上の点を求める
		Vector3 point = Bezier(controlPoint, controlPoint1, controlPoint2, t);
		// スクリーン座標系に変換
		Vector3 ndcPos = Transform(point, viewProjectionMatrix);
		Vector3 screenPos = Transform(ndcPos, viewportMatrix);
		// 描画
		if (hasPreviousPoint) {
			Novice::DrawLine(
				static_cast<int>(previousScreenPos.x),
				static_cast<int>(previousScreenPos.y),
				static_cast<int>(screenPos.x),
				static_cast<int>(screenPos.y),
				color);
		}
		// 現在の点を次の前の点として保存
		previousScreenPos = screenPos;
		hasPreviousPoint = true;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// カメラ
	Vector3 cameraScale{ 1.0f,1.0f,1.0f };      // カメラのスケール
	Vector3 cameraRotate{ 0.2f,0.0f,0.0f };     // カメラの角度
	Vector3 cameraTranslate{ 0.0f,2.5f,-10.f }; // カメラの位置

	float cameraTranslateSpeed = 0.03f;
	float cameraRotateSpeed = 0.01f;
	bool cameraMode = true;

	///ベジエ曲線
	Vector3 controlPoints[3] = {
		{ -0.8f, 0.58f, 1.0f }, // 制御点1
		{ 1.76f, 1.0f, -0.3f }, // 制御点2
		{ 0.94f, -0.7f, 2.3f }  // 制御点3
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		CameraControllerManager(cameraMode, cameraTranslate, cameraRotate, cameraTranslateSpeed, cameraRotateSpeed, keys, preKeys);

		Matrix4x4 worldMatrix = MakeAffineMatrix(cameraScale, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = InverseMatrix(worldMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 1.0f, 0.0f);

		Matrix4x4 worldViewProjectionMatrix = MultiplyMatrix(viewMatrix, projectionMatrix);

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		for (int i = 0; i < 3; i++)
		{
			DrawSphere({ controlPoints[i], 0.01f }, worldViewProjectionMatrix, viewportMatrix, BLACK);
		}

		DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], 32, worldViewProjectionMatrix, viewportMatrix, BLUE);
		ImGui::Begin("Window");
		
		for (int i = 0; i < 3; i++)
		{
			std::string label = "Control Point " + std::to_string(i);
			ImGui::DragFloat3(label.c_str(), &controlPoints[i].x, 0.01f);

		}

		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		ImGui::End();

		if (cameraMode)
		{
			Novice::ScreenPrintf(20, 20, "CameraMode : Translate : X->(D or A) Y->(Q or E) Z->(W or S)");
		}
		else
		{
			Novice::ScreenPrintf(20, 20, "CameraMode : Rotate : X->(W or S) Y->(Q or E) Z->(D or A)");
		}
		Novice::ScreenPrintf(20, 40, "RotateReset : R");
		Novice::ScreenPrintf(20, 60, "TranslateReset : T");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
