#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}

protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;
	float angle;
};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle) {
		this->center = center;
		this->direction = direction.normalized();
		this->up = up.normalized();
		this->horizontal = Vector3f::cross(this->direction, this->up);
		this->angle = angle;
		// this->up = Vector3f::cross(this->horizontal, this->direction);
	}

	virtual Ray generateRay( const Vector2f& point) {
		float D = 1.0f / tan(this->angle/2);
		Vector3f direction = (point.x() * this->horizontal + point.y() * this->up + D * this->direction).normalized();
		return Ray(this->center, direction);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:

};

#endif //CAMERA_H
