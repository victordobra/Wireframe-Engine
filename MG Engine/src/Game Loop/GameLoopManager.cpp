#include "GameLoopManager.h"
#include "GameLoopManagerInternal.h"
#include "OSManager.h"
#include "EngineMath.h"
#include "GameNode.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include <chrono>
#include <future>

#include <windows.h>

unsigned long*& ScreenColours = OSManager::GetScreenColoursRef();
static unsigned int ScreenWidth;
static unsigned int ScreenHeight;
static unsigned int GameWidth;
static unsigned int GameHeight;

static float DeltaTime = 0;

static bool KeysDown[256] = { false };
static bool KeysPressedCache[256] = { false };
static bool KeysPressed[256] = { false };
static bool KeysReleased[256] = { false };

Vector2 MousePos;

Camera* Cam; 
MeshRenderer* MeshR;

void GameLoopManager::Start() {
	ScreenWidth = OSManager::GetScreenWidth();
	ScreenHeight = OSManager::GetScreenHeight();
	GameWidth = OSManager::GetGameWidth();
	GameHeight = OSManager::GetGameHeight();

	Cam = new Camera();
	Cam->NearClippingPlane = 0.1f;
	Cam->FarClippingPlane = 1000;
	Cam->FOV = 60;
	MeshR = new MeshRenderer();
	Mesh* M = Mesh::FromObjFile("Cube.obj");
	MeshR->RendererMesh = M;
	MeshR->Position = { 0, 0, 5 };
	MeshR->Scale = { 1, 1, 1 };
	MeshR->Texture = (HBITMAP)LoadImage(NULL, L"Cube.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	for (int i = 0; i < GameNode::GameNodeCount; i++)
		GameNode::GameNodes[i]->GameStart();
	
}

void RunGameNodeFrame(size_t Index) {
	GameNode::GameNodes[Index]->GameFrame();
}
void RunGameNodeRender(size_t Index) {
	GameNode::GameNodes[Index]->GameRender();
}

float XRot = 0;
float YRot = 0;
void GameLoopManager::Update() {
	std::chrono::high_resolution_clock Timer{ };
	auto Start = Timer.now();

	if (KeysDown['W'])
		Cam->Translate({ 0, 0, 3 * DeltaTime });
	if (KeysDown['S'])
		Cam->Translate({ 0, 0, -3 * DeltaTime });
	if (KeysDown['A'])
		Cam->Translate({ -3 * DeltaTime, 0, 0 });
	if (KeysDown['D'])
		Cam->Translate({ 3 * DeltaTime, 0, 0 });
	if (KeysDown['E'])
		Cam->Translate({ 0, 3 * DeltaTime, 0 });
	if (KeysDown['Q'])
		Cam->Translate({ 0, -3 * DeltaTime, 0 });
	if (KeysDown[VK_UP])
		XRot += 90 * DeltaTime;
	if (KeysDown[VK_DOWN])
		XRot -= 90 * DeltaTime;
	if (KeysDown[VK_LEFT])
		YRot += 90 * DeltaTime;
	if (KeysDown[VK_RIGHT])
		YRot -= 90 * DeltaTime;

	Cam->Rotation = Quaternion::EulerAngles(XRot, YRot, 0);
	std::future<void>* Futures = new std::future<void>[GameNode::GameNodeCount];

	for (size_t i = 0; i < GameNode::GameNodeCount; i++)
		Futures[i] = std::async(std::launch::async | std::launch::deferred, RunGameNodeFrame, i);
	for (size_t i = 0; i < GameNode::GameNodeCount; i++)
		Futures[i].wait();

	for (size_t i = 0; i < GameNode::GameNodeCount; i++)
		Futures[i] = std::async(std::launch::async | std::launch::deferred, RunGameNodeRender, i);
	for (size_t i = 0; i < GameNode::GameNodeCount; i++)
		Futures[i].wait();

	delete[] Futures;

	memset(KeysPressed, 0, sizeof(bool) * 256);
	memset(KeysReleased, 0, sizeof(bool) * 256);

	auto End = Timer.now();
	DeltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() / 1000000;

	using namespace std::chrono_literals;

	std::this_thread::sleep_for(1000000us / 60 - DeltaTime * 1000000us);
	End = Timer.now();
	DeltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() / 1000000;
}

void GameLoopManager::OnKeyDown(int KeyCode) {
	KeysDown[KeyCode] = true;
	KeysPressed[KeyCode] = true;
}
void GameLoopManager::OnKeyUp(int KeyCode) {
	KeysDown[KeyCode] = false;
	KeysReleased[KeyCode] = true;
}
void GameLoopManager::SetMousePos(int NewX, int NewY) {
	MousePos = { (float)NewX, (float)NewY };
}

#pragma region External Functions

unsigned int GameLoopManager::GetScreenWidth() {
	return ScreenWidth;
}
unsigned int GameLoopManager::GetScreenHeight() {
	return ScreenHeight;
}
unsigned int GameLoopManager::GetGameWidth() {
	return GameWidth;
}
unsigned int GameLoopManager::GetGameHeight() {
	return GameHeight;
}
void GameLoopManager::SetGameWidth(unsigned int NewWidth) {
	GameWidth = NewWidth;
	OSManager::SetGameSize(GameWidth, GameHeight);
}
void GameLoopManager::SetGameHeight(unsigned int NewHeight) {
	GameHeight = NewHeight;
	OSManager::SetGameSize(GameWidth, GameHeight);
}
void GameLoopManager::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
	GameWidth = NewWidth;
	GameHeight = NewHeight;
	OSManager::SetGameSize(GameWidth, GameHeight);
}
float GameLoopManager::GetDeltaTime() {
	return DeltaTime;
}
bool GameLoopManager::IsKeyDown(int KeyCode) {
	return KeysDown[KeyCode];
}
bool GameLoopManager::IsKeyUp(int KeyCode) {
	return !KeysDown[KeyCode];
}
bool GameLoopManager::IsKeyPressed(int KeyCode) {
	return KeysPressed[KeyCode];
}
bool GameLoopManager::IsKeyReleased(int KeyCode) {
	return KeysReleased[KeyCode];
}

#pragma endregion