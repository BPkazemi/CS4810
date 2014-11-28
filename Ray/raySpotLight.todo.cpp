#include <math.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "rayScene.h"
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	return Point3D();
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
    // TODO: Verify correctness with a more tesselated object
    glEnable( GL_LIGHTING );

    // Position & color
    GLfloat light_position[] = { location.p[0], location.p[1], location.p[2], 1.0 };
    GLfloat light_diffuse[] = { color.p[0], color.p[1], color.p[2] };
    GLfloat light_specular[] = { color.p[0], color.p[1], color.p[2] };

    glLightfv( GL_LIGHT0 + index, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0 + index, GL_SPECULAR, light_specular );
    glLightfv( GL_LIGHT0 + index, GL_POSITION, light_position );

    // Attenuation
    glLightf( GL_LIGHT0+index, GL_CONSTANT_ATTENUATION, constAtten );
    glLightf( GL_LIGHT0+index, GL_LINEAR_ATTENUATION, linearAtten );
    glLightf( GL_LIGHT0+index, GL_QUADRATIC_ATTENUATION, quadAtten );

    // Spotlight direction, cutoff, & dropoff
    GLfloat spot_direction[] = { direction.p[0], direction.p[1], direction.p[2] };
    glLightfv( GL_LIGHT0 + index, GL_SPOT_DIRECTION, spot_direction );
    glLightf( GL_LIGHT0 + index, GL_SPOT_CUTOFF, (cutOffAngle * 180.0 / PI)); 
    glLightf( GL_LIGHT0 + index, GL_SPOT_EXPONENT, dropOffRate );

    glEnable( GL_LIGHT0 + index );
}
