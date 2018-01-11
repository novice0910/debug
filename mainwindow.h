#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
//#include <QtDebug>
#include "cardcom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();

private slots:
    void on_btnOnOff_clicked();
    void on_btnSend_clicked();

private:
    Ui::MainWindow *ui;
    CardCom *card;
    bool m_CardComOn;
};

#endif // MAINWINDOW_H
