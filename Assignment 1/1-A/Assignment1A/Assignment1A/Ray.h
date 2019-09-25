#pragma once
#include "Vector3.h"
#include <iostream>

class Ray
{
private:
	Vector3 _origin;
	Vector3 _direction;
public:
	Ray(Vector3 origin, Vector3 direction);
	Ray();
	~Ray();
	Vector3 Origin() const;
	Vector3 Direction() const;
	Vector3 pointAtParameter(float t) const;
};
std::ostream &operator<<(std::ostream &out, const Ray r);

