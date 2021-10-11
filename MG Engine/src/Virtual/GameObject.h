#pragma once

#include "Object.h"
#include <string>

class GameObject : public Object {
protected:
	void Print(std::string Text);
	void Print(long long Num);
	void Print(float Num);
	void Print(double Num);
	void Print(Object Obj);
public:
	GameObject() { }

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
};

