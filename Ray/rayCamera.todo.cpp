#include <GLUT/glut.h>
#include <math.h>
#include "rayCamera.h"



//////////////////
// OpenGL stuff //
//////////////////
void RayCamera::drawOpenGL(void){
    /* Viewing Transformation  */
    glMatrixMode( GL_MODELVIEW );
    gluLookAt( position.p[0], position.p[1], position.p[2],
            position.p[0] + direction.p[0], position.p[1] + direction.p[1], position.p[2] + direction.p[2],
            up.p[0], up.p[1], up.p[2]);
}
void RayCamera::rotateUp(Point3D center,float angle){
    // To implement a rotation of "angle" degrees, around the up axis, about the point center
    /* ------- Approach 1 ---------
     * -- Translation & Rotation --
     * -----------------------------
    glMatrixMode( GL_MODELVIEW );
    glTranslatef( position.p[0], position.p[1], position.p[2] );
    glRotatef( angle, up.p[0], up.p[1], up.p[2] );
    */

    /* ----------- Approach 2 ------------
     * --- Rodrigues' Rotation Formula ---
     * -----------------------------------

    float ca = cos( angle );
    float sa = sin( angle );

    Point3D rot = direction*ca + 
        up.crossProduct(direction)*sa + 
        up*(up.dot(direction))*(1.0-ca);

    Point3D newUp = -(rot.crossProduct( right ));

    gluLookAt( center.p[0], center.p[1], center.p[2], 
            center.p[0] + rot.p[0], center.p[1] + rot.p[1], center.p[2] + rot.p[2],
            newUp.p[0], newUp.p[1], newUp.p[2] );
    */
}
void RayCamera::rotateRight(Point3D center,float angle){
    // To implement a rotation of "angle" degrees, around the right axis, about the point center
    /* ------- Approach 1 ---------
     * -- Translation & Rotation --
     * -----------------------------
    glMatrixMode( GL_MODELVIEW );
    glTranslatef( position.p[0], center.p[1], center.p[2] );
    glRotatef( angle, up.p[0], up.p[1], up.p[2] );
    */
    

    /* ---------- Approach 2 -----------
     * -- Rodrigues' Rotation Formula --
     * ---------------------------------

    float ca = - cos( angle );
    float sa = sin( angle );

    Point3D rot = direction*ca + 
        up.crossProduct(direction)*sa + 
        up*(up.dot(direction))*(1.0-ca);

    Point3D newRight = -(rot.crossProduct( up ));

    gluLookAt( center.p[0], center.p[1], center.p[2], 
            center.p[0] + rot.p[0], center.p[1] + rot.p[1], center.p[2] + rot.p[2],
            newRight.p[0], newRight.p[1], newRight.p[2] );
    */

    /*
     * Useful link: http://gamedev.stackexchange.com/questions/20758/how-can-i-orbit-a-camera-about-its-target-point
     */
}
void RayCamera::moveForward(float dist){
}
void RayCamera::moveRight(float dist){
}
void RayCamera::moveUp(float dist){
}
