#pragma once
#include"Vector3.h"
#include"Novice.h"

void CameraTranslateController(Vector3& cameraT, float cameraSpeed, char keys[256]);

void CameraRotateController(Vector3& cameraR, float cameraSpeed, char keys[256]);

void CameraControllerManager(bool& cameraMode, Vector3& cameraT, Vector3& cameraR, float cameraTlanslateSpeed, float cameraRotateSpeed, char keys[256], char preKeys[256]);