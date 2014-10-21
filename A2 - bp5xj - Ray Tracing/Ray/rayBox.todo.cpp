#include <math.h>
#include <GL/glut.h>
#include <float.h>
#include "rayScene.h"
#include "rayBox.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    double tNear = FLT_MIN, tFar = FLT_MAX;
	return -1;
}
BoundingBox3D RayBox::setBoundingBox(void){
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayBox::drawOpenGL(int materialIndex){
	return -1;
}
