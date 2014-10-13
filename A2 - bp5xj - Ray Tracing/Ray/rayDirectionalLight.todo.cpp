#include <math.h>
#include <GL/glut.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    Point3D i_light = color;
    Point3D k_diffuse = iInfo.material->diffuse;
    double density = fmax( 0.0f, fmin( 1.0f, iInfo.normal.dot( -direction )));

    Point3D i_diffuse = k_diffuse * density * i_light;
    return i_diffuse;
}
Point3D RayDirectionalLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    Point3D i_light = color;
    Point3D k_specular = iInfo.material->specular;
    // Point3D L = ( location - iInfo.iCoordinate ).unit();
    Point3D L = -direction;
    Point3D reflected = iInfo.normal * 2 * (L.dot(iInfo.normal)) - L;
    Point3D V = ( cameraPosition - iInfo.iCoordinate ).unit();

    double r_strength = fmax( 0.0f, fmin( 1.0f, reflected.dot( V )));
    double sfo = iInfo.material->specularFallOff;

    Point3D i_specular = k_specular * pow(r_strength, sfo) * i_light;
    return i_specular;
}
int RayDirectionalLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape,int& isectCount){
	return 0;
}
Point3D RayDirectionalLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL(int index){
}
