#include <math.h>
#include <GL/glut.h>
#include <float.h>
#include "rayScene.h"
#include "rayBox.h"
#include "rayTriangle.h"
#include <bitset>

extern const double EPSILON;

//////////////////////
//  Helper Function //
/////////////////////
double intersectPlane( Plane3D& plane, Ray3D& ray ) {
    if ( ray.direction.dot( plane.normal ) == 0.0 )  return -1.0;  // Ray parallel to plane

    double t = -(plane.evaluate( ray.position )) / ray.direction.dot( plane.normal );
    if ( t < 0.0 ) return -1.0;  // Box behind ray

    return t;
}
////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    Point3D corner0 = center - (length / 2.0);
    Point3D corner1 = center + (length / 2.0);
    double xMin = corner0.p[0], yMin = corner0.p[1], zMin = corner0.p[2],
           xMax = corner1.p[0], yMax = corner1.p[1], zMax = corner1.p[2],
           tCur, tMin = FLT_MAX;
    bool doesIntersect = false;

    Plane3D planes[6]; 
    Point3D b1, b2, b3;
    b1 = corner0; 
    b2 = corner0 + Point3D( length.p[0], 0, 0 );
    b3 = corner0 + Point3D( 0, 0, length.p[2] ); 
    Plane3D bottom = Plane3D( b1, b2, b3 );
    planes[0] = bottom;

    Point3D t1, t2, t3;
    t1 = corner1;
    t2 = corner1 + Point3D( 0, 0, -length.p[2] );
    t3 = corner1 + Point3D( -length.p[0], 0, 0 );
    Plane3D top = Plane3D( t1, t2, t3 );
    planes[1] = top;

    Point3D l1, l2, l3;
    l1 = corner0;
    l2 = corner0 + Point3D( 0, 0, length.p[2] );
    l3 = corner0 + Point3D( 0, length.p[1], 0 );
    Plane3D left = Plane3D( l1, l2, l3 );
    planes[2] = left;

    Point3D r1, r2, r3;
    r1 = corner1;
    r2 = corner1 + Point3D( 0, -length.p[1], 0 );
    r3 = corner1 + Point3D( 0, 0, -length.p[2] );
    Plane3D right = Plane3D( r1, r2, r3 );
    planes[3] = right;

    Point3D ba1, ba2, ba3;
    ba1 = corner0;
    ba2 = corner0 + Point3D( 0, length.p[1], 0 );
    ba3 = corner0 + Point3D( length.p[0], 0, 0 );
    Plane3D back = Plane3D( ba1, ba2, ba3 );
    planes[4] = back;

    Point3D f1, f2, f3;
    f1 = corner1;
    f2 = corner1 + Point3D( -length.p[0], 0, 0 );
    f3 = corner1 + Point3D( 0, -length.p[1], 0 );
    Plane3D front = Plane3D( f1, f2, f3 );
    planes[5] = front;

    // Plane3D curPl;
    for ( int i = 0; i < 6; i++ ) {
        Plane3D curPl = planes[ i ];

        double tCur = intersectPlane( curPl, ray );
        if ( tCur == -1.0 ) { continue; }

        Point3D iDistance = ray.direction * tCur;
        Point3D pHit = ray.position + iDistance;

        bool xTest = ( pHit.p[0] <= corner1.p[0] ) &&
                ( pHit.p[0] >= corner0.p[0] );
        bool yTest = ( pHit.p[1] <= corner1.p[1] ) &&
                ( pHit.p[1] >= corner0.p[1] );
        bool zTest = ( pHit.p[2] <= corner1.p[2] ) &&
                ( pHit.p[2] >= corner0.p[2] );

        if( xTest == false || yTest == false || zTest == false )  continue;  // OOB

        if( tCur < tMin ) {
            doesIntersect = true;
            tMin = tCur;

            iInfo.material = material;
            iInfo.iCoordinate = pHit;  
            iInfo.normal = curPl.normal;
            iInfo.texCoordinate; // TODO: texCoordinates will be discussed in upcoming lectures
        }
    }

    return (doesIntersect) ? tMin : -1.0;
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
