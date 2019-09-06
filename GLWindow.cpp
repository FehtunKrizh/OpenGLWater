#include <math.h>

#include <QTimer>
#include <QDebug>
#include <GL/glu.h>
#include <QKeyEvent>

#include "GLWindow.h"

GLfloat A[N][N],B[N][N],C[N][N],T[N][N];
//----------------public function------------------------
GLWindow::GLWindow(QWidget *parent):QGLWidget(parent)
{
    rotX=rotY=rotZ=0.0f;
    speedX=speedY=speedZ=1.0f;
    LightAmbient[0]=0.5f;//значение фоновного света
    LightAmbient[1]=0.5f;
    LightAmbient[2]=0.6f;
    LightAmbient[3]=0.5f;

    LightDiffuse[0]=1.0f;
    LightDiffuse[1]=1.0f;
    LightDiffuse[2]=0.3f;
    LightDiffuse[3]=0.5f;

    LightPosition[0]=0.0f;
    LightPosition[1]=1.0f;
    LightPosition[2]=0.0f;
    LightPosition[3]=1.0f;

    tranX=tranY=0.0f;
    tranZ=-5.0f;

    matSpec[0]=0.3;
    matSpec[1]=0.2;
    matSpec[2]=0.6;
    matSpec[3]=0.5;

    matAMB[0]=0.3;
    matAMB[1]=0.6;
    matAMB[2]=0.8;
    matAMB[3]=0.5;

    shiniess=0.6*128;
    blender=true;
    light=false;

//    A=new GLfloat*[N];
//    B=new GLfloat*[N];
//    C=new GLfloat*[N];
//    T=new GLfloat*[N];

//    for(unsigned int i=0;i<N;++i)
//    {
//        A[i]=new GLfloat[N];
//        B[i]=new GLfloat[N];
//        C[i]=new GLfloat[N];
//        T[i]=new GLfloat[N];
//    }
    for(int i=-N/2;i<N/2;++i)
    {
        for(int j=-N/2;j<N/2;++j)
        {
            A[i+N/2][j+N/2]=0;
            B[i+N/2][j+N/2]=0;
            C[i+N/2][j+N/2]=0;
        }
    }
}

GLWindow::~GLWindow()
{

}
//-------------------protected function------------------
void GLWindow::initializeGL(void)
{
    glMaterialfv(GL_FRONT,GL_AMBIENT,matAMB);
    glMaterialfv(GL_FRONT,GL_SPECULAR,matSpec);
    glMaterialf(GL_FRONT,GL_SHININESS,shiniess);

    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f,0.0f,0.0f,0.5f);
    glClearDepth(1.0);//зустановка буфера глубины
    glDepthFunc(GL_LESS);//(GL_LEQUAL) (GL_LESS);//тип теста глубины
    glEnable(GL_DEPTH_TEST);//разрешаем тест глубины
    glShadeModel(GL_SMOOTH);//сглаженное закрашивание
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);// улучшение вычисления перспективы

    glColor4f(0.0f,0.0f,0.0f,0.5f);//полня яркость, 50% альфа
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    //задаем освещение
    glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);//установка фонового света
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);//установка диффузионного света
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);// Позиция света
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);



}
void GLWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble ratio=(GLdouble)w/(GLdouble)h;
    if (w>=h)
    {
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 10.0);
    }
    else
    {
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 10.0);
    }
    glViewport(0,0,(GLint)w,(GLint)h);
}
void GLWindow::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(tranX,tranY,tranZ);
    glRotatef(rotX,1.0f,0.0f,0.0f);
    glRotatef(rotY,0.0f,1.0f,0.0f);
    glRotatef(rotZ,0.0f,0.0f,1.0f);
    //glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_POINTS);
    {
        for(unsigned int i=1;i<N-1;++i)
        {
            for(unsigned int j=1;j<N-1;++j)
            {
                glColor4f(0,0.1+0.5*C[i][j],0.1+0.3*C[i][j],0.5);
                glNormal3f(1.0*i/N-0.5,C[i][j],1.0*j/N-0.5);
                glVertex3f(1.0*i/N-0.5,C[i][j],1.0*j/N-0.5);
            }
        }
    }glEnd();

    for(unsigned int i=1;i<N-1;++i)
    {
        for(unsigned int j=1;j<N-1;++j)
        {
            T[i][j]=A[i][j];
            A[i][j]=B[i][j];
            B[i][j]=C[i][j];
            C[i][j]=T[i][j];
        }
    }


}

void GLWindow::keyPressEvent(QKeyEvent *press)
{
    switch(press->key())
    {
        case Qt::Key_X:glRotX();break;
        case Qt::Key_Y:glRotY();break;
        case Qt::Key_Z:glRotZ();break;
        case Qt::Key_Space:drop();break;
        case Qt::Key_L:lightOnOff();break;
        case Qt::Key_B:blenderOnOff();break;
        case Qt::Key_T:nextTimeStep();break;
        case Qt::Key_S:changeSigleRot();break;
        case Qt::Key_Escape:this->close();break;
    }
    updateGL();
}

//-----------------------private function---------------------------------------

GLvoid GLWindow::drop(void)
{
    for(int i=-N/2;i<N/2;++i)
    {
        for(int j=-N/2;j<N/2;++j)
        {
            A[i+N/2][j+N/2]=B[i+N/2][j+N/2]=C[i+N/2][j+N/2]=exp(-(i*i*1.0+j*j*1.0)/1024);
        }
    }
    //C[N/2][N/2]=1.0f;
    updateGL();
}

GLvoid GLWindow::glRotX(void)
{
    rotX+=speedX;
    if(rotX>=360.0)
    {
        rotX=0.0f;
    }
}

GLvoid GLWindow::glRotY(void)
{
    rotY+=speedY;
    if(rotY>=360.0)
    {
        rotY=0.0f;
    }
}

GLvoid GLWindow::glRotZ(void)
{
    rotZ+=speedZ;
    if(rotZ>=360.0)
    {
        rotZ=0.0f;
    }
}

GLvoid GLWindow::lightOnOff(void)
{
    if(light)
    {
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
    light=!light;
}
GLvoid GLWindow::blenderOnOff(void)
{
    if(blender)
    {
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
    blender=!blender;
}
GLvoid GLWindow::nextTimeStep(void)
{

        for(unsigned int i=1;i<N-1;++i)
        {
            for(unsigned int j=1;j<N-1;++j)
            {
                C[i][j]=(1-Omega)*B[i][j]+Omega*0.25*(A[i][j+1]+A[i][j-1]+A[i-1][j]+A[i+1][j]);
            }
        }
        updateGL();

}


GLvoid GLWindow::changeSigleRot(void)
{
    speedX=-speedX;
    speedY=-speedY;
    speedZ=-speedZ;
}

//---------------------------private slots--------------------------------
