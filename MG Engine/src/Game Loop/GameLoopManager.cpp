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
		meshRenderer->position = { 0.0f, 0.0f, 5.0f };
		meshRenderer->color = { 0.0f, 0.3f, 1.0f };
		meshRenderer->scale = { 1.0f, 1.0f, 1.0f };
		AssetLoadResult result;
		Mesh* mesh = new Mesh("Assets\\Cube.obj", result);
		meshRenderer->mesh = mesh;
		camera = new Camera();
		camera->clearColor = { 0.01f, 0.01f, 0.01f };
		camera->clearMode = ClearMode::COLOR;
		camera->zNear = 0.01f;
		camera->zFar = 100.0f;
		camera->fov = 60.0f;

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

	static float yRot = 0.0f;

	void GLMUpdate() {
		//Start a timer to calculate delta time
		std::chrono::high_resolution_clock timer{ };
		auto start = timer.now();

		//Create an array of futures
		std::future<void>* futures = new std::future<void>[GameNode::gameNodeCount];

		yRot += 90.0f * deltaTime;
		if (yRot > 360.0f)
			yRot -= 360.0f;

		meshRenderer->rotation = Quaternion::EulerAngles(0, yRot, 0);

		if (keysDown['W'])
			camera->position.z += deltaTime;
		else if (keysDown['S'])
			camera->position.z -= deltaTime;

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
		deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000;
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