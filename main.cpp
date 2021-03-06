#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum ) {
      cerr << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
  }
  
  int index = 1;
  char *inputFile, *outputFile = "output.bmp", *depthFile = "depth.bmp", *normalsFile = "normals.bmp";
  int sizeX = 200, sizeY = 200;
  float depthMin = 8.0f, depthMax = 12.0f;
  bool doDepth = false, doNormals = false;
  while (index < argc) {
    if (strcmp("-input", argv[index]) == 0) {
      inputFile = argv[++index];
    } else if (strcmp("-size", argv[index]) == 0) {
      if (index + 2 < argc) {
        sizeX = atoi(argv[++index]);
        sizeY = atoi(argv[++index]);
      } else {
        cerr << "ERROR: bad input with -size flag. Defaulting to 200x200." << endl;
      }
    } else if (strcmp("-output", argv[index]) == 0) {
      if (index + 1 < argc) {
         outputFile = argv[++index];
      } else {
        cerr << "ERROR: bad input with -output flag. Defaulting to output.bmp." << endl;
      }
    } else if (strcmp("-depth", argv[index]) == 0) {
      if (index + 3 < argc) {
        depthMin = atof(argv[++index]);
        depthMax = atof(argv[++index]);
        depthFile = argv[++index];
      } else {
        cerr << "ERROR: bad input with -depth flag. Defaulting to 8, 12, depth.bmp" << endl;
      }
      doDepth = true;
    } else if (strcmp("-normals", argv[index]) == 0) {
      if (index + 1 < argc) {
         normalsFile = argv[++index];
      } else {
        cerr << "ERROR: bad input with -normals flag. Defaulting to normals.bmp" << endl;
      }
      doNormals = true;
    } else {
      cerr << "ERROR: bad command line arguments!";
    }
    ++index;
  }

  // First, parse the scene using SceneParser.
  SceneParser scene = SceneParser(inputFile);
  Camera* camera = scene.getCamera();
  float tmin = camera->getTMin();
  Group* group = scene.getGroup();

  Image image = Image(sizeX, sizeY);
  Image depthImage = Image(sizeX, sizeY);
  Image normalImage = Image(sizeX, sizeY);

  Vector3f backgroundColor = scene.getBackgroundColor();
  Vector3f ambientLight = scene.getAmbientLight();
  int numLights = scene.getNumLights();
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.
  float x,y;
  for (int i = 0; i < sizeX; i++) {
    x = 2.0f*i / (sizeX - 1) - 1;
    for (int j = 0; j < sizeY; j++) {
      y = 2.0f*j / (sizeY - 1) - 1;
      Ray ray = camera->generateRay(Vector2f(x,y));
      Hit hit = Hit();
      if (group->intersect(ray, hit, tmin)) {
        Vector3f totalColor = scene.getAmbientLight();
        if (hit.hasTex && hit.getMaterial()->t.valid()) {
          totalColor = totalColor * hit.getMaterial()->t(hit.texCoord[0], hit.texCoord[1]);
        } else {
          totalColor = totalColor * hit.getMaterial()->getDiffuseColor();
        }
        for (int k=0; k < numLights; k++) {
          Light* light = scene.getLight(k);
          Vector3f dir, col;
          float f;
          light->getIllumination(ray.pointAtParameter(hit.getT()), dir, col, f);
          totalColor += hit.getMaterial()->Shade(ray, hit, dir, col);
        }
        image.SetPixel(i, j, totalColor);
      } else {
        image.SetPixel(i, j, backgroundColor);
      }
      if (doDepth) {
        float depth = hit.getT();
        if (depth > depthMax) {
          depthImage.SetPixel(i, j, Vector3f(0.0f, 0.0f, 0.0f));
        } else if (depth < depthMin) {
          depthImage.SetPixel(i, j, Vector3f(1.0f, 1.0f, 1.0f));
        } else {
          float c = 1 - (depth - depthMin) / (depthMax - depthMin);
          depthImage.SetPixel(i, j, Vector3f(c, c, c));
        }
      }
      if (doNormals) {
        Vector3f normal = hit.getNormal();
        normal[0] = abs(normal[0]);
        normal[1] = abs(normal[1]);
        normal[2] = abs(normal[2]);
        normalImage.SetPixel(i, j, normal.normalized() * Vector3f(1.0f, 1.0f, 1.0f));
      }
    }
  }
  image.SaveImage(outputFile);
  if (doDepth) {
    depthImage.SaveImage(depthFile);
  }
  if (doNormals) {
    normalImage.SaveImage(normalsFile);
  }

  return 0;
}

