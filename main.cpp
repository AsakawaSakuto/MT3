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

// 球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane) {
	// 単位ベクトルを求める
	Vector3 unit;
	unit = Normalize(plane.normal);

	Vector3 center = Multiply(plane.distance, plane.normal); // 任意の点
	float d = Dot(center, unit); // 原点からの距離
	// shereとplaneの距離
	float k = Dot(unit, sphere.center) - d;
	float dist = k * k; // 絶対値をとる

	if (dist <= sphere.radius * sphere.radius) {
		return true;
	} else {
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

	Vector3 scale{ 1.0f, 1.0f, 1.0f };
	Vector3 rotate{ 0.0f, 0.0f, 0.0f };
	Vector3 translate{ 0.0f, 0.0f, 0.0f };
	Vector3 cameraScale{ 1.0f, 1.0f, 1.0f };
	Vector3 cameraRotate{ 0.f, 0.f, 0.f };
	Vector3 cameraTranslate{ 0.0f, 1.f, -10.0f };

	Sphere sphere;
	sphere.center = { 0.0f, 1.0f, 0.0f };
	sphere.radius = 0.5f;

	// 平面上の点
	Vector3 planePointA = { 0.1f, 0.2f, 0.1f };
	Vector3 planePointB = { 0.0f, 0.2f, 0.1f };
	Vector3 planePointC = { 0.1f, 0.2f, 0.0f };

	Vector3 v1 = Subtract(planePointB, planePointA); // b-a
	Vector3 v2 = Subtract(planePointC, planePointB); // c-b
	// 法線
	Vector3 normal = Normalize(Cross(v1, v2));

	Plane plane;
	plane.normal = normal;
	plane.distance = Dot(planePointA, plane.normal);

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

		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = InverseMatrix(cameraMatrix);
		Matrix4x4 projectionMatrix =
			MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		// WVPMatrix
		Matrix4x4 worldViewProjectionMatrix =
			MultiplyMatrix(worldMatrix, MultiplyMatrix(viewMatrix, projectionMatrix));
		// ViewPortMatrix
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (IsCollision(sphere, plane)) {
			color = RED;
		} else {
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		// 平面
		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, WHITE);

		// 球
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, color);

		ImGui::Begin("Window");
		
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("Sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere.radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("Plane.normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane.distance", &plane.distance, 0.01f);

		ImGui::End();

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
