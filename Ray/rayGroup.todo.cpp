#include <stdlib.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "rayGroup.h"
#include <math.h>


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
int RayGroup::drawReflection( int materialIndex ) {
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
                glScalef(1.0, -1.0, 1.0);
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
                    glScalef(1.0, -1.0, 1.0);
                    curShape->drawOpenGL( materialIndex );
                glPopMatrix();

            }
        glEndList();

        glCallList( openGLCallListID );
    }

    return -1;
}

int RayGroup::drawOpenGL(int materialIndex){
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
