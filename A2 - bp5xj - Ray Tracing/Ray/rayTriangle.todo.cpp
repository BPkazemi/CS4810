#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void){
    Point3D t0 = v[0]->position;
    Point3D t1 = v[1]->position;
    Point3D t2 = v[2]->position;

    Point3D line1 = t1 - t0;
    Point3D line2 = t2 - t0;

    plane.normal = line1.crossProduct(line2).unit();
}
double RayTriangle::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    // TODO: Utilize mx param
    // TODO: Modifying z-axis doesn't seem to change anything
    initialize();

    Point3D p0 = ray.position;
    Point3D V = ray.direction.unit();
    double t = -(plane.evaluate( p0 )) / ( V.dot( plane.normal ) );
    Point3D p = p0 + V*t;

    RayVertex t1, t2;
    for ( int i = 0; i < 2; i++ ) {
        t1 = *v[ i ];
        t2 = *v[ i+1 ];

        v1 = t1.position - p0;
        v2 = t2.position - p0;

        Point3D n1 = v2.crossProduct( v1 ).unit();

        if ( ((p-p0).dot(n1)) < 0 ) {  // Check if p within lines
            return 0;
        }
    }
    t1 = *v[ 2 ];
    t2 = *v[ 0 ];
    v1 = t1.position - p0;
    v2 = t2.position - p0;
    Point3D n3 = v2.crossProduct( v1 ).unit();
    if ( ((p-p0).dot(n3)) < 0 ) {  // Check if p within lines
        return 0;
    }

    iInfo.material = material;
    iInfo.iCoordinate = p;  
    iInfo.normal = plane.normal;
    iInfo.texCoordinate; // TODO: texCoordinates will be discussed in upcoming lectures

	return t;
}
BoundingBox3D RayTriangle::setBoundingBox(void){
    Point3D pList[3];
    pList[0] = v[0]->position;
    pList[1] = v[1]->position;
    pList[2] = v[2]->position;

    BoundingBox3D bBox = *(new BoundingBox3D( pList, 3 ));
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL(int materialIndex){
	return -1;
}
