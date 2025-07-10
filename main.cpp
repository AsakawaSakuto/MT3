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
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// カメラ
	Vector3 cameraScale{ 1.0f,1.0f,1.0f };      // カメラのスケール
	Vector3 cameraRotate{ 0.2f,0.0f,0.0f };     // カメラの角度
	Vector3 cameraTranslate{ 0.0f,2.5f,-10.f }; // カメラの位置

	float cameraTranslateSpeed = 0.03f;
	float cameraRotateSpeed = 0.01f;
	bool cameraMode = true;

	bool isAnimationActive = false;

	const float deltaTime = 1.0f / 60.0f; // 固定タイム
	const float gravity = -9.8f;

	Plane plane;
	plane.normal = Normalize({ -0.2f,0.9f,-0.3f });
	plane.distance = 0.0f;

	Ball ball = {};
	ball.position = { 0.8f,1.2f,0.3f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.restitution = 0.6f;
	ball.color = WHITE;

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
			// 移動前の位置を記録
			Vector3 prevPos = ball.position;

			// 重力適用
			ball.acceleration.y = gravity;
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;

			// 衝突判定
			float distCurrent = Dot(ball.position, plane.normal) - plane.distance;

			if (distCurrent < ball.radius) {
				float distPrev = Dot(prevPos, plane.normal) - plane.distance;
				Vector3 moveVec = ball.position - prevPos;
				float moveDotNormal = Dot(moveVec, plane.normal);

				Vector3 correction{};

				// 前のフレームでは非衝突 && 現在衝突 && 法線方向に移動していた
				if (distPrev >= ball.radius && moveDotNormal < 0.0f && std::abs(moveDotNormal) > 1e-6f) {
					float t = (ball.radius - distPrev) / moveDotNormal;
					if (t >= 0.0f && t <= 1.0f) {
						correction = moveVec * t;
						ball.position = prevPos + correction;
					} else {
						// 範囲外 → 通常補正
						ball.position += plane.normal * (ball.radius - distCurrent);
					}
				} else {
					// めり込み補正（その他）
					ball.position += plane.normal * (ball.radius - distCurrent);
				}

				// 反射処理
				float vDotN = Dot(ball.velocity, plane.normal);
				if (vDotN < 0.0f) {
					Vector3 reflect = plane.normal * -(1.0f + ball.restitution) * vDotN;
					ball.velocity += reflect;
				}
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, WHITE);

		DrawSphere({ ball.position,ball.radius }, worldViewProjectionMatrix, viewportMatrix, WHITE);

		ImGui::Begin("Window");

		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		if (ImGui::Button("Start")) {
			isAnimationActive = true;
		}
		if (ImGui::Button("BallReset")) {
			isAnimationActive = false;
			ball.position = { 0.8f,1.2f,0.3f };
			ball.velocity = { 0.0f,0.0f,0.0f };
			ball.acceleration = { 0.0f,0.0f,0.0f };
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
