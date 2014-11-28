#include "rayScene.h"
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <math.h>


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
	return Ray3D();
}

Point3D RayScene::GetColor(Ray3D ray,int rDepth,Point3D cLimit){
	return Point3D();
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(void){
    if( tex != NULL ) {
        glBindTexture( GL_TEXTURE_2D, tex->texName[0] );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->img->width(), tex->img->height(), 0, 
                GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(&(tex->img)->pixel(0,0)));

        glEnable(GL_TEXTURE_2D);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        glBindTexture(GL_TEXTURE_2D, tex->texName[0]);

        GLfloat mat_amb[] = { 
            ambient.p[0],
            ambient.p[1],
            ambient.p[2]
        };
        GLfloat mat_diffuse[] = {
            1.0,
            1.0,
            1.0,
            (1.0 - transparent.p[0])
        };
        GLfloat mat_specular[] = {
            specular.p[0],
            specular.p[1],
            specular.p[2]
        };
        GLfloat mat_shininess[] = {
            specularFallOff
        };
        GLfloat mat_emission[] = {
            emissive.p[0],
            emissive.p[1],
            emissive.p[2]
        };

        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb );
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
        glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
    } else {
        glDisable(GL_TEXTURE_2D);

        GLfloat mat_amb[] = { 
            ambient.p[0],
            ambient.p[1],
            ambient.p[2]
        };
        GLfloat mat_diffuse[] = {
            diffuse.p[0],
            diffuse.p[1],
            diffuse.p[2],
            (1.0 - transparent.p[0])
        };
        GLfloat mat_specular[] = {
            specular.p[0],
            specular.p[1],
            specular.p[2]
        };
        GLfloat mat_shininess[] = {
            specularFallOff
        };
        GLfloat mat_emission[] = {
            emissive.p[0],
            emissive.p[1],
            emissive.p[2]
        };

        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb );
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
        glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
    }

}
void RayTexture::setUpOpenGL(void){
    glGenTextures(1, texName);
}
