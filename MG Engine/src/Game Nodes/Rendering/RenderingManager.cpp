#include "RenderingManager.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "EngineMath.h"
#include <istream>
#include <future>

void HandleLocalFace(Vector4* Positions, Vector3* UVCoor, Matrix4x4& ProjMatrix, unsigned long* TextureColors, size_t TextureWidth, size_t TextureHeight, size_t GameWidth, size_t GameHeight, Camera* This) {
	Vector4 ViewSpacePositions[3] = { ProjMatrix * Positions[0], ProjMatrix * Positions[1], ProjMatrix * Positions[2] };

	if (ViewSpacePositions[0].W < This->NearClippingPlane || ViewSpacePositions[1].W < This->NearClippingPlane || ViewSpacePositions[2].W < This->NearClippingPlane)
		return;

	for (size_t i = 0; i < 3; i++) {
		ViewSpacePositions[i].X /= ViewSpacePositions[i].W;
		ViewSpacePositions[i].Y /= ViewSpacePositions[i].W;
		ViewSpacePositions[i].Z /= ViewSpacePositions[i].W;

		UVCoor[i].X /= ViewSpacePositions[i].W;
		UVCoor[i].Y /= ViewSpacePositions[i].W;
		UVCoor[i].Z = 1.0f / ViewSpacePositions[i].W;
	}
	
	int ViewX[3] = { (int)((ViewSpacePositions[0].X + 1) / 2 * GameWidth), (int)((ViewSpacePositions[1].X + 1) / 2 * GameWidth), (int)((ViewSpacePositions[2].X + 1) / 2 * GameWidth) };
	int ViewY[3] = { (int)((ViewSpacePositions[0].Y + 1) / 2 * GameHeight), (int)((ViewSpacePositions[1].Y + 1) / 2 * GameHeight), (int)((ViewSpacePositions[2].Y + 1) / 2 * GameHeight) };
	float ViewZ[3] = { ViewSpacePositions[0].W, ViewSpacePositions[1].W, ViewSpacePositions[2].W };

	if (ViewY[0] > ViewY[1]) {
		std::swap(ViewX[0], ViewX[1]);
		std::swap(ViewY[0], ViewY[1]);
		std::swap(ViewZ[0], ViewZ[1]);
		std::swap(UVCoor[0], UVCoor[1]);
	}
	if (ViewY[1] > ViewY[2]) {
		std::swap(ViewX[1], ViewX[2]);
		std::swap(ViewY[1], ViewY[2]);
		std::swap(ViewZ[1], ViewZ[2]);
		std::swap(UVCoor[1], UVCoor[2]);
	}
	if (ViewY[0] > ViewY[1]) {
		std::swap(ViewX[0], ViewX[1]);
		std::swap(ViewY[0], ViewY[1]);
		std::swap(ViewZ[0], ViewZ[1]);
		std::swap(UVCoor[0], UVCoor[1]);
	}

	for (int i = ViewY[0]; i < ViewY[1]; i++) {
		if (i >= GameHeight || i < 0)
			continue;

		float SmallLerpFactor = (float)(i - ViewY[0]) / (ViewY[1] - ViewY[0]);
		float LargeLerpFactor = (float)(i - ViewY[0]) / (ViewY[2] - ViewY[0]);

		int SmallLerp = (int)Math::Lerp(ViewX[0], ViewX[1], SmallLerpFactor);
		int LargeLerp = (int)Math::Lerp(ViewX[0], ViewX[2], LargeLerpFactor);

		float ZSmallLerp = Math::Lerp(ViewZ[0], ViewZ[1], SmallLerpFactor);
		float ZLargeLerp = Math::Lerp(ViewZ[0], ViewZ[2], LargeLerpFactor);

		Vector3 UVSmallLerp = Vector3::Lerp(UVCoor[0], UVCoor[1], SmallLerpFactor);
		Vector3 UVLargeLerp = Vector3::Lerp(UVCoor[0], UVCoor[2], LargeLerpFactor);

		int LeftLerp, RightLerp;
		float ZLeftLerp, ZRightLerp;
		Vector3 UVLeftLerp, UVRightLerp;

		if (SmallLerp < LargeLerp) {
			LeftLerp = SmallLerp;
			RightLerp = LargeLerp;

			ZLeftLerp = ZSmallLerp;
			ZRightLerp = ZLargeLerp;

			UVLeftLerp = UVSmallLerp;
			UVRightLerp = UVLargeLerp;
		} else {
			LeftLerp = LargeLerp;
			RightLerp = SmallLerp;

			ZLeftLerp = ZLargeLerp;
			ZRightLerp = ZSmallLerp;

			UVLeftLerp = UVLargeLerp;
			UVRightLerp = UVSmallLerp;
		}
		if (LeftLerp == RightLerp) {
			if(LeftLerp < GameWidth && LeftLerp >= 0)
				if ((ZLeftLerp < This->ZBuffer[i * GameWidth + LeftLerp] || This->ZBuffer[i * GameWidth + LeftLerp] == -1) && ZLeftLerp > This->NearClippingPlane) {
					int UVCoorTextureX = (int)(UVLeftLerp.X / UVLeftLerp.Z * (TextureWidth - 1));
					int UVCoorTextureY = (int)(UVLeftLerp.Y / UVLeftLerp.Z * (TextureHeight - 1));

					UVCoorTextureX = UVCoorTextureX >= TextureWidth ? TextureWidth - 1 : UVCoorTextureX;
					UVCoorTextureX = UVCoorTextureX < 0 ? 0 : UVCoorTextureX;
					UVCoorTextureY = UVCoorTextureY >= TextureHeight ? TextureHeight - 1 : UVCoorTextureY;
					UVCoorTextureY = UVCoorTextureY < 0 ? 0 : UVCoorTextureY;

					This->Colors[i * GameWidth + LeftLerp] = TextureColors[UVCoorTextureY * TextureWidth + UVCoorTextureX];
					This->ZBuffer[i * GameWidth + LeftLerp] = ZLeftLerp;
				}
		} else
			for (int j = LeftLerp; j <= RightLerp; j++) {
				if (j >= GameWidth || j < 0)
					continue;

				float LerpFactor = ((float)j - (float)LeftLerp) / ((float)RightLerp - (float)LeftLerp);
				float ZValue = Math::Lerp(ZLeftLerp, ZRightLerp, LerpFactor);

				if ((ZValue < This->ZBuffer[i * GameWidth + j] || This->ZBuffer[i * GameWidth + j] == -1) && ZValue > This->NearClippingPlane) {
					Vector3 UVCoord = Vector3::Lerp(UVLeftLerp, UVRightLerp, LerpFactor);
					int UVCoorTextureX = (int)(UVCoord.X / UVCoord.Z * (TextureWidth - 1));
					int UVCoorTextureY = (int)(UVCoord.Y / UVCoord.Z * (TextureHeight - 1));

					UVCoorTextureX = UVCoorTextureX >= TextureWidth ? TextureWidth - 1 : UVCoorTextureX;
					UVCoorTextureX = UVCoorTextureX < 0 ? 0 : UVCoorTextureX;
					UVCoorTextureY = UVCoorTextureY >= TextureHeight ? TextureHeight - 1 : UVCoorTextureY;
					UVCoorTextureY = UVCoorTextureY < 0 ? 0 : UVCoorTextureY;

					This->Colors[i * GameWidth + j] = TextureColors[UVCoorTextureY * TextureWidth + UVCoorTextureX];
					This->ZBuffer[i * GameWidth + j] = ZValue;
				}
			}
	}

	for (int i = ViewY[1]; i < ViewY[2]; i++) {
		if (i >= GameHeight || i < 0)
			continue;

		float SmallLerpFactor = (float)(i - ViewY[1]) / (ViewY[2] - ViewY[1]);
		float LargeLerpFactor = (float)(i - ViewY[0]) / (ViewY[2] - ViewY[0]);

		int SmallLerp = (int)Math::Lerp(ViewX[1], ViewX[2], SmallLerpFactor);
		int LargeLerp = (int)Math::Lerp(ViewX[0], ViewX[2], LargeLerpFactor);

		float ZSmallLerp = Math::Lerp(ViewZ[1], ViewZ[2], SmallLerpFactor);
		float ZLargeLerp = Math::Lerp(ViewZ[0], ViewZ[2], LargeLerpFactor);

		Vector3 UVSmallLerp = Vector3::Lerp(UVCoor[1], UVCoor[2], SmallLerpFactor);
		Vector3 UVLargeLerp = Vector3::Lerp(UVCoor[0], UVCoor[2], LargeLerpFactor);

		int LeftLerp, RightLerp;
		float ZLeftLerp, ZRightLerp;
		Vector3 UVLeftLerp, UVRightLerp;

		if (SmallLerp < LargeLerp) {
			LeftLerp = SmallLerp;
			RightLerp = LargeLerp;

			ZLeftLerp = ZSmallLerp;
			ZRightLerp = ZLargeLerp;

			UVLeftLerp = UVSmallLerp;
			UVRightLerp = UVLargeLerp;
		} else {
			LeftLerp = LargeLerp;
			RightLerp = SmallLerp;

			ZLeftLerp = ZLargeLerp;
			ZRightLerp = ZSmallLerp;

			UVLeftLerp = UVLargeLerp;
			UVRightLerp = UVSmallLerp;
		}
		if (LeftLerp == RightLerp) {
			if ((ZLeftLerp < This->ZBuffer[i * GameWidth + LeftLerp] || This->ZBuffer[i * GameWidth + LeftLerp] == -1) && ZLeftLerp > This->NearClippingPlane) {
				int UVCoorTextureX = (int)(UVLeftLerp.X / UVLeftLerp.Z * (TextureWidth - 1));
				int UVCoorTextureY = (int)(UVLeftLerp.Y / UVLeftLerp.Z * (TextureHeight - 1));

				UVCoorTextureX = UVCoorTextureX >= TextureWidth ? TextureWidth - 1 : UVCoorTextureX;
				UVCoorTextureX = UVCoorTextureX < 0 ? 0 : UVCoorTextureX;
				UVCoorTextureY = UVCoorTextureY >= TextureHeight ? TextureHeight - 1 : UVCoorTextureY;
				UVCoorTextureY = UVCoorTextureY < 0 ? 0 : UVCoorTextureY;

				This->Colors[i * GameWidth + LeftLerp] = TextureColors[UVCoorTextureY * TextureWidth + UVCoorTextureX];
				This->ZBuffer[i * GameWidth + LeftLerp] = ZLeftLerp;
			}
		}
		else
			for (int j = LeftLerp; j <= RightLerp; j++) {
				if (j >= GameWidth || j < 0)
					continue;

				float LerpFactor = ((float)j - (float)LeftLerp) / ((float)RightLerp - (float)LeftLerp);
				float ZValue = Math::Lerp(ZLeftLerp, ZRightLerp, LerpFactor);

				if ((ZValue < This->ZBuffer[i * GameWidth + j] || This->ZBuffer[i * GameWidth + j] == -1) && ZValue > This->NearClippingPlane) {
					Vector3 UVCoord = Vector3::Lerp(UVLeftLerp, UVRightLerp, LerpFactor);
					int UVCoorTextureX = (int)(UVCoord.X / UVCoord.Z * (TextureWidth - 1));
					int UVCoorTextureY = (int)(UVCoord.Y / UVCoord.Z * (TextureHeight - 1));

					UVCoorTextureX = UVCoorTextureX >= TextureWidth ? TextureWidth - 1 : UVCoorTextureX;
					UVCoorTextureX = UVCoorTextureX < 0 ? 0 : UVCoorTextureX;
					UVCoorTextureY = UVCoorTextureY >= TextureHeight ? TextureHeight - 1 : UVCoorTextureY;
					UVCoorTextureY = UVCoorTextureY < 0 ? 0 : UVCoorTextureY;

					This->Colors[i * GameWidth + j] = TextureColors[UVCoorTextureY * TextureWidth + UVCoorTextureX];
					This->ZBuffer[i * GameWidth + j] = ZValue;
				}
			}
	}

}
void RenderingManager::RenderFace(MeshRenderer* MeshR, Vector3** Positions, Vector2** UVCoordinates, Vector3* Normal, unsigned long* TextureColors, size_t TextureWidth, size_t TextureHeight, Matrix4x4& GlobMatrix, Matrix4x4& LocalMatrix, Matrix4x4& ProjMatrix, Camera* This) {
	if (Vector3::Dot(*Normal, MeshR->Position - This->Position) > 0)
		return;
	
	size_t GameWidth = This->GameWidth, GameHeight = This->GameHeight;

	Vector4 WorldSpacePositions[3] = { GlobMatrix * *Positions[0], GlobMatrix * *Positions[1], GlobMatrix * *Positions[2] };
	WorldSpacePositions[0].W = 1;
	WorldSpacePositions[1].W = 1;
	WorldSpacePositions[2].W = 1;

	Vector4 LocalSpacePositions[3] = { LocalMatrix * WorldSpacePositions[0], LocalMatrix * WorldSpacePositions[1], LocalMatrix * WorldSpacePositions[2] };
	Vector3 UVCoor[3] = { *UVCoordinates[0], *UVCoordinates[1], *UVCoordinates[2] };
	
	HandleLocalFace(LocalSpacePositions, UVCoor, ProjMatrix, TextureColors, TextureWidth, TextureHeight, GameWidth, GameHeight, This);

	delete[] Positions;
	delete[] UVCoordinates;
}