#include <stdlib.h>
#include <GL/glut.h>
#include "rayGroup.h"
#include <float.h>

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    float local_t, min_t = FLT_MAX;
    RayShape *curShape, *minShape;
    bool doesIntersect = false;

    /** ~~ Transformations ~~ **/
    Matrix4D transformation = this->getMatrix();
    Matrix4D transform_inv = getInverseMatrix();
    Ray3D rayTransform = transform_inv.mult( ray );
    rayTransform.direction = rayTransform.direction.unit();

    for( int i = 0; i < sNum; i++ ) {
        RayIntersectionInfo iInfoShape;

        curShape = shapes[i];
        local_t = curShape->intersect( rayTransform, iInfoShape, 0 );

        /** ~~ Transform back to global coordinates ~~ **/
        iInfoShape.iCoordinate = transformation.multPosition( iInfoShape.iCoordinate );
        iInfoShape.normal = getNormalMatrix().multDirection( iInfoShape.normal ).unit();
        double global_t = (iInfoShape.iCoordinate - ray.position).length();

        bool intersectPredicate = 
            ( mx > 0 ) ? ( local_t > 0.0 && global_t < min_t && global_t < mx ) :
                        ( local_t > 0.0 && global_t < min_t );

        if ( intersectPredicate == true ) {
            doesIntersect = true;
            minShape = curShape;
            min_t = global_t;
            iInfo = iInfoShape;
        }
    }

    return ( doesIntersect ) ? min_t : -1;
}

BoundingBox3D RayGroup::setBoundingBox(void){
	return bBox;
}

int StaticRayGroup::set(void){
    inverseTransform = this->getMatrix().invert();
    normalTransform = this->getMatrix().transpose().invert();
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
