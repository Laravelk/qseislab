#include "main/controller.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Main::Controller controller;

    return a.exec();
}
