#pragma once
#include "GameNode.h"
#include "Vector3.h"
#include "Quaternion.h"

class Camera : public GameNode {
public:
	size_t GameWidth { }, GameHeight { };

	float* ZBuffer { };
	unsigned long* Colors { };

	Vector3 Position { };
	Quaternion Rotation { };
	float FOV { };
	float NearClippingPlane { };
	float FarClippingPlane { };
	unsigned long ClearColor { 0x000000 };

	void GameStart() override;
	void GameRender() override;

	void Translate(Vector3 Movement);
	void Rotate(Vector3 Rotation);
};

