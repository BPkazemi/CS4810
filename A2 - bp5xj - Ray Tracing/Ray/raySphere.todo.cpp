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
        return -1.0;
    }

    double t_edge = sqrt( pow(radius, 2.0) - d_squared );
    double t = fmin( t_center-t_edge, t_center+t_edge ); 
    if ( t < 0.0 ) { return -1.0; }  // Intersect behind ray

    Point3D p = p0 + v*t;

    Point3D pVector = (p-center);
    double pRadius = pVector.length();
    double difference = pow( pRadius, 2.0 ) - pow( radius, 2.0 );
    bool doesIntersect = (pow( (p - center).length(), 2.0 ) - pow( radius, 2.0 )) < EPSILON; 
    if ( !doesIntersect ) {
        return -1.0;
    }

    iInfo.material = material;
    iInfo.iCoordinate = p;  
    iInfo.normal = (p - center) / (p - center).length();  
    iInfo.texCoordinate; // TODO: texCoordinates will be discussed in upcoming lectures

	return t;
}
BoundingBox3D RaySphere::setBoundingBox(void){
    Point3D antiPodal1 = center - Point3D(radius, radius, radius);
    Point3D antiPodal2 = center + Point3D(radius, radius, radius);

    return BoundingBox3D( antiPodal1, antiPodal2 );
}

//////////////////
// OpenGL stuff //
//////////////////
int RaySphere::drawOpenGL(int materialIndex){
	return -1;
}
