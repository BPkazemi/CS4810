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
    if( bBox.intersect( rayTransform ) <= 0.0 ) {  // How do we handle the 0 case?
        return -1.0;
    }
    */

    /*
    // ~~ Find intersections with child bounding volumes ~~ //
    BoundingBox3D shapeBox;
    RayShape *curChild;
    map<double, RayShape*> bv_t;
    double curT;

    for ( int i = 0; i < sNum; i++ ) {
        curChild = shapes[i];
        shapeBox = curChild->setBoundingBox();

        curT = shapeBox.intersect( rayTransform );
        if ( curT > 0.0 ) {
            bv_t[ curT ] = curChild;
        }
    }

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
    BoundingBox3D shapeBox, bBox = BoundingBox3D();
    RayShape *curShape;

    double minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN, minZ = FLT_MAX, maxZ = FLT_MIN;

    /* ~~ Accumulate child bounding boxes ~~ */
    for ( int i = 0; i < sNum; i++ ) {
        curShape = shapes[i];
        shapeBox = curShape->setBoundingBox();

        /*
        minX = min( minX, min( shapeBox.p[0].p[0], shapeBox.p[1].p[0] ));
        maxX = max( maxX, max( shapeBox.p[0].p[0], shapeBox.p[1].p[0] ));
        minY = min( minY, min( shapeBox.p[0].p[1], shapeBox.p[1].p[1] ));
        maxY = max( maxY, max( shapeBox.p[0].p[1], shapeBox.p[1].p[1] ));
        minZ = min( minZ, min( shapeBox.p[0].p[2], shapeBox.p[1].p[2] ));
        maxZ = max( maxZ, max( shapeBox.p[0].p[2], shapeBox.p[1].p[2] ));
        */

        bBox = bBox + shapeBox;
    }
    /*
    Point3D minPoint = Point3D( minX, minY, minZ );
    Point3D maxPoint = Point3D( maxX, maxY, maxZ );
    bBox = BoundingBox3D( minPoint, maxPoint );
    */

    /* ~~ Transform accumulated box ~~ */
    Matrix4D transformation = this->getMatrix();
    bBox = bBox.transform( transformation );

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
