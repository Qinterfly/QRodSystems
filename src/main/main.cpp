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

//! Load a style sheet
void loadStyleSheet(QString const& path)
{
    QFile fileStyleSheet(path);
    if (!fileStyleSheet.exists())
    {
        printf("File contained the style sheet was not found\n");
    }
    else
    {
        fileStyleSheet.open(QFile::ReadOnly | QFile::Text);
        QTextStream streamStyle(&fileStyleSheet);
        qApp->setStyleSheet(streamStyle.readAll());
        fileStyleSheet.close();
    }
}

//! Entry point
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    loadStyleSheet(":/styles/modern.qss");
    qInstallMessageHandler(throwMessage);
    MainWindow window;
    window.show();
    return app.exec();
}

