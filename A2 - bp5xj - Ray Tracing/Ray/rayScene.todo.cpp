#include "rayScene.h"
#include <GL/glut.h>
#include <math.h>
#include <float.h>

extern const double EPSILON;
///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect(Point3D v,Point3D n){
    Point3D r = v - n * 2.0 * ( n.dot(v) );
	return r;
}

int RayScene::Refract(Point3D v,Point3D n,double ir,Point3D& refract){
    // TODO: Calculate theta_i, theta_r, and T correctly
    /*
    double theta_i, theta_r;
    double dotProduct = n.unit().dot( -v.unit() );
    theta_i = acos( dotProduct / ( v.unit().length() * n.unit().length() ) );
    theta_r = asin( ir * sin(theta_i) );

    refract = n*( ir*cos(theta_i) - cos(theta_r) ) - v*ir;
    */ 
    refract = v;

	return 1;
}

Ray3D RayScene::GetRay(RayCamera* camera,int i,int j,int width,int height){
    // TODO: Negative camera positions break the ray tracer...
    // TODO: Changing the camera's z-position breaks the ray tracer
    Point3D p0 = camera->position;
    Point3D up = (camera->up).unit();
    Point3D right = (camera->right).unit();
    double theta = ( camera->heightAngle ) / 2.0;
    double aspectRatio = (double) camera->aspectRatio;
    Point3D direction = (camera->direction).unit();

    double widthAngle = camera->heightAngle * aspectRatio;
    double alpha = widthAngle / 2.0;

    Point3D verticalDir = up*tan(theta);
    Point3D horizontalDir = right*(tan(alpha));
    Point3D p1 = p0 + direction - verticalDir - horizontalDir;
    Point3D p2 = p0 + direction + verticalDir - horizontalDir;
    Point3D p3 = p0 + direction - verticalDir + horizontalDir;

    Point3D upAmount = (p3-p1) * (((float) i + 0.5)/(float) height);
    Point3D rightAmount = (p2-p1) * (((float) j + 0.5)/(float) width); 

    Point3D p = p1 + upAmount + rightAmount; 
    Point3D vector = p - p0;
    return Ray3D( p0, vector.unit() ); 
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
    RayIntersectionInfo iInfo;
    Ray3D reflect, refract;
    double t = group->intersect( ray, iInfo, 0.0 );
    if ( t != -1 ) {
        /* ~~~~~~ Emissive & Ambient ~~~~~ */
        Point3D i_emissive = iInfo.material->emissive;
        Point3D i_ambient = ambient * iInfo.material->ambient;

        /* ~~~~~~ Diffuse & Specular ~~~~~ */
        Point3D i_diffuse_specular = *(new Point3D(0.0, 0.0, 0.0));
        Point3D shadow = *(new Point3D(1.0, 1.0, 1.0));
        int isectCount = 0;
        for ( int i = 0; i < lightNum; i++ ) {
            RayLight* curLight = lights[ i ];
            Point3D curDiffuse = curLight->getDiffuse( camera->position, iInfo );
            Point3D curSpecular = curLight->getSpecular( camera->position, iInfo );

            /* ~~ Shadow Term ~~ */
            for ( int k = 0; k < group->sNum; k++ ) {
                RayShape* curShape = group->shapes[k];
                shadow *= curLight->transparency( iInfo, curShape, cLimit );
            }
            i_diffuse_specular += (curDiffuse + curSpecular) * shadow;
        }

        /* ~~ Reflection & Refraction ~~  */
        // TODO: Reflection is too bright. Seems to be because of the background color or kspec?
        Point3D i_reflection = *(new Point3D(0.0, 0.0, 0.0));
        Point3D k_spec = iInfo.material->specular;
        reflect.direction = Reflect( ray.direction, iInfo.normal );
        reflect.position = iInfo.iCoordinate + reflect.direction * EPSILON;

        if ( rDepth > 0 && 
                ( k_spec[0] > cLimit[0] && 
                  k_spec[1] > cLimit[1] && 
                  k_spec[2] > cLimit[2]) ) {
            i_reflection += this->GetColor( reflect, (rDepth-1), cLimit/k_spec) * k_spec;
        }

        Point3D i_refraction = *(new Point3D(0.0, 0.0, 0.0));
        Point3D k_tran = iInfo.material->transparent;
        Refract( ray.direction, iInfo.normal, iInfo.material->refind, refract.direction );
        refract.position = iInfo.iCoordinate + refract.direction * EPSILON;
        if ( rDepth > 0 && 
                ( k_tran[0] > cLimit[0] && 
                  k_tran[1] > cLimit[1] && 
                  k_tran[2] > cLimit[2]) ) {
            i_refraction += this->GetColor( refract, (rDepth-1), cLimit/k_tran) * k_tran;
        }

        // TODO: Color of square looks a little off
        Point3D color = i_emissive + i_ambient + i_diffuse_specular + 
            i_reflection + i_refraction;
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
