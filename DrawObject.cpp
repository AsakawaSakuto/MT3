#include "DrawObject.h"

//�O���b�h��\������
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	const float kGridHalfWidth = 2.0f;//Grid�̔����̕�
	const uint32_t kSubdivision = 10;//������
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//����̒���
	//�������O�ւ̐������X�Ɉ����Ă���
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//��̏����g���ă��[���h���W�n��̎n�_�ƏI�_�����߂�
		Vector3 start(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, -kGridHalfWidth);
		Vector3 end(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, kGridHalfWidth);

		// �X�N���[�����W�n�܂ŕϊ�
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, start);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, end);

		Matrix4x4 startWvpMatrix = MultiplyMatrix(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endWvpMatrix = MultiplyMatrix(endWorldMatrix, viewProjectionMatrix);

		Vector3 startLocal = Transform(start, startWvpMatrix);
		Vector3 endLocal = Transform(end, endWvpMatrix);

		Vector3 startScreen = Transform(startLocal, viewportMatrix);
		Vector3 endScreen = Transform(endLocal, viewportMatrix);

		//�ϊ��������W���g���ĕ\���B�F�͔����D�F�B�i0xAAAAAAFF�j�A���_�͍����炢
		Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), 0xAAAAAAFF);
	}

	//������E�����Ȃ��悤�ɏ��X�Ɉ����Ă���
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		//�������O�����E�ɕς�邾��

		Vector3 start(-kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);
		Vector3 end(kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);

		// �X�N���[�����W�n�܂ŕϊ�
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

//�X�t�B�A��\������
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	//������
	const uint32_t kSubdivision = 12;
	//�o�x��������̊p�x
	const float kLonEvery = (2 * (static_cast<float>(M_PI))) / kSubdivision;
	//�ܓx��������̊p�x
	const float kLatEvery = (static_cast<float>(M_PI)) / kSubdivision;



	//�ܓx�̕����ɕ����@-��/2 ~ ��/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -(static_cast<float>(M_PI)) / 2.0f + kLatEvery * latIndex;//���݂̈ܓx

		//�o�x�̕����ɕ����@0~2��
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//���݂̌o�x

			//world���W�n�ł�a,b,c�����߂�
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

			// ab, bc�Ő�������
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}