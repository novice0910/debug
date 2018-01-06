#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    card = new CardCom;
    m_CardComOn = false;
}

void MainWindow::on_btnOnOff_clicked()
{
    if(m_CardComOn == false)    //open
    {
        QString portName = ui->cBoxPort->currentText();
        quint32 baud = ui->cBoxBaud->currentText().toInt();
        char dataBits = ui->cBoxDataBits->currentText().toLatin1().data()[0];
        char stopBits = ui->cBoxStop->currentText().toLatin1().data()[0];
        char parityBits = ui->cBoxParity->currentText().toLatin1().data()[0];
        bool ret = card->openSerial(portName,baud,dataBits,stopBits,parityBits);
        if(ret)
        {
            ui->statusBar->showMessage(tr("打开串口成功!"),1000);
            ui->cBoxPort->setEnabled(false);
            ui->cBoxBaud->setEnabled(false);
            ui->cBoxDataBits->setEnabled(false);
            ui->cBoxParity->setEnabled(false);
            ui->cBoxStop->setEnabled(false);
            ui->btnOnOff->setText(tr("关闭"));
            m_CardComOn = true;
        }
        else
        {
            ui->statusBar->showMessage(tr("打开串口失败！"),1000);
        }
    }
    else
    {
        card->closeSerial();
        ui->cBoxPort->setEnabled(true);
        ui->cBoxBaud->setEnabled(true);
        ui->cBoxDataBits->setEnabled(true);
        ui->cBoxParity->setEnabled(true);
        ui->cBoxStop->setEnabled(true);
        ui->btnOnOff->setText(tr("打开"));
        m_CardComOn = false;
        ui->statusBar->showMessage(tr("串口关闭"),1000);
    }
}
