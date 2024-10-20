#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_SocketUPD = new QUdpSocket(this);
    m_Port = 8888;
    connect(m_SocketUPD, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    m_SocketUPD->bind(m_Port);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    QString data = "";
    while(m_SocketUPD->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_SocketUPD->pendingDatagramSize());
        QHostAddress senderIP;
        quint16 senderPort;
        m_SocketUPD->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);
        data += QString(datagram);
        ui->temp_label->setText(data);
    }
}
void MainWindow::SendWrite()
{
    QString str_ip = ui->lineEdit_IP->text();
    QHostAddress IP(str_ip);
    m_SocketUPD->writeDatagram("Servers, where are you?" , IP , m_Port);

}
void MainWindow::on_pushButton_clicked()
{


    for(int value = 0; value <= 100; value++){
        QThread::msleep(40);
        ui->progressBar->setValue(value);
        qApp->processEvents(QEventLoop::AllEvents);
        SendWrite();
    }
}

