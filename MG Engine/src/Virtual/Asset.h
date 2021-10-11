#pragma once

#include "Object.h"
#include <string>

class Asset : public Object {
public:
	virtual void SaveToFile(std::string FileLocation);
	virtual void LoadFromFile(std::string FileLocation);
	virtual bool LoadFromExternalFile(std::string FileLocation);
};

