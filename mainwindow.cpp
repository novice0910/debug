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
    card->init();
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

void MainWindow::on_btnSend_clicked()
{
//    qDebug()<<"111";
//    QByteArray array = ui->lineEdit->text().toLatin1();
//    qDebug()<<"dddd"<<array;
//    card->writeSerial(array);
//    char buf[] = {0x02,0x00 ,0x04 ,0x32 ,0x24 ,0xff ,0xff,0x16,0x03};
//    char vrfSecretKey[] = {0x02,0x00,0x0e,0x02,0x46
//                           ,0x60 ,0xE4 ,0x85 ,0xAD ,0xB6 ,0x22,0x22
//                           ,0x22 ,0x22 ,0x22 ,0x22 ,0x07 ,0x59 ,0x03};
//    char buf1[sizeof(buf) + sizeof(vrfSecretKey)] ={};
//    memcpy(buf1,buf,sizeof(buf));
//    memcpy((buf1 + sizeof(buf)),vrfSecretKey,sizeof(vrfSecretKey));
//    card->writeSerial(buf,sizeof(buf));
//    usleep(100);
//    card->writeSerial(vrfSecretKey,sizeof(vrfSecretKey));
//    card->writeSerial(buf1,sizeof(buf1));
}
