#pragma once
#include "Object.h"

class GameNode : public Object {
private:
	size_t GameNodeIndex;
public:
	static GameNode** GameNodes;
	static size_t GameNodeCount;

	GameNode();
	GameNode(const GameNode& GN);
	GameNode(GameNode&& GN) noexcept;

	GameNode* GetParent() const;
	GameNode** GetChildren() const;
	GameNode** GetChildren(size_t& ChildCount) const;
	void SetParent(GameNode* NewParent);
	void AddChild(GameNode* NewChild);

	virtual void GameStart() { }
	virtual void GameFrame() { }
	virtual void GameRender() { }
	virtual void GameTick() { }

	~GameNode();
};

