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

const char kWindowTitle[] = "LE2B_02_アサカワ_サクト";

int kWindowWidth = 1280;
int kWindowHeight = 720;

struct Spring {
	Vector3 anchor;
	float naturalLength;		// 自然長
	float stiffness;			// 剛性 バネ定数k
	float dampingCoefficient;	// 減衰係数
};

struct Ball {
	Vector3 position;			// ボールの位置
	Vector3 velocity;			// ボールの速度
	Vector3 acceleration;		// ボールの加速度
	float mass;					// ボールの質量
	float radius;				// ボールの半径
	unsigned int color;			// ボールの色
};

void UpdateSpring(Spring& spring, Ball& ball,float& len,Vector3& dif) {
	const float deltaTime = 1.0f / 60.0f;
	Vector3 diff = ball.position - spring.anchor;
	dif = diff;
	float length = diff.Length();
	len = length;
	if (length != 0.0f) {
		Vector3 direction = Normalize(diff);
		Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
		Vector3 displacement = length * (ball.position - restPosition);
		Vector3 restoringForce = -spring.stiffness * displacement;
		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
		Vector3 force = restoringForce + dampingForce;
		ball.acceleration = force / ball.mass;
	}
	// 加速度も速度もどちらも秒を基準とした値である
	// それが、1/60秒間(deltaTime)適用されたと考える
	ball.velocity += ball.acceleration * deltaTime;
	ball.position += ball.velocity * deltaTime;
}

void DrawSpring(Spring& spring, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 springScreenPos = MakeScreenToWorld(spring.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(springScreenPos.x), static_cast<int>(springScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		WHITE);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, color);
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

	Spring spring;
	spring.anchor = { 0.0f,0.0f,0.0f };
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball;
	ball.position = { 2.5f,0.0f,0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;
	ball.velocity = {};
	ball.acceleration = {};

	bool moveSpring = false;

	float len = 0.0f;
	Vector3 dif = {};

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

		if (moveSpring)
		{
			UpdateSpring(spring, ball, len, dif);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		// バネを描画
		DrawSpring(spring, ball, worldViewProjectionMatrix, viewportMatrix, BLUE);

		ImGui::Begin("Window");

		ImGui::Checkbox("Move Spring", &moveSpring);
		ImGui::DragFloat3("ballPos", &ball.position.x, 0.01f);

		ImGui::Text("CameraController");
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

		ImGui::Text("SpringAnchor,X:%f,Y:%f,Z:%f", spring.anchor.x, spring.anchor.y, spring.anchor.z);
		ImGui::Text("Updated Position: %.2f, %.2f, %.2f", ball.position.x, ball.position.y, ball.position.z);
		ImGui::Text("Updated Velocity: %.2f, %.2f, %.2f", ball.velocity.x, ball.velocity.y, ball.velocity.z);
		ImGui::Text("Updated Acceleration: %.2f, %.2f, %.2f", ball.acceleration.x, ball.acceleration.y, ball.acceleration.z);
		ImGui::Text("len:%f", len);
		ImGui::Text("dif:%f,%f,%f", dif.x, dif.y, dif.z);

		ImGui::End();

		if (cameraMode) {
			Novice::ScreenPrintf(20, 20, "CameraMode : Translate : X->(D or A) Y->(Q or E) Z->(W or S)");
		} else {
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
