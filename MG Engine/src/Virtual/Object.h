#pragma once

#include <string>
#include <typeinfo>

class Object {
public:
	virtual std::string ToString() const;
	virtual int GetHashCode() const;
	virtual bool Equals(const Object& O) const;
	virtual bool Equals(Object&& O) const;

	bool operator==(const Object& O) const;
	bool operator==(Object&& O) const;
	bool operator!=(const Object& O) const;
	bool operator!=(Object&& O) const;
};

