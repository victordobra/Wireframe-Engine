#pragma once

#include <string>
#include <typeinfo>

class Object {
public:
	virtual std::string ToString() const;
	virtual size_t GetHashCode() const;

	bool operator==(const Object& O) const;
	bool operator==(Object&& O) const;
	bool operator!=(const Object& O) const;
	bool operator!=(Object&& O) const;
};

