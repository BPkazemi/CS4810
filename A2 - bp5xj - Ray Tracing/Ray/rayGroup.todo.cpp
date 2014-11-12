#include <stdlib.h>
#include <GL/glut.h>
#include "rayGroup.h"
#include <float.h>
#include <map>
using namespace std;

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

    /** ~~ BVH Test ~~ **
    BoundingBox3D bBox = setBoundingBox();
    if( bBox.intersect( rayTransform ) < 0.0 ) {  // How do we handle the 0 case?
        return -1.0;
    }
    */

    /* ~~ Find intersections with child bounding volumes ~~ *
    BoundingBox3D sBox;
    map<double, RayShape*> bv_t;
    double curT;

    for ( int i = 0; i < sNum; i++ ) {
        sBox = shapes[i]->setBoundingBox();

        curT = sBox.intersect( rayTransform );
        if ( curT > 0.0 ) {
            bv_t[ curT ] = shapes[i];
        }
    }
    */

    /*
    for ( map<double, RayShape*>::iterator i=bv_t.begin(); i != bv_t.end(); i++ ) {
        if( min_t < (*i).first ) { break; }

        RayIntersectionInfo iInfoShape;
        curShape = (*i).second;
        local_t = curShape->intersect( rayTransform, iInfoShape, 0.0 );

        // ~~ Transform back to global coordinates ~~ //
        iInfoShape.iCoordinate = transformation.multPosition( iInfoShape.iCoordinate );
        iInfoShape.normal = getNormalMatrix().multDirection( iInfoShape.normal ).unit();
        double global_t = (iInfoShape.iCoordinate - ray.position).length();

        bool intersectPredicate = 
            ( mx > 0.0 ) ? ( local_t > 0.0 && global_t < min_t && global_t < mx ) :
                        ( local_t > 0.0 && global_t < min_t );

        if ( intersectPredicate == true ) {
            doesIntersect = true;
            minShape = curShape;
            min_t = global_t;
            iInfo = iInfoShape;
        }

    }
    */

    // Comment out the below section if you're sorting shapes by intersection proximity
    for( int i = 0; i < sNum; i++ ) {
        RayIntersectionInfo iInfoShape;

        curShape = shapes[i];
        local_t = curShape->intersect( rayTransform, iInfoShape, 0.0 );

        // ~~ Transform back to global coordinates ~~ //
        iInfoShape.iCoordinate = transformation.multPosition( iInfoShape.iCoordinate );
        iInfoShape.normal = getNormalMatrix().multDirection( iInfoShape.normal ).unit();
        double global_t = (iInfoShape.iCoordinate - ray.position).length();

        bool intersectPredicate = 
            ( mx > 0.0 ) ? ( local_t > 0.0 && global_t < min_t && global_t < mx ) :
                        ( local_t > 0.0 && global_t < min_t );

        if ( intersectPredicate == true ) {
            doesIntersect = true;
            minShape = curShape;
            min_t = global_t;
            iInfo = iInfoShape;
        }
    }

    return ( doesIntersect ) ? min_t : -1.0;
}

BoundingBox3D RayGroup::setBoundingBox(void){
    BoundingBox3D bBox = BoundingBox3D( Point3D(0, 0, 0), Point3D(0, 0, 0) );

    /* ~~ Accumulate child bounding boxes ~~ */
    for ( int i = 0; i < sNum; i++ ) {
        bBox += shapes[i]->setBoundingBox();
    }

    /* ~~ Transform accumulated box ~~ */
    Matrix4D transformation = this->getMatrix();
    return bBox.transform( transformation );
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
