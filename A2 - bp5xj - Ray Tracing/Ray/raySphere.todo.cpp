#include <math.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySphere.h"

extern const double EPSILON;
////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
    // TODO: Utilize mx param
    Point3D p0 = ray.position;
    Point3D L = center - p0;
    Point3D v = ray.direction.unit();
    double t_center = L.dot( v );  
    double d_squared = L.dot( L ) - pow(t_center, 2.0);

    if ( d_squared > pow( radius, 2.0 ) ) {
        return 0;
    }

    double t_edge = sqrt( pow(radius, 2.0) - d_squared );
    double t = fmin( t_center-t_edge, t_center+t_edge ); 

    Point3D p = p0 + v*t;

    Point3D pVector = (p-center);
    double pRadius = pVector.length();
    double difference = pow( pRadius, 2.0 ) - pow( radius, 2.0 );
    bool doesIntersect = (pow( (p - center).length(), 2.0 ) - pow( radius, 2.0 )) < EPSILON; 
    if ( !doesIntersect ) {
        return 0;
    }

    iInfo.material = material;
    iInfo.iCoordinate = p;  
    iInfo.normal = (p - center) / (p - center).length();  
    iInfo.texCoordinate; // TODO: texCoordinates will be discussed in upcoming lectures

	return t;
}
BoundingBox3D RaySphere::setBoundingBox(void){
    Point3D p1 = center + Point3D( radius, 0, 0 );
    Point3D p2 = center - Point3D( radius, 0, 0 );
    Point3D p3 = center + Point3D( 0, radius, 0 );
    Point3D p4 = center - Point3D( 0, radius, 0 );
    Point3D p5 = center + Point3D( 0, 0, radius );
    Point3D p6 = center - Point3D( 0, 0, radius );

    double xMin = fmin( p1[0], p2[0]);
    double yMin = fmin( p3[1], p4[1]);
    double zMin = fmin( p5[2], p6[2] );
    Point3D antiPodal1 = Point3D( xMin, yMin, zMin );

    double xMax = fmax( p1[0], p2[0]);
    double yMax = fmax( p3[1], p4[1]);
    double zMax = fmax( p5[2], p6[2] );
    Point3D antiPodal2 = Point3D( xMax, yMax, zMax );

    BoundingBox3D bBox = *(new BoundingBox3D( antiPodal1, antiPodal2  ));
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RaySphere::drawOpenGL(int materialIndex){
	return -1;
}
