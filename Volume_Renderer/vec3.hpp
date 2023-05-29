#ifndef vec3_hpp
#define vec3_hpp

#include <iostream>
#include <cmath>

#include "vec2.hpp"

class vec3;

vec3 V3_Normalised(const vec3& v);
vec3 V3_Cross(const vec3& v1, const vec3& v2);
vec3 operator|(const vec2 &v2, const float &_z);
vec3 operator|(const float &_x, const vec2 &v2);

class vec3 {
public:
	friend vec3 operator+(const vec3& v1, const vec3& v2) {
		return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	}
	friend vec3 operator-(const vec3& v1, const vec3& v2) {
		return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	}
	friend vec3 operator*(const vec3& v1, const vec3& v2){
		return {v1.x*v2.x, v1.y*v2.y, v1.z*v2.z};
	}
	friend vec3 operator*(const vec3& v, const float& a){
		return {a*v.x, a*v.y, a*v.z};
	}
	friend vec3 operator*(const float& a, const vec3& v){
		return {a*v.x, a*v.y, a*v.z};
	}
	friend vec3 operator/(const vec3& v, const float& a){
		const float aInv = 1.0f/a;
		return {v.x*aInv, v.y*aInv, v.z*aInv};
	}
	friend vec3 operator/(const float& a, const vec3& v){
		return {a/v.x, a/v.y, a/v.z};
	}
	vec3 operator-(){
		return {-x, -y, -z};
	}
	vec3& operator+=(const vec3& v){
		*this = *this + v;
		return *this;
	}
	vec3& operator-=(const vec3& v){
		*this = *this - v;
		return *this;
	}
	vec3& operator*=(const float& a){
		*this = *this * a;
		return *this;
	}
	vec3& operator/=(const float& a){
		*this = *this / a;
		return *this;
	}
	vec3 &operator()(float (*const &func)(void)){
		x = func(); y = func(); z = func();
		return *this;
	}
	
	friend std::ostream& operator<<(std::ostream& stream, const vec3& v){
		stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return stream;
	}
	
	friend float V3_Dot(const vec3 &v1, const vec3 &v2){
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	
	vec3& Normalise(){
		*this /= sqrt(SqMag());
		return *this;
	}
	
	friend vec3 V3_Normalised(const vec3& v){
		return v / sqrt(v.SqMag());
	}
	
	friend vec3 V3_Cross(const vec3& v1, const vec3& v2){
		return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
	}
	
	float SqMag() const {
		return x*x + y*y + z*z;
	}
	
	vec2 xy() const { return {x, y}; }
	vec2 yx() const { return {y, x}; }
	vec2 xz() const { return {x, z}; }
	
	friend vec3 operator|(const vec2 &v2, const float &_z){ return {v2.x, v2.y, _z}; }
	friend vec3 operator|(const float &_x, const vec2 &v2){ return {_x, v2.x, v2.y}; }
	
	float x, y, z;
};

#endif /* vec3_hpp */
