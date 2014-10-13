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
    double density = iInfo.normal.dot( -direction ); 

    Point3D i_diffuse = k_diffuse * density * i_light;
    return i_diffuse;
}
Point3D RayDirectionalLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
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
