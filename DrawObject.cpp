#include "DrawObject.h"

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