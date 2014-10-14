#include <stdlib.h>
#include <GL/glut.h>
#include "rayGroup.h"
#include <float.h>

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    float cur_t, min_t = FLT_MAX;
    RayShape *curShape, *minShape;
    bool doesIntersect = false;

    for( int i = 0; i < sNum; i++ ) {
        RayIntersectionInfo iInfoShape;

        curShape = shapes[i];
        cur_t = curShape->intersect( ray, iInfoShape, 0 );

        bool intersectPredicate = 
            ( mx > 0 ) ? ( cur_t > 0.0 && cur_t < min_t && cur_t < mx ) :
                        ( cur_t > 0.0 && cur_t < min_t );

        if ( intersectPredicate == true ) {
            doesIntersect = true;
            minShape = curShape;
            min_t = cur_t;
            iInfo = iInfoShape;
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
