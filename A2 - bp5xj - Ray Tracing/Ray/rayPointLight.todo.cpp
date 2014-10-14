#include <math.h>
#include <GL/glut.h>
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D i_light = color / (constAtten + linearAtten*distance + quadAtten*pow(distance,2.0));

    Point3D k_diffuse = iInfo.material->diffuse;
    Point3D direction = (location - iInfo.iCoordinate).unit();
    double density = fmax( 0.0f, fmin( 1.0f, iInfo.normal.dot( direction )));

    Point3D i_diffuse = k_diffuse * density * i_light;
    return i_diffuse;
}
Point3D RayPointLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D i_light = color / (constAtten + linearAtten*distance + quadAtten*pow(distance,2.0));

    Point3D k_specular = iInfo.material->specular;
    Point3D L = ( location - iInfo.iCoordinate ).unit();
    Point3D R = iInfo.normal * 2.0 * (L.dot(iInfo.normal)) - L;
    Point3D V = ( cameraPosition - iInfo.iCoordinate ).unit();

    double percent_specular = fmax( 0.0f, fmin( 1.0f, R.dot( V )));
    double sfo = iInfo.material->specularFallOff;

    Point3D i_specular = k_specular * pow(percent_specular, sfo) * i_light;
    return i_specular;
}
int RayPointLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape,int& isectCount){
	return 0;
}
Point3D RayPointLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL(int index){
}
