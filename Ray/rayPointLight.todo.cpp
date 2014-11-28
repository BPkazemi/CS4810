#include <math.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
}
Point3D RayPointLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
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
    glEnable( GL_LIGHTING );

    // Position & color
    GLfloat light_position[] = { location.p[0], location.p[1], location.p[2], 1.0 };
    GLfloat light_diffuse[] = { color.p[0], color.p[1], color.p[2] };
    GLfloat light_ambient[] = { color.p[0], color.p[1], color.p[2] };
    GLfloat light_specular[] = { color.p[0], color.p[1], color.p[2] };

    glLightfv( GL_LIGHT0 + index, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0 + index, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT0 + index, GL_SPECULAR, light_specular );
    glLightfv( GL_LIGHT0 + index, GL_POSITION, light_position );

    // Attenuation
    glLightf( GL_LIGHT0+index, GL_CONSTANT_ATTENUATION, constAtten );
    glLightf( GL_LIGHT0+index, GL_LINEAR_ATTENUATION, linearAtten );
    glLightf( GL_LIGHT0+index, GL_QUADRATIC_ATTENUATION, quadAtten );

    glEnable( GL_LIGHT0 + index );
}
