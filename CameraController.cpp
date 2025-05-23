#include"CameraController.h"

void CameraTranslateController(Vector3& cameraT, float cameraSpeed, char keys[256])
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