#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QStandardPaths>
#include "qqsendtool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QSettings st;
    QQSendTool QQtool;
    QTimer timer;
    QTimer showTimer;
};
#endif // MAINWINDOW_H
