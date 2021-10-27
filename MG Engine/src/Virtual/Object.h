#pragma once

#include <string>
#include <typeinfo>

class Object {
public:
	virtual std::string ToString() const;
	virtual int GetHashCode() const;

	bool operator==(const Object& O) const;
	bool operator==(Object&& O) const;
	bool operator!=(const Object& O) const;
	bool operator!=(Object&& O) const;
};

