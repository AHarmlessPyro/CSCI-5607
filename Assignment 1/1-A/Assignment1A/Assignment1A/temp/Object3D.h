#pragma once
#include "Ray.h"

class Object3D
{
protected:
	int _mtl;

public:
	struct Hit {
		bool hit;
		float tVal;
		Vector3 point;
		Object3D* lastHit;
	};

	virtual Hit* hit(Ray &r, float tmin, float tmax) = 0;

	virtual int Mat() const = 0;
};

