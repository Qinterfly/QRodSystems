/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief The startup function
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    MainWindow window;
    window.showMaximized();
    return app.exec();
}
