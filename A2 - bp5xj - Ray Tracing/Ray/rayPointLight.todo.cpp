#include <math.h>
#include <GL/glut.h>
#include "rayPointLight.h"
#include "rayScene.h"

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
    // TODO: Make use of isectCount
    Point3D L = ( location - iInfo.iCoordinate ).unit();
    double length = ( location - iInfo.iCoordinate ).length();
    Ray3D iRay = *(new Ray3D( iInfo.iCoordinate, L ));

    RayIntersectionInfo dummyInfo;
    double t = shape->intersect( iRay, dummyInfo, length );  
    isectCount += 1;
    return (t == -1) ? 1 : 0;
}
Point3D RayPointLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){ 
    Point3D transAccum = *(new Point3D(1.0, 1.0, 1.0));
    RayIntersectionInfo iShadowInfo;

    Point3D L = ( location - iInfo.iCoordinate ).unit();
    double length = ( location - iInfo.iCoordinate ).length();
    Ray3D iRay = *( new Ray3D( iInfo.iCoordinate + L*EPSILON, L ));

    double t = shape->intersect( iRay, iShadowInfo, length );  
    if( t != -1.0 ) {
        Point3D kTrans = iShadowInfo.material->transparent;

        if ( kTrans[0] > cLimit.p[0] &&
                kTrans[1] > cLimit.p[1] &&
                kTrans[2] > cLimit.p[2] ) {
            transAccum *= kTrans * transparency( iShadowInfo, shape, cLimit/kTrans );
        }
    }
    return transAccum;
    /*
    areaLight = new RaySphere();
    areaLight->center = location;
    areaLight->radius = lightRadius;

    int n = 5;
    bool softShadows = false;

    Point3D transAccum = *(new Point3D(1.0, 1.0, 1.0));
    Point3D softShadow = *(new Point3D(0.0, 0.0, 0.0));
    RayIntersectionInfo iShadowInfo;
    Point3D L;
    Point3D curLocation;

    for ( int i = 0; i < n; i++) {
        Point3D start = Point3D( areaLight->center.p[0] - lightRadius,
                areaLight->center.p[1] - lightRadius,
                areaLight->center.p[2] - lightRadius );
        double incr = i * ((2.0 * lightRadius)/5.0);
        Point3D amount = Point3D( incr, incr, incr );

        curLocation = start + amount;
        L = ( curLocation - iInfo.iCoordinate ).unit();
        double length = ( curLocation - iInfo.iCoordinate ).length();
        Ray3D iRay = *( new Ray3D( iInfo.iCoordinate + L*EPSILON, L ));

        double t = shape->intersect( iRay, iShadowInfo, length );  
        if( t != -1 ) {
            Point3D kTrans = iShadowInfo.material->transparent;

            if ( kTrans[0] > cLimit.p[0] &&
                    kTrans[1] > cLimit.p[1] &&
                    kTrans[2] > cLimit.p[2] ) {
                transAccum *= kTrans * transparency( iShadowInfo, shape, cLimit/kTrans );
                softShadow += transAccum;
                softShadows = true;
            }
        }

    }
    return (softShadows) ? (softShadow / n) : Point3D(1.0, 1.0, 1.0);
    */
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL(int index){
}
