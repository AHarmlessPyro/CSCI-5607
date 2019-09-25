#include "Ray.h"



Ray::Ray()
{
}

Ray::Ray(Vector3 origin, Vector3 direction) {
	_origin = origin;
	_direction = direction;
}

Ray::~Ray()
{
}

Vector3 Ray::Origin() const {
	return _origin;
}

Vector3 Ray::Direction() const {
	return _direction;
}

Vector3 Ray::pointAtParameter(float t)  const{
	return _origin + (t*_direction);
}

std::ostream &operator<<(std::ostream &out, const Ray r) {
	out << (r.Origin()) << " -> " << (r.Direction());
	return out;
}