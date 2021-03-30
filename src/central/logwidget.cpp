/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the LogWidget class
 */

#include <QHeaderView>
#include <QTime>
#include <QTimer>
#include "logwidget.h"

enum ColumnType
{
    kTime,
    kType,
    kMessage
};

LogWidget::LogWidget(QWidget* parent)
    : QTableWidget(parent)
{
    setColumnCount(3);
    setSortingEnabled(false);
    setSizeAdjustPolicy(AdjustToContents);
    horizontalHeader()->setStretchLastSection(true);
    setHorizontalHeaderLabels({tr("Time"), tr("Type"), tr("Message")});
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//! Represent a message sent
void LogWidget::log(QtMsgType messageType, const QString& message)
{
    static const int kWaitToScroll = 10;
    int iRow = rowCount();
    insertRow(iRow);
    QString type = "Unknown";
    QIcon icon = QIcon(":/icons/dialog-question.svg");
    switch (messageType)
    {
    case QtDebugMsg:
        type = "Debug";
        icon = QIcon(":/icons/dialog-debug.svg");
        break;
    case QtInfoMsg:
        type = "Info";
        icon = QIcon(":/icons/dialog-info.svg");
        break;
    case QtWarningMsg:
        type = "Warning";
        icon = QIcon(":/icons/dialog-warning.svg");
        break;
    case QtCriticalMsg:
        type = "Critical";
        icon = QIcon(":/icons/dialog-error.svg");
        break;
    case QtFatalMsg:
        type = "Fatal";
        icon = QIcon(":/icons/dialog-fatal.svg");
        break;
    }
    QString time = QTime::currentTime().toString();
    QString filterMessage = message;
    filterMessage.remove('\"');
    setItem(iRow, ColumnType::kTime, new QTableWidgetItem(time));
    setItem(iRow, ColumnType::kType, new QTableWidgetItem(icon, type));
    setItem(iRow, ColumnType::kMessage, new QTableWidgetItem(filterMessage));
    resizeColumnsToContents();
    QTimer::singleShot(kWaitToScroll, this, &QTableWidget::scrollToBottom);
}
