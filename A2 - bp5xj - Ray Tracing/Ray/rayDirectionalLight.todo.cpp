#include <math.h>
#include <GL/glut.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

extern const double EPSILON;
////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    Point3D i_light = color;
    Point3D k_diffuse = iInfo.material->diffuse;
    double density = fmax( 0.0f, fmin( 1.0f, iInfo.normal.dot( -direction )));

    Point3D i_diffuse = k_diffuse * density * i_light;
    return i_diffuse;
}
Point3D RayDirectionalLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    Point3D i_light = color;
    Point3D k_specular = iInfo.material->specular;
    Point3D L = -direction;
    Point3D R = iInfo.normal * 2.0 * (L.dot(iInfo.normal)) - L;
    Point3D V = ( cameraPosition - iInfo.iCoordinate ).unit();

    double percent_specular = fmax( 0.0f, fmin( 1.0f, R.dot( V )));
    double sfo = iInfo.material->specularFallOff;

    Point3D i_specular = k_specular * pow(percent_specular, sfo) * i_light;
    return i_specular;
}
int RayDirectionalLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape,int& isectCount){
    // TODO: Make use of isectCount
    Point3D L = -direction;
    Ray3D iRay = Ray3D( iInfo.iCoordinate, L );

    RayIntersectionInfo dummyInfo;
    double t = shape->intersect( iRay, dummyInfo, 0.0 );  
    isectCount += 1;
    return (t == -1) ? 1 : 0;
}
Point3D RayDirectionalLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
    Point3D transAccum = Point3D(1.0, 1.0, 1.0);
    RayIntersectionInfo iShadowInfo;

    Point3D L = -direction;
    Ray3D iRay = Ray3D( iInfo.iCoordinate + L*EPSILON, L );
    double t = shape->intersect( iRay, iShadowInfo, 0.0 );  

    if( t != -1 ) {
        Point3D kTrans = iShadowInfo.material->transparent;

        if ( kTrans[0] > cLimit.p[0] &&
                kTrans[1] > cLimit.p[1] &&
                kTrans[2] > cLimit.p[2] ) {
            transAccum *= kTrans * transparency( iShadowInfo, shape, cLimit/kTrans );
        }
    }
    return transAccum;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL(int index){
}
