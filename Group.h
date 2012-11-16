#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

  Group() {
    this->num_objects = 0;
  }
	
  Group( int num_objects ){
    this->num_objects = num_objects;
  }

  ~Group(){
    for (unsigned i=0; i < objects.size(); i++) {
      delete objects[i];
    }
  }

  virtual bool intersect( const Ray& r , Hit& h , float tmin ) {
    bool hasIntersect = false;
    for (unsigned i=0; i < objects.size(); i++) {
      Object3D* obj = objects[i];
      if (obj->intersect(r, h, tmin)) {
        hasIntersect = true;
      }
    }
    return hasIntersect;
  }
	
  void addObject( int index , Object3D* obj ){
    if (index != objects.size()) {
      cerr << "ERROR: inconsitent indices!" << endl;
    }
    objects.push_back(obj);
    if (objects.size() > num_objects) {
      cerr << "ERROR: number of actual objects greater than declared number!" << endl;
    }
  }

  int getGroupSize(){ 
    return this->num_objects;
  }

 private:
    vector<Object3D*> objects;
    int num_objects;

};

#endif
	
