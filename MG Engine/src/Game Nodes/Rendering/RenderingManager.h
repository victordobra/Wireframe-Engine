#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

class MeshRenderer;
class Camera;

namespace RenderingManager {
	void RenderFace(MeshRenderer* MeshR, Vector3** Positions, Vector2** UVCoordinates, Vector3* Normal, unsigned long* TextureColors, size_t TextureWidth, size_t TextureHeight, Matrix4x4& GlobMatrix, Matrix4x4& LocalMatrix, Matrix4x4& ProjMatrix, Camera* This);
}