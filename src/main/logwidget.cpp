/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of LogWidget class
 */

#include "logwidget.h"

LogWidget::LogWidget(QWidget* parent)
    : QTextEdit(parent)
{

}

//! Represent a message sent
void LogWidget::log(QtMsgType type, const QString &msg)
{
    switch (type)
    {
    case QtDebugMsg:
        append("Debug: %s)\n" + msg);
        break;
    case QtInfoMsg:
        append("Info: %s)\n" + msg);
        break;
    case QtWarningMsg:
        append("Warning: %s)\n" + msg);
        break;
    case QtCriticalMsg:
        append("Critical: %s)\n" + msg);
        break;
    case QtFatalMsg:
        append("Fatal: %s)\n" + msg);
        abort();
    }
}
