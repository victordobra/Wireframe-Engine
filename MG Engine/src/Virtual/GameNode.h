#pragma once

namespace mge {
	class GameNode {
	private:
		size_t gameNodeIndex;
	public:
		static GameNode** gameNodes;
		static size_t gameNodeCount;

		GameNode();
		GameNode(const GameNode& other);
		GameNode(GameNode&& other) noexcept;

		GameNode* GetParent() const;
		GameNode** GetChildren() const;
		GameNode** GetChildren(size_t& childCount) const;
		void SetParent(GameNode* newParent);
		void AddChild(GameNode* newChild);

		virtual void GameStart() { }
		virtual void GameFrame() { }
		virtual void GameRender() { }
		virtual void GameTick() { }

		~GameNode();
	};
}
