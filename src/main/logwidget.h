/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of LogWidget class
 */

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTextEdit>

class LogWidget : public QTextEdit
{
public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget() = default;
    void log(QtMsgType type, const QString &msg);
};

#endif // LOGWIDGET_H
