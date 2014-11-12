#include <stdlib.h>
#include <math.h>

#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>
#include <float.h>

#include "geometry.h"
#include "../Ray/rayBox.h"
#include "../Ray/rayScene.h"


///////////////////////
// Ray-tracing stuff //
///////////////////////
double BoundingBox3D::intersect(const Ray3D& ray) const {
    Point3D p0 = p[0], p1 = p[1];

    // Ray inside box test
    bool xTest = p0.p[0] <= ray.position.p[0] && ray.position.p[0] <= p1.p[0];
    bool yTest = p0.p[1] <= ray.position.p[1] && ray.position.p[1] <= p1.p[1];
    bool zTest = p0.p[2] <= ray.position.p[2] && ray.position.p[2] <= p1.p[2];

    if ( xTest == true && yTest == true && zTest == true ) { return 0.0; }

    // Create RayBox of BoundingVolume
    Point3D center = Point3D( 
            ( p0.p[0] + p1.p[0] ) / 2.0, 
            ( p0.p[1] + p1.p[1] ) / 2.0, 
            ( p0.p[2] + p1.p[2] ) / 2.0
    );
    double xLen = p1.p[0] - p0.p[0];
    double yLen = p1.p[1] - p0.p[1];
    double zLen = p1.p[2] - p0.p[2];

    RayBox rBox = RayBox();
    rBox.center = center;
    rBox.length = Point3D(xLen, yLen, zLen);

    RayIntersectionInfo iInfo;
    return rBox.intersect( ray, iInfo, 0.0 );
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
