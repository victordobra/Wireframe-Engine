#include "Object.h"

std::string Object::ToString() const {
	return (std::string)typeid(this).name() + "( )";
}
int Object::GetHashCode() const {
	return typeid(this).hash_code();
}
bool Object::Equals(const Object& O) const {
	return memcmp(&O, this, sizeof(O));
}
bool Object::Equals(Object&& O) const {
	return memcmp(&O, this, sizeof(O));
}
bool Object::operator==(const Object& O) const {
	return Equals(O);
}
bool Object::operator==(Object&& O) const {
	return Equals(O);
}
bool Object::operator!=(const Object& O) const {
	return !Equals(O);
}
bool Object::operator!=(Object&& O) const {
	return !Equals(O);
}