#include <math.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D L = (iInfo.iCoordinate - location).unit();
    double cutOff = cos( cutOffAngle );

    if ( direction.dot(L) > cutOff ) {
        Point3D numer = color * ( pow(direction.dot( L ), dropOffRate ));
        double denom = constAtten + linearAtten*distance + quadAtten*pow(distance, 2.0);
        Point3D i_light = numer / denom;

        Point3D k_diffuse = iInfo.material->diffuse;
        Point3D direction = (location - iInfo.iCoordinate).unit();
        double density = fmax( 0.0f, fmin( 1.0f, iInfo.normal.dot( direction )));

        Point3D i_diffuse = k_diffuse * density * i_light;
        return i_diffuse;
    }
    return Point3D(0.0, 0.0, 0.0);
}
Point3D RaySpotLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
int RaySpotLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape,int& isectCount){
	return 0;
}
Point3D RaySpotLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySpotLight::drawOpenGL(int index){
}
