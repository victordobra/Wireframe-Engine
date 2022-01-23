#pragma once
#include "GameNode.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "OSManager.h"

namespace mge {
	enum class ClearMode {
		NONE,
		COLOR
	};

	class Camera : public GameNode {
	public:
		Vector3 position;
		Quaternion rotation;

		Vector3 clearColor;
		ClearMode clearMode;
		float fov;
		float zNear;
		float zFar;

		void GameStart() override { aspectRatio = (float)OSMGetGameWidth() / OSMGetGameHeight(); }
		void GameRender() override;
	private:
		float aspectRatio;

		Matrix4x4 GetCameraMatrix();
	};
}