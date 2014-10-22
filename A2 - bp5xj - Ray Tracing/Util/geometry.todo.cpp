#include <stdlib.h>
#include <math.h>

#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>
#include <float.h>

#include "geometry.h"


///////////////////////
// Ray-tracing stuff //
///////////////////////
double BoundingBox3D::intersect(const Ray3D& ray) const {
    // Ray inside box test
    Point3D p1 = p[0], p2 = p[1], p0 = ray.position;
    bool xTest = ( p0.p[0] <= fmax( p1.p[0], p2.p[0] ) ) &&
        ( p0.p[0] >= fmin( p1.p[0], p2.p[0] ));
    bool yTest = ( p0.p[1] <= fmax( p1.p[1], p2.p[1] ) ) && 
        ( p0.p[1] >= fmin( p1.p[1], p2.p[1] ) );
    bool zTest = ( p0.p[2] <= fmax( p1.p[2], p2.p[2] ) ) &&
        ( p0.p[2] >= fmin( p1.p[2], p2.p[2] ) ); 
    if ( xTest == true && yTest == true && zTest == true ) {
        return 0.0; 
    }

    // Create 6 planes, intersect 3 
    Point3D v1, v2, v3;
    double xDel = p2.p[0] - p1.p[0], 
           yDel = p2.p[1] - p1.p[1], 
           zDel = p2.p[2] - p1.p[2];

    // "Slabs" have same normal orientation
    /* ~~ Create Box shapes for each plane
     * ~~ Then do ray-box intersection
    */

        bool xTest = ( pHit.p[0] <= fmax( p1.p[0], p2.p[0] ) ) &&
            ( pHit.p[0] >= fmin( p1.p[0], p2.p[0] ));
        bool yTest = ( pHit.p[1] <= fmax( p1.p[1], p2.p[1] ) ) && 
            ( pHit.p[1] >= fmin( p1.p[1], p2.p[1] ) );
        bool zTest = ( pHit.p[2] <= fmax( p1.p[2], p2.p[2] ) ) &&
            ( pHit.p[2] >= fmin( p1.p[2], p2.p[2] ) ); 
        bool inBounds = xTest && yTest && zTest;
        if( curT < minT && inBounds == true ) {
            doesIntersect = true;
            minT = curT;
        }
    }

    return (doesIntersect) ? minT : -1.0;
}

/////////////////////
// Animation stuff //
/////////////////////
Matrix3D::Matrix3D(const Point3D& e){
	(*this)=Matrix3D();
}

Matrix3D::Matrix3D(const Quaternion& q){
	(*this)=Matrix3D();
}
Matrix3D Matrix3D::closestRotation(void) const {
	return (*this);
}
/* While these Exp and Log implementations are the direct implementations of the Taylor series, the Log
 * function tends to run into convergence issues so we use the other ones:*/
Matrix3D Matrix3D::Exp(const Matrix3D& m,int iter){
	return m;
}
