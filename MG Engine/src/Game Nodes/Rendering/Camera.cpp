#include "Camera.h"
#include "Matrix4x4.h"
#include "OSManager.h"
#include "EngineMath.h"
#include "MeshRenderer.h"
#include "RenderingManager.h"
#include <future>
#include <windows.h>

void ProcessObject(GameNode* Obj, Matrix4x4& LocalMatrix, Matrix4x4& ProjMatrix, Camera* This) {
	if (dynamic_cast<MeshRenderer*>(Obj) == nullptr)
		return;

	//Get the transform matrix of the mesh
	MeshRenderer* MeshR = (MeshRenderer*)Obj;
	Matrix4x4 TransformMatrix = MeshR->GetTransformMatrix();

	size_t FaceC = MeshR->RendererMesh->FaceC;
	std::future<void>* Futures = new std::future<void>[FaceC];

	BITMAP Bmp { };
	GetObject(MeshR->Texture, sizeof(BITMAP), &Bmp);
	size_t TextureWidth = Bmp.bmWidth;
	size_t TextureHeight = Bmp.bmHeight;
	unsigned long* TextureColors = new unsigned long[TextureWidth * TextureHeight];
	GetBitmapBits(MeshR->Texture, (long)(TextureWidth * TextureHeight * sizeof(unsigned long)), TextureColors);
	Mesh* RMesh = MeshR->RendererMesh;

	for (size_t i = 0; i < FaceC; i++) {
		Vector3** Positions = new Vector3*[3]{ RMesh->Positions + RMesh->Faces[i].PositionIndices[0], RMesh->Positions + RMesh->Faces[i].PositionIndices[1], RMesh->Positions + RMesh->Faces[i].PositionIndices[2] };
		Vector2** UVCoordinates = new Vector2*[3]{ RMesh->UVCoordinates + RMesh->Faces[i].UVCoordinateIndices[0], RMesh->UVCoordinates + RMesh->Faces[i].UVCoordinateIndices[1], RMesh->UVCoordinates + RMesh->Faces[i].UVCoordinateIndices[2] };
		Vector3* Normal = RMesh->Normals + RMesh->Faces[i].NormalIndex;

		Futures[i] = std::async(std::launch::async | std::launch::deferred, RenderingManager::RenderFace, MeshR, Positions, UVCoordinates, Normal, TextureColors, TextureWidth, TextureHeight, std::ref(TransformMatrix), std::ref(LocalMatrix), std::ref(ProjMatrix), This);
	}

	for (size_t i = 0; i < FaceC; i++)
		Futures[i].wait();

	delete[] Futures;
	delete[] TextureColors;
}

void Camera::GameStart() {
	//Set up the Z Buffer
	GameWidth = OSManager::GetGameWidth();
	GameHeight = OSManager::GetGameHeight();

	ZBuffer = new float[(size_t)GameWidth * GameHeight];
	Colors = new unsigned long[(size_t)GameWidth * GameHeight]{ 0 };

	for (int i = 0; i < GameWidth * GameHeight; i++)
		ZBuffer[i] = -1;
}

void Camera::GameRender() {
	//Calculate the localization matrix
	Matrix4x4 LocalizationMatrix = Matrix4x4::Translation(-Position) * Matrix4x4::Rotation(Rotation.Inverted());

	//Calculate the projection matrix
	float AspectRatio = (float)OSManager::GetGameWidth() / (float)OSManager::GetGameHeight();
	float InvFOVTan = 1 / Math::Tan(Math::DegToRad * FOV / 2);
	float ClippingPlaneFraction = FarClippingPlane / (FarClippingPlane - NearClippingPlane);

	Matrix4x4 ProjectionMatrix;
	ProjectionMatrix(0, 0) =  InvFOVTan / AspectRatio;
	ProjectionMatrix(1, 1) = InvFOVTan;
	ProjectionMatrix(2, 2) = ClippingPlaneFraction;
	ProjectionMatrix(2, 3) = -NearClippingPlane * ClippingPlaneFraction;
	ProjectionMatrix(3, 2) = 1;
	ProjectionMatrix(3, 3) = 0;

	//Reset the Z Buffer
	for (size_t i = 0; i < (size_t)GameWidth * GameHeight; i++)
		ZBuffer[i] = -1;

	for (size_t i = 0; i < (size_t)GameWidth * GameHeight; i++)
		Colors[i] = ClearColor;

	//Loop through every object
	std::future<void>* Futures = new std::future<void>[GameNode::GameNodeCount];

	for (size_t i = 0; i < GameNode::GameNodeCount; i++)
		Futures[i] = std::async(std::launch::async | std::launch::deferred, ProcessObject, GameNode::GameNodes[i], std::ref(LocalizationMatrix), std::ref(ProjectionMatrix), this);

	for (size_t i = 0; i < GameNode::GameNodeCount; i++)
		Futures[i].wait();
	
	delete[] Futures;

	memcpy(OSManager::GetScreenColoursRef(), Colors, (size_t)GameWidth * GameHeight * sizeof(unsigned long));
}

void Camera::Translate(Vector3 Movement) {
	Position += Matrix4x4::Rotation(Rotation) * Movement;
}
void Camera::Rotate(Vector3 Rotation) {
	this->Rotation *= Quaternion::EulerAngles(Rotation);
}