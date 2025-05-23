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

struct AABB
{
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

// AABB1の描画
void DrawAABB1(const AABB& aabb1, Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 vertices1[8];
	vertices1[0] = { aabb1.min.x,aabb1.min.y,aabb1.min.z };
	vertices1[1] = { aabb1.max.x,aabb1.min.y,aabb1.min.z };
	vertices1[2] = { aabb1.max.x,aabb1.min.y,aabb1.max.z };
	vertices1[3] = { aabb1.min.x,aabb1.min.y,aabb1.max.z };

	vertices1[4] = { aabb1.min.x,aabb1.max.y,aabb1.min.z };
	vertices1[5] = { aabb1.max.x,aabb1.max.y,aabb1.min.z };
	vertices1[6] = { aabb1.max.x,aabb1.max.y,aabb1.max.z };
	vertices1[7] = { aabb1.min.x,aabb1.max.y,aabb1.max.z };


	// a,b,cをScreen座標系まで変換
	// スクリーン座標系まで変換

	Matrix4x4 worldMatrix1[8];
	Matrix4x4 worldViewProjectionMatrix1[8];
	Vector3 ndcVertex1[8];
	Vector3 screenVertices1[8];


	for (int i = 0; i < 8; i++)
	{
		worldMatrix1[i] = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, vertices1[i]);

		// WVPMatrix
		worldViewProjectionMatrix1[i] = MultiplyMatrix(worldMatrix1[i], viewProjectionMatrix);

		// 正規化デバイス座標
		ndcVertex1[i] = Transform(Vector3{}, worldViewProjectionMatrix1[i]);

		// スクリーン座標
		screenVertices1[i] = Transform(ndcVertex1[i], viewportMatrix);
	}

	Novice::DrawLine((int)screenVertices1[0].x, (int)screenVertices1[0].y, (int)screenVertices1[1].x, (int)screenVertices1[1].y, color);
	Novice::DrawLine((int)screenVertices1[1].x, (int)screenVertices1[1].y, (int)screenVertices1[2].x, (int)screenVertices1[2].y, color);
	Novice::DrawLine((int)screenVertices1[2].x, (int)screenVertices1[2].y, (int)screenVertices1[3].x, (int)screenVertices1[3].y, color);
	Novice::DrawLine((int)screenVertices1[3].x, (int)screenVertices1[3].y, (int)screenVertices1[0].x, (int)screenVertices1[0].y, color);

	Novice::DrawLine((int)screenVertices1[4].x, (int)screenVertices1[4].y, (int)screenVertices1[5].x, (int)screenVertices1[5].y, color);
	Novice::DrawLine((int)screenVertices1[5].x, (int)screenVertices1[5].y, (int)screenVertices1[6].x, (int)screenVertices1[6].y, color);
	Novice::DrawLine((int)screenVertices1[6].x, (int)screenVertices1[6].y, (int)screenVertices1[7].x, (int)screenVertices1[7].y, color);
	Novice::DrawLine((int)screenVertices1[7].x, (int)screenVertices1[7].y, (int)screenVertices1[4].x, (int)screenVertices1[4].y, color);

	Novice::DrawLine((int)screenVertices1[1].x, (int)screenVertices1[1].y, (int)screenVertices1[5].x, (int)screenVertices1[5].y, color);
	Novice::DrawLine((int)screenVertices1[2].x, (int)screenVertices1[2].y, (int)screenVertices1[6].x, (int)screenVertices1[6].y, color);
	Novice::DrawLine((int)screenVertices1[3].x, (int)screenVertices1[3].y, (int)screenVertices1[7].x, (int)screenVertices1[7].y, color);
	Novice::DrawLine((int)screenVertices1[0].x, (int)screenVertices1[0].y, (int)screenVertices1[4].x, (int)screenVertices1[4].y, color);
}

// AABB2の描画
void DrawAABB2(const AABB& aabb2, Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 vertices2[8];
	vertices2[0] = { aabb2.min.x,aabb2.min.y,aabb2.min.z };
	vertices2[1] = { aabb2.max.x,aabb2.min.y,aabb2.min.z };
	vertices2[2] = { aabb2.max.x,aabb2.min.y,aabb2.max.z };
	vertices2[3] = { aabb2.min.x,aabb2.min.y,aabb2.max.z };
	vertices2[4] = { aabb2.min.x,aabb2.max.y,aabb2.min.z };
	vertices2[5] = { aabb2.max.x,aabb2.max.y,aabb2.min.z };
	vertices2[6] = { aabb2.max.x,aabb2.max.y,aabb2.max.z };
	vertices2[7] = { aabb2.min.x,aabb2.max.y,aabb2.max.z };

	// a,b,cをScreen座標系まで変換
	// スクリーン座標系まで変換

	Matrix4x4 worldMatrix2[8];
	Matrix4x4 worldViewProjectionMatrix2[8];
	Vector3 ndcVertex2[8];
	Vector3 screenVertices2[8];

	for (int i = 0; i < 8; i++)
	{
		worldMatrix2[i] = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, vertices2[i]);

		// WVPMatrix
		worldViewProjectionMatrix2[i] = MultiplyMatrix(worldMatrix2[i], viewProjectionMatrix);

		// 正規化デバイス座標
		ndcVertex2[i] = Transform(Vector3{}, worldViewProjectionMatrix2[i]);

		// スクリーン座標
		screenVertices2[i] = Transform(ndcVertex2[i], viewportMatrix);
	}

	Novice::DrawLine((int)screenVertices2[0].x, (int)screenVertices2[0].y, (int)screenVertices2[1].x, (int)screenVertices2[1].y, color);
	Novice::DrawLine((int)screenVertices2[1].x, (int)screenVertices2[1].y, (int)screenVertices2[2].x, (int)screenVertices2[2].y, color);
	Novice::DrawLine((int)screenVertices2[2].x, (int)screenVertices2[2].y, (int)screenVertices2[3].x, (int)screenVertices2[3].y, color);
	Novice::DrawLine((int)screenVertices2[3].x, (int)screenVertices2[3].y, (int)screenVertices2[0].x, (int)screenVertices2[0].y, color);
	
	Novice::DrawLine((int)screenVertices2[4].x, (int)screenVertices2[4].y, (int)screenVertices2[5].x, (int)screenVertices2[5].y, color);
	Novice::DrawLine((int)screenVertices2[5].x, (int)screenVertices2[5].y, (int)screenVertices2[6].x, (int)screenVertices2[6].y, color);
	Novice::DrawLine((int)screenVertices2[6].x, (int)screenVertices2[6].y, (int)screenVertices2[7].x, (int)screenVertices2[7].y, color);
	Novice::DrawLine((int)screenVertices2[7].x, (int)screenVertices2[7].y, (int)screenVertices2[4].x, (int)screenVertices2[4].y, color);
	
	Novice::DrawLine((int)screenVertices2[1].x, (int)screenVertices2[1].y, (int)screenVertices2[5].x, (int)screenVertices2[5].y, color);
	Novice::DrawLine((int)screenVertices2[2].x, (int)screenVertices2[2].y, (int)screenVertices2[6].x, (int)screenVertices2[6].y, color);
	Novice::DrawLine((int)screenVertices2[3].x, (int)screenVertices2[3].y, (int)screenVertices2[7].x, (int)screenVertices2[7].y, color);
	Novice::DrawLine((int)screenVertices2[0].x, (int)screenVertices2[0].y, (int)screenVertices2[4].x, (int)screenVertices2[4].y, color);
}

// 衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true;
	} 
	else
	{
		return false;
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

	AABB aabb1
	{
		.min{-1.0f,-1.0f,-1.0f},
		.max{0.0f,0.0f,0.0f},
	};

	AABB aabb2
	{
		.min{0.5f,0.5f,0.5f},
		.max{1.5f,1.5f,1.5f},
	};

	int color = WHITE;

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

		// minとmaxが入れ替わらないように
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);

		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);

		// 当たり判定
		if (IsCollision(aabb1, aabb2))
		{
			color = RED;
		} 
		else 
		{
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		// AABB
		DrawAABB1(aabb1, worldViewProjectionMatrix, viewportMatrix, color);
		DrawAABB2(aabb2, worldViewProjectionMatrix, viewportMatrix, WHITE);

		ImGui::Begin("Window");
		
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);

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
