#include <string>
#include "GameNode.h"

namespace mge {
	GameNode** GameNode::gameNodes = new GameNode*[0];
	size_t GameNode::gameNodeCount = 0;

	GameNode::GameNode() {
		gameNodeIndex = gameNodeCount;
		gameNodeCount++;

		GameNode** NewGameNodes = new GameNode * [gameNodeCount];
		memcpy(NewGameNodes, gameNodes, sizeof(GameNode*) * (gameNodeCount - 1));

		NewGameNodes[gameNodeIndex] = this;

		delete[] gameNodes;

		gameNodes = NewGameNodes;
	}
	GameNode::GameNode(const GameNode& other) {
		memcpy(this, &other, sizeof(*this));

		gameNodeIndex = gameNodeCount;
		gameNodeCount++;

		GameNode** NewGameNodes = new GameNode * [gameNodeCount];
		memcpy(NewGameNodes, gameNodes, sizeof(GameNode*) * (gameNodeCount - 1));

		NewGameNodes[gameNodeIndex] = this;

		delete[] gameNodes;

		gameNodes = NewGameNodes;
	}
	GameNode::GameNode(GameNode&& GN) noexcept {
		gameNodeIndex = gameNodeCount;
		gameNodeCount++;

		GameNode** NewGameNodes = new GameNode*[gameNodeCount];
		memcpy(NewGameNodes, gameNodes, sizeof(GameNode*) * (gameNodeCount - 1));

		NewGameNodes[gameNodeIndex] = this;

		delete[] gameNodes;

		gameNodes = NewGameNodes;
	}

	GameNode::~GameNode() {
		gameNodeCount--;

		GameNode** NewGameNodes = new GameNode * [gameNodeCount];
		memcpy(NewGameNodes, gameNodes, gameNodeIndex);
		memcpy(NewGameNodes + gameNodeIndex, gameNodes + gameNodeIndex + 1, sizeof(GameNode*) * (gameNodeCount - gameNodeIndex));

		delete[] gameNodes;

		gameNodes = NewGameNodes;
	}
}