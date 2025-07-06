#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"imgui.h"
#include"algorithm"
#include"MatrixFunction.h"
#include"mathFunction.h"
#include"DrawObject.h"
#include"CameraController.h"
#include"Vector3.h"

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

	const float deltaTime = 1.0f / 60.0f; // 固定タイムステップ (秒)

	// 振り子のパラメータ
	float pendulumLength = 1.5f;
	float gravity = 9.8f;
	float initialAngle = static_cast<float>(M_PI) / 4.0f;
	float currentAngle = initialAngle;
	float angularVelocity = 0.0f;
	bool isAnimationActive = false;
	Vector3 pivotPosition = { 0.0f, 1.5f, 0.0f };

	Sphere ball;
	ball.radius = 0.1f;
	ball.center.x = pivotPosition.x + pendulumLength * std::sin(currentAngle);
	ball.center.y = pivotPosition.y - pendulumLength * std::cos(currentAngle);
	ball.center.z = pivotPosition.z;

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

		if (isAnimationActive) {
			if (pendulumLength > 1e-6f) {
				float angularAcceleration = -(gravity / pendulumLength) * std::sin(currentAngle);
				angularVelocity += angularAcceleration * deltaTime;
			}
			currentAngle += angularVelocity * deltaTime;

			// ボールを更新
			ball.center.x = pivotPosition.x + pendulumLength * std::sin(currentAngle);
			ball.center.y = pivotPosition.y - pendulumLength * std::cos(currentAngle);
			ball.center.z = pivotPosition.z;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawSphere(ball, worldViewProjectionMatrix, viewportMatrix, WHITE);

		Vector3 screen1 = Transform(pivotPosition, MultiplyMatrix(worldViewProjectionMatrix, viewportMatrix));
		Vector3 screen2 = Transform(ball.center, MultiplyMatrix(worldViewProjectionMatrix, viewportMatrix));
		Novice::DrawLine(int(screen1.x), int(screen1.y), int(screen2.x), int(screen2.y), WHITE);

		ImGui::Begin("Window");
		
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		if (ImGui::Button("Start")) {
			isAnimationActive = true;
			currentAngle = initialAngle;
			angularVelocity = 0.0f;
			// ボールを初期状態にする
			ball.center.x = pivotPosition.x + pendulumLength * std::sin(currentAngle);
			ball.center.y = pivotPosition.y - pendulumLength * std::cos(currentAngle);
			ball.center.z = pivotPosition.z;
		}

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
