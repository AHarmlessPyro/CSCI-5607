#include "Sphere.h"
#include <tuple>
#include <Math.h>
#include <limits>


Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

Sphere::Sphere(Vector3 cen, float rad) {
	_centre = cen;
	_radius = rad;
}

Sphere::Sphere(Vector3 cen, float rad, int mtl) {
	_centre = cen;
	_radius = rad;
	_mtl = mtl;
}

Sphere::Sphere(float x, float y, float z, float rad) {
	_centre = Vector3(x, y, z);
	_radius = rad;
}

Sphere::Sphere(float x, float y, float z, float rad, int mtl) {
	_centre = Vector3(x, y, z);
	_radius = rad;
	_mtl = mtl;
}

Vector3 Sphere::Centre() const {
	return _centre;
}

float Sphere::Radius() const {
	return _radius;
}

Object3D::Hit* Sphere::hit(Ray &r, float tmin, float tmax) {
	// Solve for ray and sphere intersection. Otherwise return background
	Vector3 OC = r.Origin() - Centre();

	float a = dot(r.Direction(), r.Direction());
	float b = dot(OC, r.Direction());
	float c = dot(OC, OC) - pow(Radius(), 2);

	Hit* temp = (Object3D::Hit*)calloc(1, sizeof(Object3D::Hit));

	float discriminant = pow(b, 2) - a * c;
	if (discriminant >= 0) {

		float tVal = (-b - sqrt(discriminant)) / a;
		if ((tmax >= tVal) && (tVal >= tmin)) {
			temp->hit = true;
			temp->tVal = tVal;
			temp->point = r.pointAtParameter(tVal);
			temp->lastHit = this;
			return temp;
		}
		tVal = (-b + sqrt(discriminant)) / a;
		if ((tmax >= tVal) && (tVal >= tmin)) {
			temp->hit = true;
			temp->tVal = tVal;
			temp->point = r.pointAtParameter(tVal);
			temp->lastHit = this;
			return temp;
		}
	}
	return { false,-1.0,Vector3(std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest() ,std::numeric_limits<float>::lowest()),this };
}

int Sphere::Mat() const {
	return _mtl;
}
