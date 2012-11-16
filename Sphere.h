#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere() : Object3D() { 
		//unit ball at the center	
		this->center = Vector3f::ZERO;
		this->radius = 1.0f;
	}

	Sphere( Vector3f center , float radius , Material* material ) : Object3D(material) {
		this->center = center;
		this->radius = radius;
	}
	

	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin) {
		// find insersection
		float a, b, c, disc, t1, t2, result;
		Vector3f r_o = r.getOrigin() - this->center; // translate so that sphere @ (0,0,0)
		Vector3f r_d = r.getDirection();
		a = r_d.abs();
		b = 2 * Vector3f::dot(r_d, r_o);
		c = Vector3f::dot(r_o, r_o) - this->radius*this->radius;
		disc = b*b - 4*a*c; 
		if (disc < 0) {
			return false;
		}
		disc = sqrt(disc);
		t1 = (-b - disc)/2*a;
		t2 = (-b + disc)/2*a; // t2 > t1
		if (t1 >= tmin) {
			result = t1;
		} else if (t2 >= tmin) {
			result = t2;
		} else {
			return false;
		}

		// compare to current Hit
		if (result < h.getT()) {
			Vector3f normal = (r.pointAtParameter(result) - this->center).normalized();
			h.set(result, material, normal);
			return true;
		}
		return false;
	}

protected:

	Vector3f center;
	float radius;
	

};


#endif
