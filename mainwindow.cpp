#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      st(/*QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)*/ + "settings.ini")

{
    ui->setupUi(this);

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, [=]{
        QQtool.startSend();

        showTimer.stop();
        ui->sendButton->setText("立即/定时发送");
    }); // 用信号槽而不用 singleShot 的原因是，可以停下
    showTimer.setInterval(40);
    connect(&showTimer, &QTimer::timeout, this, [=]{
        ui->sendButton->setText(QString("%1 秒后发送").arg((timer.remainingTime() + 900) / 1000));
    });


    char s[256];
    TCHARTool::TcharToChar(QQtool.qqh, s);
    ui->myQQNumberEdit->setText(st.contains("myQQNumber") ? st.value("myQQNumber").toString() : QString::fromLocal8Bit(s));

    ui->qqPathEdit->setText(st.contains("qqPath") ? st.value("qqPath").toString() : QQtool.qqPath);

    TCHARTool::TcharToChar(QQtool.name, s);
    ui->targetQQNumberEdit->setText(st.contains("targetQQNumber") ? st.value("targetQQNumber").toString() : QString::fromLocal8Bit(s));

    TCHARTool::TcharToChar(QQtool.beizhu, s);
    ui->targetRemarkEdit->setText(st.contains("targetRemark") ? st.value("targetRemark").toString() : QString::fromLocal8Bit(s));

    TCHARTool::TcharToChar(QQtool.text, s);
    ui->sendContentEdit->setText(st.contains("sendContext") ? st.value("sendContext").toString() : QString::fromLocal8Bit(s));

    ui->pasteCheck->setChecked(st.contains("usePaste") ? st.value("usePaste").toBool() : QQtool.usePaste);
    ui->ctrlEnterCheck->setChecked(st.contains("useCtrlEnter") ? st.value("useCtrlEnter").toBool() : QQtool.useCtrlEnter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    if (timer.isActive())
    {
        timer.stop();
        showTimer.stop();
        ui->sendButton->setText("立即/定时发送");
        return ;
    }

    TCHARTool::CharToTchar(ui->myQQNumberEdit->text().toLocal8Bit(), QQtool.qqh);
    strcpy(QQtool.qqPath, ui->qqPathEdit->text().toLocal8Bit());
    TCHARTool::CharToTchar(ui->targetQQNumberEdit->text().toLocal8Bit(), QQtool.name);
    TCHARTool::CharToTchar(ui->targetRemarkEdit->text().toLocal8Bit(), QQtool.beizhu);
    TCHARTool::CharToTchar(ui->sendContentEdit->toPlainText().toLocal8Bit(), QQtool.text);
    QQtool.usePaste = ui->pasteCheck->isChecked();
    QQtool.useCtrlEnter = ui->ctrlEnterCheck->isChecked();

    st.setValue("myQQNumber", ui->myQQNumberEdit->text());
    st.setValue("qqPath", ui->qqPathEdit->text());
    st.setValue("targetQQNumber", ui->targetQQNumberEdit->text());
    st.setValue("targetRemark", ui->targetRemarkEdit->text());
    st.setValue("sendContent", ui->sendContentEdit->toPlainText());
    st.setValue("usePaste", ui->pasteCheck->isChecked());
    st.setValue("useCtrlEnter", ui->ctrlEnterCheck->isChecked());
    st.sync();

    timer.start(ui->delaySpin->value() * 1000);
    showTimer.start();
}
