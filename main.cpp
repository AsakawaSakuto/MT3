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

// 三角形
struct Triangle
{
	Vector3 vertex[3];
};

// 衝突判定
bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	// distance を求める
	// 1.ベクトル v1,v2 を求める
	Vector3 v1 = Subtract(triangle.vertex[1], triangle.vertex[0]);
	Vector3 v2 = Subtract(triangle.vertex[2], triangle.vertex[1]);

	// 2.クロス積 v1×v2 により法線 n を算出
	Vector3 normal = Cross(v1, v2);

	// 3.正規化
	Vector3 normalNormalize = Normalize(normal);

	// 4.平面上のどれか1点を使い、内積で distance を求める
	float distance = Dot(triangle.vertex[0], normalNormalize);

	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(normalNormalize, segment.diff);

	// 媒介変数 t を求める
	float t = (distance - Dot(segment.origin, normalNormalize)) / dot;

	// 衝突点 p を求める
	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

	// 各辺を結んだベクトルを求める
	Vector3 v01 = Subtract(triangle.vertex[1], triangle.vertex[0]);
	Vector3 v12 = Subtract(triangle.vertex[2], triangle.vertex[1]);
	Vector3 v20 = Subtract(triangle.vertex[0], triangle.vertex[2]);

	// 頂点と衝突点 p を結んだベクトルを求める
	Vector3 v0p = Subtract(p, triangle.vertex[0]);
	Vector3 v1p = Subtract(p, triangle.vertex[1]);
	Vector3 v2p = Subtract(p, triangle.vertex[2]);

	// 各辺を結んだベクトルと、頂点と衝突点 p を結んだベクトルのクロス積を求める
	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	// 全ての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, normalNormalize) >= 0.0f &&
		Dot(cross12, normalNormalize) >= 0.0f &&
		Dot(cross20, normalNormalize) >= 0.0f)
	{
		return true;
	} else
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

	// 三角形
	Triangle triangle = {
	{{ 0.0f,0.8f,1.0f },
	{ 1.4f,-0.8f,1.0f },
	{ -1.4f,-0.8f,1.0f }}
	};

	// 線
	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	// 色
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

		Matrix4x4 worldMatrix = MakeAffineMatrix(cameraScale, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = InverseMatrix(worldMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 1.0f, 0.0f);

		Matrix4x4 worldViewProjectionMatrix = MultiplyMatrix(viewMatrix, projectionMatrix);

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// 正射影ベクトル
		Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
		// 最近接点
		Vector3 closestPoint = ClosestPoint(point, segment);

		// 線の始点終点
		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldViewProjectionMatrix), viewportMatrix);

		// Screen空間へと頂点を変換する
		Vector3 screenTriangle[3];

		for (uint32_t i = 0; i < 3; ++i)
		{
			// 正規化デバイス座標系まで変換
			// Transformで同次座標系->デカルト座標

			Vector3 ndcVertex = Transform(triangle.vertex[i], worldViewProjectionMatrix);

			// Viewport変換を行ってScreen空間に変換
			screenTriangle[i] = Transform(ndcVertex, viewportMatrix);
		}

		// 当たり判定
		if (IsCollision(triangle, segment))
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

		// 線
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);

		// 三角形
		Novice::DrawTriangle(
			int(screenTriangle[0].x), int(screenTriangle[0].y),
			int(screenTriangle[1].x), int(screenTriangle[1].y),
			int(screenTriangle[2].x), int(screenTriangle[2].y),
			WHITE, kFillModeWireFrame);

		ImGui::Begin("Window");
		
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);

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
