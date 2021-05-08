/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief The startup function
 */

#include <QFile>
#include <QApplication>
#include "mainwindow.h"
#include "utilities.h"

//! Entry point
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(Utilities::File::loadFileContent(":/styles/modern.qss"));
    qInstallMessageHandler(throwMessage);
    MainWindow window;
    window.show();
    return app.exec();
}

