/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief The startup function
 */

#include <QFile>
#include <QApplication>
#include <QFontDatabase>
#include "mainwindow.h"
#include "utilities.h"

//! Entry point
int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    qApp->setFont(QFont("Source Sans Pro"));
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(QRS::Utilities::File::loadFileContent(":/styles/modern.qss"));
    qInstallMessageHandler(QRS::App::throwMessage);
    QRS::App::MainWindow window;
    window.show();
    return app.exec();
}

