#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include"Matrix4x4Function.h"
#include <imgui.h>

const char kWindowTitle[] = "LE2B_02_アサカワ_サクト";

int kWindowWidth = 1280;
int kWindowHeight = 720;

struct Sphere {
	Vector3 center;//中心点
	float radius;//半径
};

//クロス積（ベクトル積）
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x + kColumnWidth * 5, y, "%s", label);
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

//グリッドを表示する
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//一つ分の長さ
	//奥から手前への線を徐々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 start(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, -kGridHalfWidth);
		Vector3 end(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, kGridHalfWidth);

		// スクリーン座標系まで変換
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, start);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, end);

		Matrix4x4 startWvpMatrix = MultiplyMatrix(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endWvpMatrix = MultiplyMatrix(endWorldMatrix, viewProjectionMatrix);

		Vector3 startLocal = Transform(start, startWvpMatrix);
		Vector3 endLocal = Transform(end, endWvpMatrix);

		Vector3 startScreen = Transform(startLocal, viewportMatrix);
		Vector3 endScreen = Transform(endLocal, viewportMatrix);

		//変換した座標を使って表示。色は薄い灰色。（0xAAAAAAFF）、原点は黒ぐらい
		Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), 0xAAAAAAFF);
	}

	//左から右もおなじように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		//奥から手前が左右に変わるだけ

		Vector3 start(-kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);
		Vector3 end(kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);

		// スクリーン座標系まで変換
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, start);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, end);

		Matrix4x4 startWvpMatrix = MultiplyMatrix(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endWvpMatrix = MultiplyMatrix(endWorldMatrix, viewProjectionMatrix);

		Vector3 startLocal = Transform(start, startWvpMatrix);
		Vector3 endLocal = Transform(end, endWvpMatrix);

		Vector3 startScreen = Transform(startLocal, viewportMatrix);
		Vector3 endScreen = Transform(endLocal, viewportMatrix);

		Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), 0xAAAAAAFF);
	}

}

//スフィアを表示する
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
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
			Vector3 a = { (sphere.radius) * cosf(lat) * cosf(lon), (sphere.radius) * sinf(lat), (sphere.radius) * cosf(lat) * sin(lon) };
			Vector3 b = { (sphere.radius) * cosf(lat + kLatEvery) * cosf(lon), (sphere.radius) * sinf(lat + kLatEvery), (sphere.radius) * cos(lat + kLatEvery) * sinf(lon) };
			Vector3 c = { (sphere.radius) * cosf(lat) * cosf(lon + kLonEvery), (sphere.radius) * sinf(lat), (sphere.radius) * cosf(lat) * sinf(lon + kLonEvery) };

			Matrix4x4 worldMatrixA = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere.center);
			Matrix4x4 worldMatrixB = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere.center);
			Matrix4x4 worldMatrixC = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, sphere.center);

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

	Sphere sphere{
		{0.f,0.f,0.f},
		1.f,
	};

	Vector3 rotate = {};
	Vector3 translate = {};

	Vector3 cameraTranslate = { 0.f,2.f,-6.49f };
	Vector3 cameraRotate = { 0.3f,0.f,0.f };

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		//各種行列の計算
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = InverseMatrix(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		//WVPMatrixを作る
		Matrix4x4 worldViewProjectionMatrix = MultiplyMatrix(viewMatrix, projectionMatrix);
		//ViewportMatrixを作る
		Matrix4x4 viewportMatrix = MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, RED);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
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
