#pragma once
#include <iostream>

class Vector3
{
private:
	float x, y, z;

public:
	inline float X() const {
		return x;
	}
	inline float Y() const {
		return y;
	}
	inline float Z() const {
		return z;
	}

	inline float R() const {
		return x;
	}
	inline float G() const {
		return y;
	}
	inline float B() const {
		return z;
	}

	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();

	friend Vector3 operator+(const Vector3&, const Vector3&);
	friend Vector3 operator-(const Vector3&, const Vector3&);
	friend Vector3 operator*(const Vector3&, const float&);
	friend Vector3 operator*(const Vector3&, const int&);
	friend Vector3 operator*(const float&, const Vector3&);
	friend Vector3 operator*(const int&, const Vector3&);

	friend Vector3 operator/(const Vector3&, const float&);
	friend Vector3 operator/(const Vector3&, const int&);

	friend Vector3 operator%(const Vector3&, const int&);
	friend Vector3 operator%(const Vector3&, const float&);

	friend float operator*(const Vector3&, const Vector3&);

	friend float dot(const Vector3&, const Vector3&);
	friend Vector3 cross(const Vector3&, const Vector3&);
	/*friend Vector3 operator*(const Vector3&, const Vector3&);
	friend Vector3 operator/(const Vector3&, const Vector3&);*/

	float length() const;

	Vector3 normalize();

	Vector3 toInt() const;

	friend float angleBetween(const Vector3 &,const Vector3 &);
};
std::ostream & operator<<(std::ostream &out, const Vector3 vec);
