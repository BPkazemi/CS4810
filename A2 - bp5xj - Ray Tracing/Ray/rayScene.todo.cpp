#include "rayScene.h"
#include <GL/glut.h>
#include <math.h>
#include <float.h>


///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect(Point3D v,Point3D n){
	return Point3D();
}

int RayScene::Refract(Point3D v,Point3D n,double ir,Point3D& refract){
	return 0;
}

Ray3D RayScene::GetRay(RayCamera* camera,int i,int j,int width,int height){
    // TODO: Image shows too far
    // TODO: Negative camera positions break the ray tracer...
    Point3D p0 = camera->position;
    Point3D up = (camera->up).unit();
    Point3D right = (camera->right).unit();
    double theta = ( camera->heightAngle ) / 2.0;
    double aspectRatio = (double) camera->aspectRatio;
    Point3D direction = (camera->direction).unit();

    Point3D verticalDir = (up*tan(theta)).unit();
    Point3D horizontalDir = (right*(tan(theta)*aspectRatio)).unit();
    Point3D p1 = p0 + direction + verticalDir - horizontalDir;
    Point3D p2 = p0 + direction + verticalDir + horizontalDir;
    Point3D p3 = p0 + direction - verticalDir - horizontalDir;
    Point3D p4 = p0 + direction - verticalDir + horizontalDir;

    Point3D upAmount = (p3-p1) * (((float) i + 0.5)/(float) height);
    Point3D rightAmount = (p2-p1) * (((float) j + 0.5)/(float) width); 

    Point3D p = p1 + upAmount + rightAmount; 
    Point3D vector = p - p0;
    return Ray3D( p0, vector.unit() ); 
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
    RayIntersectionInfo iInfo;
    double t = group->intersect( ray, iInfo, 0.0 );
    if ( t != -1 ) {
        /* ~~~~~~ Emissive & Ambient ~~~~~ */
        Point3D i_emissive = iInfo.material->emissive;
        Point3D i_ambient = ambient * iInfo.material->ambient;

        /* ~~~~~~ Diffuse & Specular ~~~~~ */
        Point3D i_diffuse = *(new Point3D(0.0, 0.0, 0.0));
        Point3D i_specular = *(new Point3D(0.0, 0.0, 0.0));
        for ( int i = 0; i < lightNum; i++ ) {
            RayLight* curLight = lights[ i ];
            Point3D curDiffuse = curLight->getDiffuse( camera->position, iInfo );
            Point3D curSpecular = curLight->getSpecular( camera->position, iInfo );

            i_diffuse += curDiffuse;
            i_specular += curSpecular;
        }


        // TODO: Color of square looks slightly off
        Point3D color = i_emissive + i_ambient + i_diffuse + i_specular;
        color.p[0] = fmax(0.0f, fmin(1.0f, color.p[0]));
        color.p[1] = fmax(0.0f, fmin(1.0f, color.p[1]));
        color.p[2] = fmax(0.0f, fmin(1.0f, color.p[2]));
        return color;
    }

    return background;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(void){
}
void RayTexture::setUpOpenGL(void){
}
