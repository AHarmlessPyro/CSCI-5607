//#include "pch.h"
#include "Vector3.h"
#include <math.h>

Vector3::Vector3()
{
}

Vector3::~Vector3()
{
}

Vector3::Vector3(float xInp, float yInp, float zInp) {
	x = xInp;
	y = yInp;
	z = zInp;
}

Vector3 operator+(const Vector3& A, const Vector3& B) {
	return Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
}

Vector3 operator-(const Vector3& A, const Vector3& B) {
	return Vector3(A.x - B.x, A.y - B.y, A.z - B.z);
}

Vector3 operator*(const Vector3& A, const float& B) {
	return Vector3(A.x * B, A.y * B, A.z * B);
}

Vector3 operator*(const Vector3& A, const int& B) {
	return Vector3(A.x*B, A.y * B, A.z * B);
}

Vector3 operator*(const float& B, const Vector3& A) {
	return A * B;
}

Vector3 operator*(const int& B, const Vector3& A) {
	return A * B;
}

float operator*(const Vector3& A, const Vector3& B) {
	return dot(A, B);
}

Vector3 operator/(const Vector3& A, const float& B) {
	return Vector3(A.x / B, A.y / B, A.z / B);
}

Vector3 operator/(const Vector3& A, const int& B) {
	return Vector3(A.x / B, A.y / B, A.z / B);
}

Vector3 operator%(const Vector3& A, const int& B) {
	return Vector3((float)fmod(A.x, B), (float)fmod(A.y, B), (float)fmod(A.z, B));
}

Vector3 operator%(const Vector3& A, const float& B) {
	return Vector3(fmod(A.x, B), fmod(A.y, B), fmod(A.z, B));
}

float dot(const Vector3& A, const Vector3& B) {
	return (A.x*B.x + A.y*B.y + A.z*B.z);
}

Vector3 cross(const Vector3& A, const Vector3& B) {
	return Vector3((A.y*B.z - A.z*B.y), (A.z*B.x - A.x*B.z), (A.x*B.y - A.y*B.x));
}

float Vector3::length() const {
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

Vector3 Vector3::normalize() {
	if (this->length() != 0) {
		return (*this) / (this->length());
	}return Vector3(FP_INFINITE, FP_INFINITE, FP_INFINITE);
}

std::ostream & operator<<(std::ostream & out, const Vector3 vec)
{
	out << "" << vec.X() << " " << vec.Y() << " " << vec.Z() << "";
	return out;
}

Vector3 Vector3::toInt() const {
	return Vector3((int)abs(this->x), (int)abs(this->y), (int)abs(this->z));
}

float angleBetween(const Vector3 &A, const Vector3 &B) {
	return acos(dot(A, B) / (A.length()*B.length()));
}