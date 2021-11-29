#include "GameNode.h"

GameNode** GameNode::GameNodes = new GameNode*[0];
size_t GameNode::GameNodeCount = 0;

GameNode::GameNode() {
	GameNodeIndex = GameNodeCount;
	GameNodeCount++;

	GameNode** NewGameNodes = new GameNode*[GameNodeCount];
	memcpy(NewGameNodes, GameNodes, sizeof(GameNode*) * (GameNodeCount - 1));

	NewGameNodes[GameNodeIndex] = this;

	delete[] GameNodes;

	GameNodes = NewGameNodes;
}
GameNode::GameNode(const GameNode& GN) {
	GameNodeIndex = GameNodeCount;
	GameNodeCount++;

	GameNode** NewGameNodes = new GameNode*[GameNodeCount];
	memcpy(NewGameNodes, GameNodes, sizeof(GameNode*) * (GameNodeCount - 1));

	NewGameNodes[GameNodeIndex] = this;

	delete[] GameNodes;

	GameNodes = NewGameNodes;
}
GameNode::GameNode(GameNode&& GN) noexcept {
	GameNodeIndex = GameNodeCount;
	GameNodeCount++;

	GameNode** NewGameNodes = new GameNode * [GameNodeCount];
	memcpy(NewGameNodes, GameNodes, sizeof(GameNode*) * (GameNodeCount - 1));

	NewGameNodes[GameNodeIndex] = this;

	delete[] GameNodes;

	GameNodes = NewGameNodes;
}

GameNode::~GameNode() {
	GameNodeCount--;

	GameNode** NewGameNodes = new GameNode*[GameNodeCount];
	memcpy(NewGameNodes, GameNodes, GameNodeIndex);
	memcpy(NewGameNodes + GameNodeIndex, GameNodes + GameNodeIndex + 1, sizeof(GameNode*) * (GameNodeCount - GameNodeIndex));

	delete[] GameNodes;

	GameNodes = NewGameNodes;
}