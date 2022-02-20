#include "Node.h"

namespace mge {
	Node* Node::scene = new Node();

	void Node::SetParent(Node* newParent) {
		parent = newParent;
		parent->children.push_back(dynamic_cast<Node*>(this));
	}

	Node::~Node() {
		if (parent != nullptr)
			for (size_t i = 0; i < parent->children.size(); ++i)
				if (parent->children[i] == this)
					parent->children.erase(parent->children.begin() + i);

		for (size_t i = 0; i < children.size(); ++i)
			delete children[i];

		Delete();
	}
}