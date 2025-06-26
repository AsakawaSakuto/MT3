#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"imgui.h"
#include"algorithm"
#include"MatrixFunction.h"
#include"mathFunction.h"
#include"DrawObject.h"
#include"CameraController.h"

#include<string>

const char kWindowTitle[] = "LE2B_02_アサカワ_サクト";

int kWindowWidth = 1280;
int kWindowHeight = 720;

enum class parts {
	Shoulder,		// 肩
	Elbow,			// 肘
	Hand			// 手
};

// 階層構造の変換行列を計算
void UpdateHierarchy(const Vector3 translate[], const Vector3 rotate[], const Vector3 scale[], Matrix4x4 worldMatrix[], int partCount) {

	// partsの数だけ回す
	for (int i = 0; i < partCount; i++) {

		// ローカル変換行列を作成
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale[i]);
		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(rotate[i]);
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate[i]);

		// SRT順でローカルのアフィン行列
		Matrix4x4 localMatrix = MultiplyMatrix(scaleMatrix, MultiplyMatrix(rotateMatrix, translateMatrix));

		///iでそれが根元(親)かどうか判定する
		if (i == 0) {
			// 親がいない場合はローカル変換がそのままワールド変換
			worldMatrix[i] = localMatrix;
		} else {
			// 親がいる場合は親のワールド変換を適用
			worldMatrix[i] = MultiplyMatrix(localMatrix, worldMatrix[i - 1]);
		}
	}
}

// 階層構造を描画
void DrawHierarchy(const Matrix4x4 worldMatrix[], const uint32_t color[], int partCount,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	float sphereRadius) {

	for (int i = 0; i < partCount; i++) {
		// パーツの位置を取得
		Vector3 worldPosition = {
			worldMatrix[i].m[3][0],
			worldMatrix[i].m[3][1],
			worldMatrix[i].m[3][2]
		};

		// 球体描画
		DrawSphere({ worldPosition, sphereRadius }, viewProjectionMatrix, viewportMatrix, color[i]);

		// 親と線を描画
		if (i != 0) {
			// 親の座標を求める
			Vector3 parentPosition = {
				worldMatrix[i - 1].m[3][0],
				worldMatrix[i - 1].m[3][1],
				worldMatrix[i - 1].m[3][2]
			};

			// スクリーン座標変換
			Vector3 tPos1 = Transform(worldPosition, viewProjectionMatrix);
			Vector3 tPos2 = Transform(parentPosition, viewProjectionMatrix);

			Vector3 screenPos1 = Transform(tPos1, viewportMatrix);
			Vector3 screenPos2 = Transform(tPos2, viewportMatrix);

			// 線を描画
			Novice::DrawLine(
				static_cast<int>(screenPos1.x),
				static_cast<int>(screenPos1.y),
				static_cast<int>(screenPos2.x),
				static_cast<int>(screenPos2.y),
				WHITE
			);
		}
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

	// 各パーツのローカル変換
	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f}
	};

	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, 0.0f}	
	};

	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}
	};

	// ワールド座標
	Matrix4x4 worldMatrices[3];

	// 各パーツの色
	uint32_t colors[3] = {
		RED,   // Shoulder 赤
	    GREEN, // Elbow    緑
		BLUE   // Hand     青
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

		UpdateHierarchy(translates, rotates, scales, worldMatrices, 3);

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

		// 階層構造の描画
		DrawHierarchy(worldMatrices, colors, 3, worldViewProjectionMatrix, viewportMatrix, 0.05f);

		ImGui::Begin("Window");
		
		// 各パーツのImGui
		ImGui::Text("Shoulder");
		ImGui::DragFloat3("Position", &translates[0].x, 0.01f);
		ImGui::DragFloat3("Rotation", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("Scale", &scales[0].x, 0.01f, 0.1f, 5.0f);

		ImGui::Text("Elbow");
		ImGui::DragFloat3("Position", &translates[1].x, 0.01f);
		ImGui::DragFloat3("Rotation", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("Scale", &scales[1].x, 0.01f, 0.1f, 5.0f);

		ImGui::Text("Hand");
		ImGui::DragFloat3("Position", &translates[2].x, 0.01f);
		ImGui::DragFloat3("Rotation", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("Scale", &scales[2].x, 0.01f, 0.1f, 5.0f);

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
