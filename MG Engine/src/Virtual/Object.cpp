#include "Object.h"

std::string Object::ToString() const {
	return (std::string)typeid(this).name() + "( )";
}
size_t Object::GetHashCode() const {
	return typeid(this).hash_code();
}
bool Object::operator==(const Object& O) const {
	if (sizeof(*this) != sizeof(O))
		return false;
	return memcmp(&O, this, sizeof(O));
}
bool Object::operator==(Object&& O) const {
	if (sizeof(*this) != sizeof(O))
		return false;
	return memcmp(&O, this, sizeof(O));
}
bool Object::operator!=(const Object& O) const {
	if (sizeof(*this) != sizeof(O))
		return true;
	return !memcmp(&O, this, sizeof(O));
}
bool Object::operator!=(Object&& O) const {
	if (sizeof(*this) != sizeof(O))
		return true;
	return !memcmp(&O, this, sizeof(O));
}