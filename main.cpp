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
	Vector3 min;// 最小点
	Vector3 max;// 最大点
};

// AABB1の描画
void DrawAABB(const AABB& aabb1, Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// aabb1
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
	// スクリーン座標系まで変換をかける

	// aabb1
	Matrix4x4 worldMatrix1[8];
	Matrix4x4 worldViewProjectionMatrix1[8];
	Vector3 ndcVertex1[8];
	Vector3 screenVertices1[8];


	for (int i = 0; i < 8; i++)
	{
		worldMatrix1[i] = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, vertices1[i]);

		// WVPMatrix
		worldViewProjectionMatrix1[i] = MultiplyMatrix(worldMatrix1[i], viewProjectionMatrix);

		// NDC(正規化デバイス座標系)
		ndcVertex1[i] = Transform(Vector3{}, worldViewProjectionMatrix1[i]);

		// スクリーン座標へ変換
		screenVertices1[i] = Transform(ndcVertex1[i], viewportMatrix);
	}

	// aabb1
	// 下辺
	Novice::DrawLine((int)screenVertices1[0].x, (int)screenVertices1[0].y, (int)screenVertices1[1].x, (int)screenVertices1[1].y, color);
	Novice::DrawLine((int)screenVertices1[1].x, (int)screenVertices1[1].y, (int)screenVertices1[2].x, (int)screenVertices1[2].y, color);
	Novice::DrawLine((int)screenVertices1[2].x, (int)screenVertices1[2].y, (int)screenVertices1[3].x, (int)screenVertices1[3].y, color);
	Novice::DrawLine((int)screenVertices1[3].x, (int)screenVertices1[3].y, (int)screenVertices1[0].x, (int)screenVertices1[0].y, color);
	// 上辺
	Novice::DrawLine((int)screenVertices1[4].x, (int)screenVertices1[4].y, (int)screenVertices1[5].x, (int)screenVertices1[5].y, color);
	Novice::DrawLine((int)screenVertices1[5].x, (int)screenVertices1[5].y, (int)screenVertices1[6].x, (int)screenVertices1[6].y, color);
	Novice::DrawLine((int)screenVertices1[6].x, (int)screenVertices1[6].y, (int)screenVertices1[7].x, (int)screenVertices1[7].y, color);
	Novice::DrawLine((int)screenVertices1[7].x, (int)screenVertices1[7].y, (int)screenVertices1[4].x, (int)screenVertices1[4].y, color);
	// 側辺
	Novice::DrawLine((int)screenVertices1[1].x, (int)screenVertices1[1].y, (int)screenVertices1[5].x, (int)screenVertices1[5].y, color);
	Novice::DrawLine((int)screenVertices1[2].x, (int)screenVertices1[2].y, (int)screenVertices1[6].x, (int)screenVertices1[6].y, color);
	Novice::DrawLine((int)screenVertices1[3].x, (int)screenVertices1[3].y, (int)screenVertices1[7].x, (int)screenVertices1[7].y, color);
	Novice::DrawLine((int)screenVertices1[0].x, (int)screenVertices1[0].y, (int)screenVertices1[4].x, (int)screenVertices1[4].y, color);
}

bool IsCollision(const AABB& aabb1, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x,aabb1.min.x,aabb1.max.x),
	std::clamp(sphere.center.y,aabb1.min.y,aabb1.max.y),
	std::clamp(sphere.center.z,aabb1.min.z,aabb1.max.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));

	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius)
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

	float cameraTranslateSpeed = 0.03f;
	float cameraRotateSpeed = 0.01f;
	bool cameraMode = true;

	Sphere sphere = {
		{0.0f,2.0f,0.0f},
		1.0f,
	};

	AABB aabb = {
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.5f,0.5f,0.5f},
	};

	int color;

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
		aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
		aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
		aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
		aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
		aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
		aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

		// 当たり判定
		if (IsCollision(aabb, sphere))
		{
			// あたってたら赤色になる
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
		
		// スフィア
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, WHITE);

		// AABB
		DrawAABB(aabb, worldViewProjectionMatrix, viewportMatrix, color);

		ImGui::Begin("Window");
		
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		ImGui::DragFloat3("Sphere Center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere Radius", &sphere.radius, 0.01f);

		ImGui::DragFloat3("aabb max", &aabb.max.x, 0.01f);
		ImGui::DragFloat3("aabb min", &aabb.min.x, 0.01f);

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
