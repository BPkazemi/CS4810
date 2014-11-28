#include <math.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "rayDirectionalLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
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
// NOTE: (0, 0, 1, 0) creates a directional light pointing in the negative z axis
void RayDirectionalLight::drawOpenGL(int index){
    glEnable( GL_LIGHTING );

    // Position & color
    GLfloat light_ambient[] = { color.p[0], color.p[1], color.p[2] };
    GLfloat light_diffuse[] = { color.p[0], color.p[1], color.p[2] };
    GLfloat light_specular[] = { color.p[0], color.p[1], color.p[2] };
    GLfloat light_position[] = { direction.p[0], direction.p[1], direction.p[2], 0.0 };

    glLightfv( GL_LIGHT0 + index, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0 + index, GL_SPECULAR, light_specular );
    glLightfv( GL_LIGHT0 + index, GL_POSITION, light_position );

    glEnable( GL_LIGHT0 + index );
}
