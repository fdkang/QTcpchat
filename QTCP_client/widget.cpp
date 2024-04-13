#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("网络聊天客户端");
    this->setFixedSize(800,650);
    this->setWindowIcon(QIcon(":/new/prefix1/User.png"));
    this->socket = new QTcpSocket(this);

    connect(socket,&QTcpSocket::connected,[=](){
        ui->chatEdit->append("与服务器建立连接");
        ui->connectBtn->setText("已连接");
        ui->connectBtn->setEnabled(false);
    });

    connect(socket,&QTcpSocket::disconnected,[=](){
        socket->close();
        ui->chatEdit->append("连接失败或断开");
        ui->connectBtn->setText("连接");
        ui->connectBtn->setEnabled(true);
    });

    connect(socket,&QTcpSocket::readyRead,[=](){
        QString recvMsg = socket->readAll();
        ui->chatEdit->append(recvMsg);
    });
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_connectBtn_clicked()
{
    QString ip = ui->ipEdit->text();
    quint16 port = ui->portEdit->text().toUShort();
    socket->connectToHost(ip,port);
}

void Widget::on_closeBtn_clicked()
{
    socket->close();
}

void Widget::on_sendBtn_clicked()
{
    QString sendMsg = ui->nameEdit->text()+":"+ui->userEdit->toPlainText();
    socket->write(sendMsg.toUtf8());
    ui->userEdit->clear();
}
