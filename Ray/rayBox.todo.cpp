#include <math.h>
#include <GLUT/glut.h>
#include "rayScene.h"
#include "rayBox.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}
BoundingBox3D RayBox::setBoundingBox(void){
	bBox=BoundingBox3D(center-(length/2),center+(length/2));
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayBox::drawOpenGL(int materialIndex){
    material->drawOpenGL();
    glDisable(GL_CULL_FACE);

    // Fix current missing face
    // Try a more sensible approach
    glPushMatrix();
        glTranslatef( center.p[0], center.p[1], center.p[2] );
        // 2 faces along the z-axis
        glPushMatrix();
            glTranslatef( 0, 0, -(length.p[2]/2.0));
            glRectf( 
                    - (length.p[0]/2.0), - (length.p[1]/2.0), 
                    + (length.p[0]/2.0), + (length.p[1]/2.0) 
                    );
        glPopMatrix();

        glPushMatrix();
            glTranslatef( 0, 0, (length.p[2]/2.0));
            glRectf( 
                    - (length.p[0]/2.0), - (length.p[1]/2.0), 
                    + (length.p[0]/2.0), + (length.p[1]/2.0) 
                    );
        glPopMatrix();

        // 2 faces along the x-axis
        glPushMatrix();
            glTranslatef( -(length.p[0])/2.0, 0, 0 );
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glRectf( 
                    - (length.p[2]/2.0), - (length.p[1]/2.0), 
                    + (length.p[2]/2.0), + (length.p[1]/2.0) 
                    );
        glPopMatrix();

        glPushMatrix();
            glTranslatef( (length.p[0])/2.0, 0, 0 );
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glRectf( 
                    - (length.p[2]/2.0), - (length.p[1]/2.0), 
                    + (length.p[2]/2.0), + (length.p[1]/2.0) 
                    );
        glPopMatrix();

        // 2 faces along the y-axis
        glPushMatrix();
            glTranslatef( 0, -length.p[1]/2.0, 0 );
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            // glTranslatef( center.p[0], center.p[1], center.p[2] - (length.p[1]/2.0) );
            glRectf( 
                    - (length.p[0]/2.0), - (length.p[2]/2.0), 
                    + (length.p[0]/2.0), + (length.p[2]/2.0) 
                    );
        glPopMatrix();

        glPushMatrix();
            glTranslatef( 0, length.p[1]/2.0, 0 );
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            // glTranslatef( center.p[0], center.p[1], center.p[2] + (length.p[1]/2.0) );
            glRectf( 
                    - (length.p[0]/2.0), - (length.p[2]/2.0), 
                    + (length.p[0]/2.0), + (length.p[2]/2.0) 
                    );
        glPopMatrix();
    glPopMatrix();

    glEnable(GL_CULL_FACE);

	return -1;
}
