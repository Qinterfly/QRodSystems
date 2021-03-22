/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of LogWidget class
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
    setEditTriggers(QTableWidget::NoEditTriggers);
}

//! Represent a message sent
void LogWidget::log(QtMsgType messageType, const QString& message)
{
    static const int kWaitToScroll = 10;
    int iRow = rowCount();
    insertRow(iRow);
    QString type = "Unknown";
    QIcon icon = QIcon::fromTheme("dialog-question");
    switch (messageType)
    {
    case QtDebugMsg:
        type = "Debug";
        icon = QIcon();
        break;
    case QtInfoMsg:
        type = "Info";
        icon = QIcon::fromTheme("dialog-information");
        break;
    case QtWarningMsg:
        type = "Warning";
        icon = QIcon::fromTheme("dialog-warning");
        break;
    case QtCriticalMsg:
        type = "Critical";
        icon = QIcon::fromTheme("dialog-error");
        break;
    case QtFatalMsg:
        type = "Fatal";
        icon = QIcon::fromTheme("dialog-error");
        break;
    }
    QString time = QTime::currentTime().toString();
    setItem(iRow, ColumnType::kTime, new QTableWidgetItem(time));
    setItem(iRow, ColumnType::kType, new QTableWidgetItem(icon, type));
    setItem(iRow, ColumnType::kMessage, new QTableWidgetItem(message));
    resizeColumnsToContents();
    QTimer::singleShot(kWaitToScroll, this, &QTableWidget::scrollToBottom);
}
