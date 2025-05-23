#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"imgui.h"
#include"algorithm"
#include"MatrixFunction.h"
#include"mathFunction.h"
#include"DrawObject.h"

const char kWindowTitle[] = "LE2B_02_アサカワ_サクト";

int kWindowWidth = 1280;
int kWindowHeight = 720;

void CameraTranslateController(Vector3 &cameraT,float cameraSpeed ,char keys[256])
{
	if (keys[DIK_A])
	{
		cameraT.x -= cameraSpeed;
	}
	if (keys[DIK_D])
	{
		cameraT.x += cameraSpeed;
	}
	if (keys[DIK_E])
	{
		cameraT.y -= cameraSpeed;
	}
	if (keys[DIK_Q])
	{
		cameraT.y += cameraSpeed;
	}
	if (keys[DIK_S])
	{
		cameraT.z -= cameraSpeed;
	}
	if (keys[DIK_W])
	{
		cameraT.z += cameraSpeed;
	}
}

void CameraRotateController(Vector3& cameraR, float cameraSpeed, char keys[256])
{
	if (keys[DIK_A])
	{
		cameraR.x -= cameraSpeed;
	}
	if (keys[DIK_D])
	{
		cameraR.x += cameraSpeed;
	}
	if (keys[DIK_E])
	{
		cameraR.y -= cameraSpeed;
	}
	if (keys[DIK_Q])
	{
		cameraR.y += cameraSpeed;
	}
	if (keys[DIK_S])
	{
		cameraR.z -= cameraSpeed;
	}
	if (keys[DIK_W])
	{
		cameraR.z += cameraSpeed;
	}
}

void CameraControllerManager(bool& cameraMode, Vector3& cameraT, Vector3& cameraR, float cameraTlanslateSpeed, float cameraRotateSpeed, char keys[256], char preKeys[256])
{
	if (cameraMode)
	{
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			cameraMode = false;
		}
		CameraTranslateController(cameraT, cameraTlanslateSpeed, keys);
	}
	else
	{
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			cameraMode = true;
		}
		CameraRotateController(cameraR, cameraRotateSpeed, keys);
	}
	if (keys[DIK_R])
	{
		cameraT = { 0.0f,2.5f,-10.f };
		cameraR = { 0.2f,0.0f,0.0f };
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

	float cameraTranslateSpeed = 0.02f;
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
		
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		ImGui::End();

		if (cameraMode)
		{
			Novice::ScreenPrintf(20, 20, "CameraMode : Translate");
		}
		else
		{
			Novice::ScreenPrintf(20, 20, "CameraMode : Rotate");
		}
		Novice::ScreenPrintf(20, 40, "ValueReset : R");

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
