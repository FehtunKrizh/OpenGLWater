#include <QApplication>

#include "GLWindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWindow glwindow;
    glwindow.resize(800,800);
    glwindow.show();
    return a.exec();
}
