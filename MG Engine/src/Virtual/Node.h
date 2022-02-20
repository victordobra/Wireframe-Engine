#pragma once
#include "Defines.h"

#include <vector>
#include <cassert>

namespace mge {
	class Node {
	public:
		static Node* scene;

		Node() { Awake(); }
		Node(const Node&) = delete;
		Node(Node&&) noexcept = delete;

		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) noexcept = delete;

		bool8_t operator==(const Node&) = delete;
		bool8_t operator==(Node&&) = delete;
		bool8_t operator!=(const Node&) = delete;
		bool8_t operator!=(Node&&) = delete;

		Node* GetParent() { return parent; }
		std::vector<Node*> GetChildren() { return children; }
		void SetParent(Node* newParent);

		virtual void Start()  { }
		virtual void Awake()  { }
		virtual void Frame()  { }
		virtual void Render() { }
		virtual void Tick()   { }
		virtual void Delete() { }

		~Node();
	private:
		std::vector<Node*> children;

		Node* parent{nullptr};
	};
}
