#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer.setSingleShot(true);


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
    TCHARTool::CharToTchar(ui->myQQNumberEdit->text().toLocal8Bit(), QQtool.qqh);
    strcpy(QQtool.qqPath, ui->qqPathEdit->text().toLocal8Bit());
    TCHARTool::CharToTchar(ui->targetQQNumberEdit->text().toLocal8Bit(), QQtool.name);
    TCHARTool::CharToTchar(ui->targetRemarkEdit->text().toLocal8Bit(), QQtool.beizhu);
    TCHARTool::CharToTchar(ui->sendContentEdit->toPlainText().toLocal8Bit(), QQtool.text);

    timer.singleShot(ui->delaySpin->value() * 1000, [=]{
        QQtool.startSend();
    });
}
