#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <time.h>
#include <cstdio>
#define INIT_VIEW_X 0.0    //Define initial camera position and viewing window values
#define INIT_VIEW_Y 0.0
#define INIT_VIEW_Z -4.5
#define VIEW_LEFT -2.0
#define VIEW_RIGHT 2.0
#define VIEW_BOTTOM -2.0
#define VIEW_TOP 2.0
#define VIEW_NEAR 1.0
#define VIEW_FAR 200.0
int R=1,G=0,B=0;
float AmbientLight[]={0.3,0.3,0.3,1.0};                  //Initialization values for lighting
float DiffuseLight[] ={0.8,0.8,0.8,1.0};
float SpecularLight[] ={1.0,1.0,1.0,1.0};
float SpecRef[] = {0.7,0.7,0.7,1.0};
float LightPos[] = {-50.0,50.0,100.0,1.0};
int Shine =128;
int walkX=0,walkY=0,lookX=0,lookY=0;
int world=1,oldX=-1,oldY=-1;
int doll=-1;

GLuint texture;
//Funkcija za iscrtavanje texture
GLuint LoadTexture(const char* filename, int width, int height){
       GLuint texture;
       unsigned char* data;
       FILE* file;
       file=fopen(filename, "rb");
       if(file==NULL)return 0;
       data=(unsigned char*)malloc(width * height * 3);
       fread(data,width * height * 3,1,file);
       fclose(file);
       glGenTextures(1,&texture);
       glBindTexture(GL_TEXTURE_2D,texture);
       glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
       glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
       free(data);
       return texture;
}
void boje (GLfloat b) //Funkcija koja boji poligon na osnovu unetog broja
{
     if (b==1)
        glColor3f(1.0, 0.0, 0.0 );//Red
     if (b==2)
        glColor3f(0.0, 1.0, 0.0 );//Green
     if (b==3)
        glColor3f(0.0, 0.0, 1.0 );//Blue
     if (b==4)
        glColor3f(1.0, 1.0, 0.0 );//Yellow
     if (b==5)
        glColor3f(1.0, 0.0, 1.0 );//Magenta
     if (b==6)
        glColor3f(0.0, 1.0, 1.0 );//Cyan
     if (b==7)
        glColor3f(1.0, 1.0, 1.0 );//White
     if (b==8)
        glColor3f(0.0, 0.0, 0.0 );//Black
}
void menu (int b)
{
     if(b==1)
     {
        R=1;
        G=0;
        B=0;
     }
     if(b==2)
     {
        R=0;
        G=1;
        B=0;
     }
     if(b==3)
     {
        R=0;
        G=0;
        B=1;
     }
}
void land()
{
    	glColor3f(0.0, 0.0, 1.0);
	glColor3f(R,G,B);
	glBegin(GL_QUADS);
		glVertex3f(-100.0, -100.0, 0.0);
		glVertex3f(-100.0,  100.0, 0.0);
		glVertex3f( 100.0,  100.0, 0.0);
		glVertex3f( 100.0, -100.0, 0.0);
	glEnd();
}
void kreirajMeni()
{
     glutCreateMenu(menu);
     glutAddMenuEntry("Crvena",1);
     glutAddMenuEntry("Zelena",2);
     glutAddMenuEntry("Plava",3);
     glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void eyeright()
{
    //function for the right eye
    glPushMatrix();
    glTranslatef(.17,1.1,.75);     //Specify the coordinates for the right eye
    glRotatef(-45,0,0,1);
    glScalef(.7,.7,.7);            //Specify the size of the right eye
    glColor3f(1.0,1.0,1.0);       //Specify the color of the eye
    gluSphere(gluNewQuadric(),.3,90,90);
    glPopMatrix();
}
void eyeleft()
{
    glPushMatrix();
    glTranslatef(-.17,1.1,.75);     //Specify the position for the left eye
    glRotatef(45,0,0,1);
    glScalef(.7,.7,.7);
    glColor3f(1.0,1.0,1.0);
    gluSphere(gluNewQuadric(),.3,90,90);
    glPopMatrix();
}

void legleft()
{
    glPushMatrix();
    glColor3f(0.5, 0.0, 0.0);
    glTranslatef(.3,-.5,0);     //Specify the position for the left leg
    glRotatef(-80.0,1,0,0);
    glScalef(.5,.8,.8);
    gluCylinder(gluNewQuadric(),.5,.5,.5,30,6);
    glPopMatrix();
}

void legright()
{
    glPushMatrix();
    glColor3f(0.5, 0.0, 0.0);
    glTranslatef(-.3,-.5,0);     //Specify the position for the right leg
    glRotatef(-80.0,1,0,0);
    glScalef(.5,.8,.8);
    gluCylinder(gluNewQuadric(),.5,.5,.5,30,6);
    glPopMatrix();
}

void armleft()
{
    glPushMatrix();
    glColor3f(1.0,0.0,1.0);
    glTranslatef(-.82,0,.1);     //Specify the position for the left arm
    glRotatef(90,0,1,0);
    glRotatef(-40,1,0,0);
    gluCylinder(gluNewQuadric(),.15,.15,.48,30,6);
    glPopMatrix();
}

void armright()
{
    glPushMatrix();
    glColor3f(1.0,0.0,1.0);
    glTranslatef(.82,0,.1);      //Specify the position for the right arm
    glRotatef(90,0,1,0);
    glRotatef(-140,1,0,0);
    gluCylinder(gluNewQuadric(),.15,.15,.48,30,6);
    glPopMatrix();
}

void handleft()
{
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.4);
    glTranslatef(.82,0,.1);     //Specify the position for the left hand
    glScalef(.4,.3,.3);
    gluSphere(gluNewQuadric(),.4,100,100);
    glPopMatrix();
}
void handright()
{
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.4);
    glTranslatef(-.82,0,.1);    //Specify the position for the right hand
    glScalef(.4,.3,.3);
    gluSphere(gluNewQuadric(),.4,100,100);
    glPopMatrix();
}

void mouth()
{
    glPushMatrix();
    glTranslatef(0,.78,.74);
    glScalef(.4,.4,.1);
    glColor3f(0.5, 0.0, 0.0);
    gluSphere(gluNewQuadric(),.4,100,100);
    glPopMatrix();
}
void eyebrowleft()
{
    glPushMatrix();
    glTranslatef(-.3,1.5,.97);;
    glRotatef(50,0,3,0);
    glRotatef(20,3,1,0);
    glColor3f(0.4, 0.4, 0.4);
    gluCylinder(gluNewQuadric(),.05,.01,.3,5,6);
    glPopMatrix();
}

void eyebrowright()
{
    glPushMatrix();
    glTranslatef(.3,1.5,.97);
    glRotatef(250,0,3,0);
    glRotatef(20,3,1,0);
    gluCylinder(gluNewQuadric(),.05,.01,.3,5,6);
    glPopMatrix();
}
void neckring()
{
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.0);
    glTranslatef(0,.5,0);
    glScalef(.59,.59,.59);
    glRotatef(90.0,1,0,0);
    glutSolidTorus(.1,1.0,20,20);
    glPopMatrix();
}
void head()
{
    glPushMatrix();
    glTranslatef(0,1.2,0);
    glScalef(.8,.8,.9 );
    glColor3f(1.0,0.8,0.6);
    gluSphere(gluNewQuadric(),1,100,100);
    glPopMatrix();
}
void maintopball()
{
    glPushMatrix();
    glTranslatef(0.5,2.0,0);
    glScalef(.9,.9,.9);
    gluSphere(gluNewQuadric(),.18,100,100);
    glPopMatrix() ;
}
void maintopball1()
{
    glPushMatrix();
    glTranslatef(-0.5,2.0,0);
    glScalef(.9,.9,.9);
    gluSphere(gluNewQuadric(),.18,100,100);
    glPopMatrix() ;
}
void hatring()
{
    glPushMatrix();
    glTranslatef(0,1.4,0);
    glScalef(.84,.84,.84);
    glRotatef(50.0,1,0,0);
    glutSolidTorus(.1,1.0,20,20);
    glPopMatrix();
}

void bellyCoatbottom()
{
    glPushMatrix();
    glTranslatef(0,-.2,0);
    glScalef(1,.7,1);
    glRotatef(50.0,2,0,0);
    gluDisk(gluNewQuadric(),0,.8,30,30);
    glPopMatrix();
}

void BellyCoat()
{
    glPushMatrix();
    glTranslatef(0,.5,0);
    glScalef(1,.7,1);
    glRotatef(90.0,1,0,0);
    gluCylinder(gluNewQuadric(),.6,.8,1,100,100);
    glPopMatrix();
}

void pupilleft()
{
    glPushMatrix();
    glTranslatef(-.17,1.1,.88);
    glScalef(.8,.8,.9);
    gluSphere(gluNewQuadric(),.1,100,100);
    glPopMatrix();
}

void pupilright()
{
    glPushMatrix();
    glTranslatef(.17,1.1,.88);
    glScalef(.8,.8,.9);
    gluSphere(gluNewQuadric(),.1,100,100);
    glPopMatrix();
}

void middlebutton()
{
    glPushMatrix()  ;
    glTranslatef(-.1,.15,.98);
    glScalef(1.9,1.9,1.9);
    gluSphere(gluNewQuadric(),.04,100,100);
    glPopMatrix();
}
void bottombutton()
{
    glPushMatrix();
    glTranslatef(-.1,-.1,.92);
    glScalef(1.9,1.9,1.9);
    glColor3f(0.0,0.0,0.0);
    gluSphere(gluNewQuadric(),.04,100,100);
    glPopMatrix();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear the window
    glColor3ub(50, 50, 150);//Change the draw color to slate blue
    glPushMatrix();//Save viewing matrix state
    if(world==1)
	{
       	      glTranslatef(walkX,-1,walkY);
                  glRotatef(lookY,0,1,0);
                  glRotatef(lookX,1,0,0);
	}

	glEnable(GL_TEXTURE_2D);
    //*******************Doll***********************
    glPushMatrix();
    glTranslatef(-1,0,-6);

     if(doll==1)
	{
                  glTranslatef(walkX,-1,walkY);
                  glRotatef(lookY,0,1,0);
                  glRotatef(lookX,1,0,0);
	}
    eyeright();
    eyeleft();
    eyebrowleft();
    eyebrowright();
    land();
    glColor3f(0.0,1.0,0.0);
    neckring();
    glColor3ub(50,40,60);
    legright();
    legleft();
    glColor3ub(255,90,0);
    armleft();
    armright();
    hatring();
    bellyCoatbottom();
    glColor3ub(0,185,0);
    handleft();
    handright();
    maintopball();
    maintopball1();
    mouth();
    glColor3ub(255,222,173);
    head();
    glColor3f(0.0,0.0,0.0);
    middlebutton();
    bottombutton();
    pupilleft();
    pupilright();
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexCoord2d(0.0, 0.0);BellyCoat();
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

void SetupRend()
{
    glClearColor(0.7,0.7,1.0,1.0);
    glEnable(GL_DEPTH_TEST);         //Enable depth testing
    glEnable(GL_LIGHTING);             //Enable lighting
    glLightfv(GL_LIGHT0,GL_AMBIENT,AmbientLight);//Set up and enable light zero
    glLightfv(GL_LIGHT0,GL_DIFFUSE,DiffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,SpecularLight);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);                   //Enable color tracking
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);//Set material to follow
    glMaterialfv(GL_FRONT,GL_SPECULAR,SpecRef);//Set specular reflectivity and shine
    glMateriali(GL_FRONT,GL_SHININESS,Shine);
}

void walk(int key,int x,int y)                                      //change positions using arrow keys
{
    if(key==GLUT_KEY_UP)    walkY+=1;
    if(key==GLUT_KEY_DOWN)  walkY-=1;
    if(key==GLUT_KEY_RIGHT) walkX+=1;
    if(key==GLUT_KEY_LEFT)  walkX-=1;
    if(key==GLUT_KEY_F10)    world=-world;
    if(key==GLUT_KEY_F9)    doll=-doll;

}

void gaze(int x,int y)
{
    if((oldX<0) || (oldY<0))
	{

      oldX=x;
                  oldY=y;
	}
    lookX+=y-oldY;lookY+=x-oldX;oldX=x;oldY=y;
}

void myReshape(int w, int h)
{
    GLfloat Ratio;
    glViewport(0,0,w,h);
    Ratio=1.0*w/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0,Ratio,VIEW_NEAR,VIEW_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(INIT_VIEW_X,INIT_VIEW_Y,INIT_VIEW_Z);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
}

int main(int argc, char ** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(750,550);
    glutInitWindowPosition(200,100);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("Doll");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(Display);
    kreirajMeni();
    glutIdleFunc(Display);
    glutSpecialFunc(walk);
    glutPassiveMotionFunc(gaze);
    SetupRend();
    glEnable(GL_NORMALIZE);
    texture=LoadTexture("C:/Users/Amina/Desktop/Dolll/texture.raw",256,256);
    glutMainLoop();
}
