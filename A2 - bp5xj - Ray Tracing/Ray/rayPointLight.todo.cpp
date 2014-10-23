#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include "rayPointLight.h"
#include "rayScene.h"

extern const double EPSILON;
////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D i_light = color / (constAtten + linearAtten*distance + quadAtten*pow(distance,2.0));

    Point3D k_diffuse = iInfo.material->diffuse;
    Point3D direction = (location - iInfo.iCoordinate).unit();
    double density = fmax( 0.0f, fmin( 1.0f, iInfo.normal.dot( direction )));

    Point3D i_diffuse = k_diffuse * density * i_light;
    return i_diffuse;
}
Point3D RayPointLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D i_light = color / (constAtten + linearAtten*distance + quadAtten*pow(distance,2.0));

    Point3D k_specular = iInfo.material->specular;
    Point3D L = ( location - iInfo.iCoordinate ).unit();
    Point3D R = iInfo.normal * 2.0 * (L.dot(iInfo.normal)) - L;
    Point3D V = ( cameraPosition - iInfo.iCoordinate ).unit();

    double percent_specular = fmax( 0.0f, fmin( 1.0f, R.dot( V )));
    double sfo = iInfo.material->specularFallOff;

    Point3D i_specular = k_specular * pow(percent_specular, sfo) * i_light;
    return i_specular;
}
int RayPointLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape,int& isectCount){
    Point3D L = ( location - iInfo.iCoordinate ).unit();
    double length = ( location - iInfo.iCoordinate ).length();
    Ray3D iRay = Ray3D( iInfo.iCoordinate, L );

    RayIntersectionInfo dummyInfo;
    double t = shape->intersect( iRay, dummyInfo, length );  
    isectCount += 1;
    return (t == -1) ? 1 : 0;
}
Point3D RayPointLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){ 
    double s = 2.0;
    float range = 0.1;
    float incr = range / s;
    int numSamples = 0;

    Point3D transAccum = Point3D(0.0, 0.0, 0.0);

    for ( float i = -(range/2.0); i <= (range/2.0); i += incr ) {
        for ( float j = -(range/2.0); j <= (range/2.0); j += incr ) {
            numSamples += 1;
            Point3D curLocation = location + Point3D(i, 0, j);

            Point3D L = ( curLocation - iInfo.iCoordinate ).unit();
            double length = ( curLocation - iInfo.iCoordinate ).length();
            Ray3D iRay = Ray3D( iInfo.iCoordinate + L*EPSILON, L );

            Point3D curTrans = Point3D(1.0, 1.0, 1.0);
            RayIntersectionInfo iShadowInfo;
            double t = shape->intersect( iRay, iShadowInfo, length );  
            if( t != -1.0 ) {
                Point3D kTrans = iShadowInfo.material->transparent;

                if ( kTrans[0] > cLimit.p[0] &&
                        kTrans[1] > cLimit.p[1] &&
                        kTrans[2] > cLimit.p[2] ) {
                    curTrans *= kTrans * transparency( iShadowInfo, shape, cLimit/kTrans );
                } else {
                    curTrans = Point3D(0.0, 0.0, 0.0);
                }
            }
            transAccum += (curTrans);
        }
    }
    return transAccum / numSamples;
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL(int index){
}
