#include <stdlib.h>
#include <GL/glut.h>
#include "rayGroup.h"
#include <float.h>

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    // TODO: Take into account mx
    float cur_t, min_t = FLT_MAX;
    RayShape *curShape, *minShape;
    bool doesIntersect = false;

    for( int i = 0; i < sNum; i++ ) {
        curShape = shapes[i];

        cur_t = curShape->intersect( ray, iInfo, 0 );
        if ( cur_t > 0.0 and cur_t < min_t ) {
            doesIntersect = true;
            minShape = curShape;
            min_t = cur_t;
        }
    }

    return ( doesIntersect ) ? min_t : -1;
}

BoundingBox3D RayGroup::setBoundingBox(void){
	return bBox;
}

int StaticRayGroup::set(void){
	return 1;
}
//////////////////
// OpenGL stuff //
//////////////////
int RayGroup::getOpenGLCallList(void){
	return 0;
}

int RayGroup::drawOpenGL(int materialIndex){
	return -1;
}

/////////////////////
// Animation Stuff //
/////////////////////
Matrix4D ParametrizedEulerAnglesAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedQuaternionAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
