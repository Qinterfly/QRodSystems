/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief The startup function
 */

#include <QApplication>
#include "mainwindow.h"

//! Log all the messages
void throwMessage(QtMsgType type, const QMessageLogContext& /*context*/, const QString& message)
{
    MainWindow::pLogger->log(type, message);
}

//! Entry point
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    qInstallMessageHandler(throwMessage);
    MainWindow window;
    window.showMaximized();
    return app.exec();
}
