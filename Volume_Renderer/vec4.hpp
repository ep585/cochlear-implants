#ifndef vec4_hpp
#define vec4_hpp

#include <iostream>
#include <cmath>

#include "vec2.hpp"
#include "vec3.hpp"

class vec4;

vec4 V4_Normalised(const vec4& v);
vec4 operator|(const vec3 &v3, const float &w);
vec4 operator|(const float &_x, const vec3 &v3);
vec4 operator|(const vec2 &v2a, const vec2 &v2b);

class vec4 {
public:
	friend vec4 operator+(const vec4& v1, const vec4& v2) {
		return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
	}
	friend vec4 operator-(const vec4& v1, const vec4& v2) {
		return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
	}
	friend vec4 operator*(const vec4& v1, const vec4& v2){
		return {v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w};
	}
	friend vec4 operator*(const vec4& v, const float& a){
		return {a*v.x, a*v.y, a*v.z, a*v.w};
	}
	friend vec4 operator*(const float& a, const vec4& v){
		return {a*v.x, a*v.y, a*v.z, a*v.w};
	}
	friend vec4 operator/(const vec4& v, const float& a){
		return {v.x/a, v.y/a, v.z/a, v.w/a};
	}
	vec4 operator-(){
		return {-x, -y, -z, -w};
	}
	vec4& operator+=(const vec4& v){
		*this = *this + v;
		return *this;
	}
	vec4& operator-=(const vec4& v){
		*this = *this - v;
		return *this;
	}
	vec4& operator*=(const float& a){
		*this = *this * a;
		return *this;
	}
	vec4& operator/=(const float& a){
		*this = *this / a;
		return *this;
	}
	vec4 &operator()(float (*const &func)(void)){
		x = func(); y = func(); z = func(); w = func();
		return *this;
	}
	
	friend std::ostream& operator<<(std::ostream& stream, const vec4& v){
		stream << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return stream;
	}
	float& operator[](const int& index){
		if(index == 0) return x;
		if(index == 1) return y;
		if(index == 2) return z;
		if(index == 3) return w;
		std::cout << "ERROR: vec4 index out of range." << std::endl;
		return x;
	}
	
	friend float V4_Dot(const vec4 &v1, const vec4 &v2){
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
	}
	
	vec4& Normalise(){
		*this /= sqrt(SqMag());
		return *this;
	}
	friend vec4 V4_Normalised(const vec4& v){
		return v / sqrt(v.SqMag());
	}
	
	float SqMag() const {
		return x*x + y*y + z*z + w*w;
	}
	
	vec2 xy() const { return {x, y}; }
	vec2 xz() const { return {x, z}; }
	vec2 xw() const { return {x, w}; }
	vec2 yz() const { return {y, z}; }
	vec2 yw() const { return {y, w}; }
	vec2 zw() const { return {z, w}; }
	
	vec3 xyz() const { return {x, y, z}; }
	vec3 xyw() const { return {x, y, w}; }
	vec3 xzw() const { return {x, z, w}; }
	vec3 yzw() const { return {y, z, w}; }
	
	friend vec4 operator|(const vec3 &v3, const float &_w){ return {v3.x, v3.y, v3.z, _w}; }
	friend vec4 operator|(const float &_x, const vec3 &v3){ return {_x, v3.x, v3.y, v3.z}; }
	friend vec4 operator|(const vec2 &v2a, const vec2 &v2b){ return {v2a.x, v2a.y, v2b.x, v2b.y}; }
	
	float x, y, z, w;
};

#endif /* vec4_hpp */
