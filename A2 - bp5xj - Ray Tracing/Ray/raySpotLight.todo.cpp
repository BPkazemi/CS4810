#include <math.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D L = (iInfo.iCoordinate - location).unit();
    double cutOff = cos( cutOffAngle );

    if ( direction.dot(L) > cutOff ) {
        Point3D numer = color * ( pow(direction.dot( L ), dropOffRate ));
        double denom = constAtten + linearAtten*distance + quadAtten*pow(distance, 2.0);
        Point3D i_light = numer / denom;

        Point3D k_diffuse = iInfo.material->diffuse;
        Point3D direction = (location - iInfo.iCoordinate).unit();
        double density = fmax( 0.0f, fmin( 1.0f, iInfo.normal.dot( direction )));

        Point3D i_diffuse = k_diffuse * density * i_light;
        return i_diffuse;
    }
    return Point3D(0.0, 0.0, 0.0);
}
Point3D RaySpotLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
    double distance = (location - iInfo.iCoordinate).length();
    Point3D L = ( location - iInfo.iCoordinate ).unit();
    double cutOff = cos( cutOffAngle );

    if ( direction.dot( -L ) > cutOff ) {
        Point3D numer = color * ( pow(direction.dot( -L ), dropOffRate ));
        double denom = constAtten + linearAtten*distance + quadAtten*pow(distance, 2.0);
        Point3D i_light = numer / denom;

        Point3D k_specular = iInfo.material->specular;
        Point3D R = iInfo.normal * 2.0f * (L.dot(iInfo.normal)) - L;
        Point3D V = ( cameraPosition - iInfo.iCoordinate ).unit();

        double percent_specular = fmax( 0.0f, fmin( 1.0f, R.dot( V )));
        double sfo = iInfo.material->specularFallOff;

        Point3D i_specular = k_specular * pow(percent_specular, sfo) * i_light;
        return i_specular;
    }
    return Point3D(0.0, 0.0, 0.0);
}
int RaySpotLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape,int& isectCount){
    // TODO: Make use of isectCount
    Point3D L = ( location - iInfo.iCoordinate ).unit();
    double length = ( location - iInfo.iCoordinate ).length();
    Ray3D iRay = *(new Ray3D( iInfo.iCoordinate, L ));

    RayIntersectionInfo dummyInfo;
    double t = shape->intersect( iRay, dummyInfo, length );
    isectCount += 1;
    return (t == -1) ? 1 : 0;
}
Point3D RaySpotLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySpotLight::drawOpenGL(int index){
}
