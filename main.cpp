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

struct Ball {
	Vector3 position; // ボールの位置
	Vector3 velocity; // ボールの速度
	float radius;     // ボールの半径
	uint32_t color;   // ボールの色
};

//スフィアを表示する
void DrawSphere(const Ball& ball, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	//分割数
	const uint32_t kSubdivision = 12;
	//経度分割一つ分の角度
	const float kLonEvery = (2 * (static_cast<float>(M_PI))) / kSubdivision;
	//緯度分割一つ分の角度
	const float kLatEvery = (static_cast<float>(M_PI)) / kSubdivision;

	//緯度の方向に分割　-π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -(static_cast<float>(M_PI)) / 2.0f + kLatEvery * latIndex;//現在の緯度

		//経度の方向に分割　0~2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//現在の経度

			//world座標系でのa,b,cを求める
			Vector3 a = { (ball.radius) * cosf(lat) * cosf(lon), (ball.radius) * sinf(lat), (ball.radius) * cosf(lat) * sin(lon) };
			Vector3 b = { (ball.radius) * cosf(lat + kLatEvery) * cosf(lon), (ball.radius) * sinf(lat + kLatEvery), (ball.radius) * cos(lat + kLatEvery) * sinf(lon) };
			Vector3 c = { (ball.radius) * cosf(lat) * cosf(lon + kLonEvery), (ball.radius) * sinf(lat), (ball.radius) * cosf(lat) * sinf(lon + kLonEvery) };

			Matrix4x4 worldMatrixA = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, ball.position);
			Matrix4x4 worldMatrixB = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, ball.position);
			Matrix4x4 worldMatrixC = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, ball.position);

			Matrix4x4 wvpMatrixA = MultiplyMatrix(worldMatrixA, viewProjectionMatrix);
			Matrix4x4 wvpMatrixB = MultiplyMatrix(worldMatrixB, viewProjectionMatrix);
			Matrix4x4 wvpMatrixC = MultiplyMatrix(worldMatrixC, viewProjectionMatrix);

			Vector3 localA = Transform(a, wvpMatrixA);
			Vector3 localB = Transform(b, wvpMatrixB);
			Vector3 localC = Transform(c, wvpMatrixC);

			Vector3 screenA = Transform(localA, viewportMatrix);
			Vector3 screenB = Transform(localB, viewportMatrix);
			Vector3 screenC = Transform(localC, viewportMatrix);

			// ab, bcで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
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

	const float deltaTime = 1.0f / 60.0f; // 固定タイム

	// 円運動のパラメータ
	float angularVelocity = static_cast<float>(M_PI);
	float radius = 0.8f;
	float currentAngle = 0.0f;
	Vector3 centerPosition = {};
	bool isAnimationActive = false;

	Ball ball;
	ball.position = { centerPosition.x + radius, centerPosition.y, centerPosition.z };
	ball.velocity = {};
	ball.radius = 0.05f;
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
			currentAngle += angularVelocity * deltaTime;
			if (currentAngle > 2.0f * (float)(M_PI)) {
				currentAngle -= 2.0f * (float)(M_PI);
			}
			ball.position.x = centerPosition.x + radius * std::cos(currentAngle);
			ball.position.y = centerPosition.y + radius * std::sin(currentAngle);
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawSphere(ball, worldViewProjectionMatrix, viewportMatrix, ball.color);

		ImGui::Begin("Window");
		
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		if (ImGui::Button("Start")) {
			isAnimationActive = true;
			currentAngle = 0.0f;
			ball.position = { centerPosition.x + radius, centerPosition.y, centerPosition.z };
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			isAnimationActive = false;
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
