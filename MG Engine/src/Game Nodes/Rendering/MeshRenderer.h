#pragma once
#include "GameNode.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include <windows.h>

class MeshRenderer : public GameNode {
public:
	Mesh* RendererMesh { };
	Vector3 Position { };
	Quaternion Rotation { };
	Vector3 Scale { };
	HBITMAP Texture { };

	Matrix4x4 GetTransformMatrix() const;
};

