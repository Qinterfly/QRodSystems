/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of MainWindow class
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//! The main window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initialize();

private:
    Ui::MainWindow *mUi;
};
#endif // MAINWINDOW_H
