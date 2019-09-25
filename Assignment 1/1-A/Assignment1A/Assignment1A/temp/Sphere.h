#pragma once
#include "Object3D.h"
#include "Vector3.h"

class Sphere : public Object3D
{
	Vector3 _centre;
	float _radius;

public:
	Sphere(Vector3 cen, float rad);
	Sphere(Vector3 cen, float rad, int mat);
	Sphere(float x, float y, float z, float rad);
	Sphere(float x, float y, float z, float rad,int mtl);
	Sphere();
	~Sphere();
	Vector3 Centre() const;
	float Radius() const;
	Hit* hit(Ray &r, float tmin, float tmax);
	int Mat() const ;
};

