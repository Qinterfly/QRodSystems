/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief The startup function
 */

#include <QApplication>
#include <QFile>
#include "mainwindow.h"

//! Log all the messages
void throwMessage(QtMsgType type, const QMessageLogContext& /*context*/, const QString& message)
{
    MainWindow::pLogger->log(type, message);
}

//! Entry point
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    QFile fileTheme(":/styles/light.qss");
    if (!fileTheme.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        fileTheme.open(QFile::ReadOnly | QFile::Text);
        QTextStream streamTheme(&fileTheme);
        qApp->setStyleSheet(streamTheme.readAll());
        fileTheme.close();
    }
    app.setStyle("Fusion");
    qInstallMessageHandler(throwMessage);
    MainWindow window;
    window.show();
    return app.exec();
}

