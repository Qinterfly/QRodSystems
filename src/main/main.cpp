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
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    uint fontSize = 12;
#ifdef Q_OS_WIN
    fontSize = 10;
#endif
    qApp->setFont(QFont("Source Sans Pro", fontSize));
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(QRS::Utilities::File::loadFileContent(":/styles/modern.qss"));
    qInstallMessageHandler(QRS::App::throwMessage);
    QRS::App::MainWindow window;
    window.show();
    return app.exec();
}

