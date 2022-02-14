#include "GameLoopManager.h"
#include "GameLoopManagerInternal.h"
#include "OSManager.h"
#include "EngineMath.h"
#include "GameNode.h"
#include "Vector2.h"
#include "MeshRenderer.h"
#include "Camera.h"

#include <chrono>
#include <future>
#include <windows.h>

namespace mge {
	static size_t screenWidth;
	static size_t screenHeight;
	static size_t gameWidth;
	static size_t gameHeight;

	static float deltaTime = 0;

	static bool keysDown[256] = { false };
	static bool keysPressedCache[256] = { false };
	static bool keysPressed[256] = { false };
	static bool keysReleased[256] = { false };
	static Vector2 mousePos;
	
	MeshRenderer* meshRenderer;
	Camera* camera;
	void GLMStart() {
		//Set the screen width and game width variables
		screenWidth = OSMGetScreenWidth();
		screenHeight = OSMGetScreenHeight();
		gameWidth = OSMGetGameWidth();
		gameHeight = OSMGetGameHeight();

		meshRenderer = new MeshRenderer();
		meshRenderer->position = { 0.0f, -2.0f, 0.0f };
		meshRenderer->color = { 1.0f, 1.0f, 1.0f };
		meshRenderer->scale = { 1.0f, 1.0f, 1.0f };

		AssetLoadResult result{};
		Mesh* mesh = new Mesh("Assets\\Teapot.obj", result);

		if (result != AssetLoadResult::SUCCESS)
			throw std::runtime_error("Error occured while reading mesh!");

		meshRenderer->SetMesh(mesh);

		camera = new Camera();
		camera->position = { 0.0f, 0.0f, 7.0f };
		camera->clearColor = { 0.01f, 0.01f, 0.01f };
		camera->clearMode = ClearMode::COLOR;

		//Run every game start function
		for (size_t i = 0; i < GameNode::gameNodeCount; i++)
			GameNode::gameNodes[i]->GameStart();
	}

	static void RunGameNodeFrame(size_t index) {
		GameNode::gameNodes[index]->GameFrame();
	}
	static void RunGameNodeRender(size_t index) {
		GameNode::gameNodes[index]->GameRender();
	}

	static float rot;
	static size_t frameIndex = 0;
	static Vector3 eulerRotation{};

	void GLMUpdate() {
		//Start a timer to calculate delta time
		std::chrono::high_resolution_clock timer{ };
		auto start = timer.now();

		//Create an array of futures
		std::future<void>* futures = new std::future<void>[GameNode::gameNodeCount];

		//rot += 90.0f * deltaTime;
		//if (rot > 180.0f)
		//	rot -= 360.0f;

		meshRenderer->rotation = Quaternion::EulerAngles({ 0.f, 45.f * DEG_TO_RAD_MULTIPLIER, 0.f });

		const float speed = 3.0f;
		const float rotSpeed = 90.0f;

		if (keysDown['W'])
			camera->Translate({ 0.0f, 0.0f, -speed * deltaTime });
		if (keysDown['S'])
			camera->Translate({ 0.0f, 0.0f, speed * deltaTime });
		if (keysDown['A'])
			camera->Translate({ -speed * deltaTime, 0.0f, 0.0f });
		if (keysDown['D'])
			camera->Translate({ speed * deltaTime, 0.0f, 0.0f });
		if (keysDown['E'])
			camera->Translate({ 0.0f, speed * deltaTime, 0.0f });
		if (keysDown['Q'])
			camera->Translate({ 0.0f, -speed * deltaTime, 0.0f });

		if (keysDown[VK_UP])
			eulerRotation.x -= rotSpeed * deltaTime;
		if (keysDown[VK_DOWN])
			eulerRotation.x += rotSpeed * deltaTime;
		if (keysDown[VK_LEFT])
			eulerRotation.y -= rotSpeed * deltaTime;
		if (keysDown[VK_RIGHT])
			eulerRotation.y += rotSpeed * deltaTime;

		if (eulerRotation.x > 360.0f)
			eulerRotation.x -= 360.0f;
		if (eulerRotation.y > 360.0f)
			eulerRotation.y -= 360.0f;

		camera->rotation = Quaternion::EulerAngles(eulerRotation * DEG_TO_RAD_MULTIPLIER);

		//Assign to each future the async of running every GameFrame function
		for (size_t i = 0; i < GameNode::gameNodeCount; i++)
			futures[i] = std::async(std::launch::async | std::launch::deferred, RunGameNodeFrame, i);
		for (size_t i = 0; i < GameNode::gameNodeCount; i++)
			futures[i].wait();

		//Do the same for GameRender 
		for (size_t i = 0; i < GameNode::gameNodeCount; i++)
			futures[i] = std::async(std::launch::async | std::launch::deferred, RunGameNodeRender, i);
		for (size_t i = 0; i < GameNode::gameNodeCount; i++)
			futures[i].wait();

		delete[] futures;

		//Reset all of the keys pressed and released
		memset(keysPressed, 0, sizeof(bool) * 256);
		memset(keysReleased, 0, sizeof(bool) * 256);

		//Stop the timer and measure the time
		auto end = timer.now();
		deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.f;
		frameIndex++;
	}

	void GLMOnKeyDown(int keyCode) {
		keysDown[keyCode] = true;
		keysPressed[keyCode] = true;
	}
	void GLMOnKeyUp(int keyCode) {
		keysDown[keyCode] = false;
		keysReleased[keyCode] = true;
	}
	void GLMSetMousePos(size_t newX, size_t newY) {
		mousePos = { (float)newX, (float)newY };
	}

	size_t GLMGetScreenWidth() {
		return screenWidth;
	}
	size_t GLMGetScreenHeight() {
		return screenHeight;
	}
	size_t GLMGetGameWidth() {
		return gameWidth;
	}
	size_t GLMGetGameHeight() {
		return gameHeight;
	}
	void GLMSetGameWidth(size_t newWidth) {
		gameWidth = newWidth;
		OSMSetGameSize(gameWidth, gameHeight);
	}
	void GLMSetGameHeight(size_t newHeight) {
		gameHeight = newHeight;
		OSMSetGameSize(gameWidth, gameHeight);
	}
	void GLMSetGameSize(size_t newWidth, size_t newHeight) {
		gameWidth = newWidth;
		gameHeight = newHeight;
		OSMSetGameSize(gameWidth, gameHeight);
	}
	
	float GLMGetDeltaTime() {
		return deltaTime;
	}
	size_t GLMGetFrameIndex() { 
		return frameIndex; 
	}

	bool GLMIsKeyDown(int keyCode) {
		return keysDown[keyCode];
	}
	bool GLMIsKeyUp(int keyCode) {
		return !keysDown[keyCode];
	}
	bool GLMIsKeyPressed(int keyCode) {
		return keysPressed[keyCode];
	}
	bool GLMIsKeyReleased(int keyCode) {
		return keysReleased[keyCode];
	}
}