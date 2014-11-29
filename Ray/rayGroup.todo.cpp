#include <stdlib.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "rayGroup.h"
#include <math.h>

#define drawOneLine(x1,y1,x2,y2)  glBegin(GL_LINES);  \
       glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	double minDistance = mx;

	for (int i = 0; i < sNum; i++) {
		double distance = shapes[i]->intersect(ray, iInfo, minDistance);
		if (distance >= 0) {
			minDistance = distance;
		}
	}

	return minDistance;
}

BoundingBox3D RayGroup::setBoundingBox(void){
	Point3D* pList;
	BoundingBox3D tBBox;
	pList=new Point3D[sNum*2];
	for(int i=0;i<sNum;i++){
		tBBox=shapes[i]->setBoundingBox();
		pList[2*i  ]=tBBox.p[0];
		pList[2*i+1]=tBBox.p[1];
	}
	tBBox=BoundingBox3D(pList,sNum*2);

	delete[] pList;
	bBox=tBBox.transform(getMatrix());
	return bBox;
}

int StaticRayGroup::set(void){
	return 1;
}
//////////////////
// OpenGL stuff //
//////////////////
int RayGroup::getOpenGLCallList(void){
    /* Generate a single call list for all children in the raygroup */
    return glGenLists(1);
}


/*
void mirrorMatrix(GLfloat m[4][4],  // OUT: Resulting matrix
        GLfloat p[3],  // IN: Point on the plane
        GLfloat v[3]) {  // IN: Plane perpendicular direction
    GLfloat dot = p[0]*v[0] + p[1]*v[1] + p[2]*v[2];

    m[0][0] = 1 - 2*v[0]*v[0];
    m[1][0] = - 2*v[0]*v[1];
    m[2][0] = - 2*v[0]*v[2];
    m[3][0] = 2*dot*v[0];

    m[0][1] = - 2*v[1]*v[0];
    m[1][1] = 1 - 2*v[1]*v[1];
    m[2][1] = - 2*v[1]*v[2];
    m[3][1] = 2*dot*v[1];

    m[0][2] = - 2*v[2]*v[0];
    m[1][2] = - 2*v[2]*v[1];
    m[2][2] = 1 - 2*v[2]*v[2];
    m[3][2] = 2*dot*v[2];

    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;
    m[3][3] = 1;
}
*/
void mirror( float size ) {
    glBegin(GL_QUADS);
        glVertex3f( -size, 0, -size );
        glVertex3f( -size, 0, size );
        glVertex3f( size, 0, size );
        glVertex3f( size, 0, -size );
    glEnd();
}
void mirrorOutline( float size ) {
    glPushMatrix();
        glLineWidth(1.0);
        glRotatef(90.0, 1, 0, 0);
        drawOneLine( -size, -size, -size, size );
        drawOneLine( -size, size, size, size );
        drawOneLine( size, size, size, -size );
        drawOneLine( size, -size, -size, -size );
    glPopMatrix();
}
void RayGroup::drawScene( int materialIndex ) {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    /* Draw objects */
    if ( openGLCallListID == 0 ) {
        // Draw shapes as before
        RayShape* curShape;
        for ( int i = 0; i < sNum; i++ ) {
            curShape = shapes[i];

            glPushMatrix();
                glMultMatrixd( (GLdouble *) getMatrix().m );
                curShape->drawOpenGL( materialIndex );
                // TODO: Flip here, too?
            glPopMatrix();
        }
    } else {
        // Draw the call-list using the call-list ID
        glNewList( openGLCallListID, GL_COMPILE );
            RayShape* curShape;
            for ( int i = 0; i < sNum; i++ ) {
                curShape = shapes[i];

                glPushMatrix();
                    glMultMatrixd( (GLdouble *) getMatrix().m );
                    curShape->drawOpenGL( materialIndex );
                glPopMatrix();

            }
        glEndList();

        glCallList( openGLCallListID );
    }
}

int RayGroup::drawOpenGL(int materialIndex){
    drawScene(materialIndex);

    glMatrixMode( GL_MODELVIEW );
    glClear( GL_STENCIL_BUFFER_BIT );

    glEnable( GL_STENCIL_TEST );
        // 1. Set the mirror's pixels to 1 in the stencil buffer
        glStencilFunc( GL_ALWAYS, 0x1, 0xFF );
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
        glStencilMask(0xFF);

        // Use the mirror to set the stencil buffer, but don't draw it
        glColorMask(0,0,0,0);
        glDepthMask(GL_FALSE);
        mirror(1.0);
        glDepthMask(GL_TRUE);

        // 2. Reflect the scene, only drawing in the valid stencil area
        glColorMask(1,1,1,1);
        glStencilFunc( GL_EQUAL, 1, 0xFF );
        glStencilMask( 0x00 );  // Disable stencil buffer writing

        // Draw reflection
        glPushMatrix();
            glDisable(GL_CULL_FACE);
            glScalef(-1.0, -1.0, 1.0);
            drawScene( materialIndex );
        glPopMatrix();
        
        // Draw mirror
        glDepthMask(GL_FALSE);
        mirrorOutline(1.0);
        // mirror(1.0);
        glDepthMask(GL_TRUE);
    glDisable( GL_STENCIL_TEST );
    mirrorOutline(1.0);

    /*
    glDisable(GL_CULL_FACE);
    drawScene( materialIndex );
    glEnable(GL_CULL_FACE);
    */

    /*
    glClearStencil( 0 );
    glClear( GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    glEnable( GL_DEPTH_BUFFER_BIT );
    glDisable( GL_STENCIL_TEST );
    glColorMask(0,0,0,0);  // Disable color buffer

    drawScene( materialIndex );

    // Tag mirror's visible pixels with a stencil value 1
    glEnable( GL_STENCIL_TEST );
    glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
    glStencilFunc( GL_ALWAYS, 1, ~0 );
    mirror( 1.0 );

    // Set depth range to farthest possible value
    glStencilFunc( GL_EQUAL, 1, ~0 );
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    */
    /* End draw objects */

    /*
    glEnable(GL_STENCIL_TEST);
        glClearStencil( 0 );
        glClear( GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
        glStencilFunc( GL_ALWAYS, 0x1, 0xFF );  // Set stencils to 1
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
        glStencilMask( 0xFF );  // Enable stencil buffer writing
        glDepthMask(GL_FALSE);

        mirror( 1.0 );

        glStencilFunc(GL_EQUAL, 0x1, 0xFF);  // Test passes if stencil value is 1
        glStencilMask(0x00);  // Disable stencil buffer writing
        glDepthMask(GL_TRUE);

        for( int i = 0; i < sNum; i++ ) {
            RayShape *curShape = shapes[i];
            // Invert objects about mirror plane
            glPushMatrix();
                glDisable(GL_CULL_FACE);
                glMultMatrixd( (GLdouble *) getMatrix().m );
                glScalef(1.0, -1.0, 1.0);
                curShape->drawOpenGL( materialIndex );
                glEnable(GL_CULL_FACE);
            glPopMatrix();
        }
    glDisable(GL_STENCIL_TEST);
    */

    glDepthMask( GL_TRUE );
    glDisable(GL_BLEND);
	return -1; // TODO: return something real
}

/////////////////////
// Animation Stuff //
/////////////////////
Matrix4D ParametrizedEulerAnglesAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedQuaternionAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
