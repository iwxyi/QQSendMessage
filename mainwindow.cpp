#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer.setSingleShot(true);
//    connect(&timer, &QTimer::timeout, this, [=]{
//        QQtool.startSend();
//    });

    char s[256];
    TCHARTool::TcharToChar(QQtool.qqh, s);
    ui->myQQNumberEdit->setText(QString::fromLocal8Bit(s));

    ui->qqPathEdit->setText(QQtool.qqPath);

    TCHARTool::TcharToChar(QQtool.name, s);
    ui->targetQQNumberEdit->setText(QString::fromLocal8Bit(s));

    TCHARTool::TcharToChar(QQtool.beizhu, s);
    ui->targetRemarkEdit->setText(QString::fromLocal8Bit(s));

    TCHARTool::TcharToChar(QQtool.text, s);
    ui->sendContentEdit->setText(QString::fromLocal8Bit(s));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    timer.singleShot(ui->delaySpin->value() * 1000, [=]{
        QQtool.startSend();
    });
}
