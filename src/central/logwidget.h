/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the LogWidget class
 */

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTableWidget>

namespace QRS::App
{

//! Log all the messages sent
class LogWidget : public QTableWidget
{
public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget() = default;
    void log(QtMsgType messageType, const QString& message);
};

}


#endif // LOGWIDGET_H
