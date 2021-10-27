#pragma once
#include "Object.h"
#include <string>

class Asset : public Object {
public:
	Asset() { }
	Asset(std::string FileLocation);

	virtual void SaveToFile(std::string FileLocation) const;
};

