#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor(d_color),specularColor(s_color), shininess(s)
  {
        	
  }

  virtual ~Material()
  {

  }

  virtual Vector3f getDiffuseColor() const 
  { 
    return  diffuseColor;
  }
    

  Vector3f Shade( const Ray& ray, const Hit& hit,
                  const Vector3f& dirToLight, const Vector3f& lightColor ) {
    Vector3f normal = hit.getNormal();
    float d = Vector3f::dot(dirToLight, normal);
    if (d < 0) {
      d = 0;
    }
    Vector3f textureColor = diffuseColor;
    if (t.valid()) {
      textureColor = t(hit.texCoord[0], hit.texCoord[1]);
    }
    Vector3f diffuse = d * Vector3f(textureColor[0]*lightColor[0], textureColor[1]*lightColor[1], textureColor[2]*lightColor[2]);
    Vector3f reflect = ray.getDirection() - 2 * Vector3f::dot(ray.getDirection(), normal) * normal.normalized();
    float c_s = Vector3f::dot(dirToLight, reflect);
    if (c_s > 0) {
      c_s = pow(c_s, shininess);
    } else {
      c_s = 0.0f;
    }
    Vector3f specular = c_s * Vector3f(specularColor[0]*lightColor[0], specularColor[1]*lightColor[1], specularColor[2]*lightColor[2]);
    return diffuse + specular;		
  }

  void loadTexture(const char * filename){
    t.load(filename);
  }
 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
