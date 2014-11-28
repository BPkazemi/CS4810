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
