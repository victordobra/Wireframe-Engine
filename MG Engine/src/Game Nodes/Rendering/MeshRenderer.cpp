#include "MeshRenderer.h"

Matrix4x4 MeshRenderer::GetTransformMatrix() const {
	return Matrix4x4::Scaling(Scale) * Matrix4x4::Rotation(Rotation) * Matrix4x4::Translation(Position);
}