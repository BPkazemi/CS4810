#include <stdlib.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "rayGroup.h"
#include <math.h>
#define PI_ 3.14159265358979323846

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
void accFrustum(GLdouble left, GLdouble right, GLdouble bottom,
        GLdouble top, GLdouble near, GLdouble far, GLdouble pixdx, 
        GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, 
        GLdouble focus) {
    GLdouble xwsize, ywsize; 
    GLdouble dx, dy;
    GLint viewport[4];

    glGetIntegerv (GL_VIEWPORT, viewport);

    xwsize = right - left;
    ywsize = top - bottom;
    dx = -(pixdx*xwsize/(GLdouble) viewport[2] + 
            eyedx*near/focus);
    dy = -(pixdy*ywsize/(GLdouble) viewport[3] + 
            eyedy*near/focus);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (left + dx, right + dx, bottom + dy, top + dy, 
            near, far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (-eyedx, -eyedy, 0.0);
}

void accPerspective(GLdouble fovy, GLdouble aspect, 
        GLdouble near, GLdouble far, GLdouble pixdx, GLdouble pixdy,
        GLdouble eyedx, GLdouble eyedy, GLdouble focus) {
    GLdouble fov2,left,right,bottom,top;
    fov2 = ((fovy*PI_) / 180.0) / 2.0;
    
    top = near / (fcos(fov2) / fsin(fov2));
    bottom = -top;
    right = top * aspect;
    left = -right;
    
    accFrustum (left, right, bottom, top, near, far,
            pixdx, pixdy, eyedx, eyedy, focus);
}
*/

int RayGroup::drawOpenGL(int materialIndex){
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    /* Transparency sorting */
    /* See: http://stackoverflow.com/questions/15831877/rendering-translucent-objects-in-a-complex-scene */
    /*
    RayShape* ordShapes[ sNum ];
    RayShape* curShape;
    int opaquePointer = 0, transPointer = (sNum)-1;
    for ( int i = 0; i < sNum; i++ ) {
        curShape = shapes[i];
        if( curShape->getShapeMaterial() != NULL ) {
            ordShapes[0] = curShape;
        }
        if ( (curShape->getShapeMaterial())->transparent.p[0] == 0.0 ) {
            ordShapes[opaquePointer++] = curShape;
        } else {
            ordShapes[transPointer--] = curShape;
        }
    }
    glDepthMask( GL_FALSE );  // Depth buffer is read-only 
    */

    /*
    glClear(GL_ACCUM_BUFFER_BIT);
    for( jitter = 0; jitter < ACSIZE; jitter++ ) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        accPerspective( 50.0,
                (GLdouble) viewport[2]/(GLdouble) viewport[3],
                1.0, 15.0, j8[jitter].x, j8[jitter].y, 0.0, 0.0, 1.0);
    */
        /* Draw objects */
        // glMatrixMode( GL_MODELVIEW );
        if ( openGLCallListID == 0 ) {
            // Draw shapes as before
            RayShape* curShape;
            for ( int i = 0; i < sNum; i++ ) {
                curShape = shapes[i];

                glPushMatrix();
                    glMultMatrixd( (GLdouble *) getMatrix().m );
                    curShape->drawOpenGL( materialIndex );
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
        /* End draw objects */


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
