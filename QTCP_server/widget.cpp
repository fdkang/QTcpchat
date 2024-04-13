#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("网络聊天服务器");
    this->setWindowIcon(QIcon(":/new/prefix1/MSN-Messenger.png"));
    //定义服务器与套接字对象空间
    server = new QTcpServer(this);
    socket = new QTcpSocket(this);

    //使用信号与槽绑定关联关系
    connect(server,&QTcpServer::newConnection,[=](){
        //创建与客户机的套接字
        QTcpSocket* socket = server->nextPendingConnection();
        ui->chatEdit->append("有新的客户端连接了服务器");
        socket_list.push_back(socket);
        onlineNum = socket_list.count();
        ui->onlinelabel->setText("当前: " + QString::number(onlineNum));
        qDebug()<<onlineNum;
        //对连接套接字对象建立关联关系
        connect(socket,&QTcpSocket::readyRead,[=](){
            QString recvMsg = socket->readAll();
            ui->chatEdit->append("客户机数据："+recvMsg);
            for(QTcpSocket* s : socket_list){
                s->write(recvMsg.toUtf8());
            }
        });
        //如果连接套接字断开连接
        connect(socket,&QTcpSocket::disconnected,[=](){
            this->socket->deleteLater();
            socket_list.removeOne(socket);
            onlineNum = socket_list.count();
            ui->onlinelabel->setText("当前: " + QString::number(onlineNum));
            qDebug()<<onlineNum;
            ui->chatEdit->append("有客户端断开连接");
        });
    });
}

Widget::~Widget()
{
    delete ui;
}

//服务器启动按钮槽函数
void Widget::on_startBtn_clicked()
{
    bool ok = false;
    quint16 port = ui->portLine->text().toUShort(&ok);

    if(!ok || port == 0) {
        QMessageBox::critical(this, "错误", "请输入有效的端口号");
        return;
    }

    if(server->isListening()) {
        QMessageBox::information(this, "提示", "服务器已经在运行");
        return;
    }

    if(server->listen(QHostAddress::Any, port)) {
        ui->startBtn->setEnabled(false);
        ui->startBtn->setText("服务启动");
    } else {
        QMessageBox::critical(this, "错误", "无法启动服务器");
    }
}


void Widget::on_sendBtn_clicked()
{
    QString sendMsg = "admin:" + ui->adminEdit->toPlainText();
    for(QTcpSocket* s : socket_list){
        s->write(sendMsg.toUtf8());
    }
    ui->adminEdit->clear();
    ui->chatEdit->append(sendMsg);
}
