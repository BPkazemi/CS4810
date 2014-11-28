#include <math.h>
#include <GLUT/glut.h>
#include "rayScene.h"
#include "rayCone.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCone::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}

BoundingBox3D RayCone::setBoundingBox(void){
	Point3D p;
	p=Point3D(radius,height/2,radius);
	bBox=BoundingBox3D(center+p,center-p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayCone::drawOpenGL(int materialIndex){
    material->drawOpenGL();
    glDisable(GL_CULL_FACE);

    GLUquadricObj *qobj = gluNewQuadric();

    glPushMatrix();
        glTranslatef( center.p[0], center.p[1], center.p[2] );
        gluCylinder( qobj, radius, 0, height, openGLComplexity, openGLComplexity );
        gluDisk( qobj, 0, radius, openGLComplexity, openGLComplexity );
    glPopMatrix();

    glEnable(GL_CULL_FACE);

	return -1;
}
