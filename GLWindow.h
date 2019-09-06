#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>

class QKeyEvent;

#define N 512
#define Omega 1.998

class GLWindow : public QGLWidget
{
    Q_OBJECT

    public:
        GLWindow(QWidget *parent = 0);
        ~GLWindow();
    private:

        GLfloat rotX;
        GLfloat rotY;
        GLfloat rotZ;

        GLfloat speedX;
        GLfloat speedY;
        GLfloat speedZ;

        GLfloat tranX;
        GLfloat tranY;
        GLfloat tranZ;




        bool light;//свет вкл/выкл
        bool blender;
        GLuint textures;//переключит текстуру

        GLfloat shiniess;

        GLfloat LightAmbient[4];//значение фоновного света
        GLfloat LightDiffuse[4];//значение диффузного света
        GLfloat matAMB[4];
        GLfloat matSpec[4];


        QString stringNameTexture[3];
        //фоновый свет не имеет никакого определенного направления. Все объекты в сцене будут
        //освещеныфоновым светом.
        //диффузный цвет создается при помощи источника света и отражается от поверхности объекта
        //в сцене.Любая поверхность объекта на которую падает прямо свет, будет очень яркой и области,
        //откуда свет падает под углом будет темнее.
        GLfloat LightPosition[4];//xyz последний параметр позиция света



        GLvoid drop(void);
        GLvoid glRotX(void);
        GLvoid glRotY(void);
        GLvoid glRotZ(void);
        GLvoid lightOnOff(void);
        GLvoid nextTimeStep(void);
        GLvoid blenderOnOff(void);
        GLvoid changeSigleRot(void);


    protected:
        virtual void initializeGL(void);
        virtual void resizeGL(int w,int h);
        virtual void paintGL(void);
        virtual void keyPressEvent(QKeyEvent *press);
};

#endif // GLWINDOW_H
