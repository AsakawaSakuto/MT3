#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"imgui.h"
#include"algorithm"
#include"MatrixFunction.h"
#include"mathFunction.h"
#include"DrawObject.h"
#include"CameraController.h"

const char kWindowTitle[] = "LE2B_02_アサカワ_サクト";

int kWindowWidth = 1280;
int kWindowHeight = 720;

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

		Vector3 a = { 0.2f,1.0f,0.0f };
		Vector3 b = { 2.4f,3.1f,1.2f };

		Vector3 c = a + b;
		Vector3 d = a - b;
		Vector3 e = a * 2.4f;
		Vector3 rotate = { 0.4f,1.43f,-0.8f };

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

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

		ImGui::Begin("Window");
		
		ImGui::Text("c:%f,%f,%f", c.x, c.y, c.z);
		ImGui::Text("d:%f,%f,%f", d.x, d.y, d.z);
		ImGui::Text("e:%f,%f,%f", e.x, e.y, e.z);
		ImGui::Text("matrix:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
			rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
			rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
			rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
			rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
		ImGui::Text("CameraController");
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
