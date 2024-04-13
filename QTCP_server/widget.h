#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_startBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::Widget *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QList<QTcpSocket*> socket_list;
    int onlineNum;
};
#endif // WIDGET_H
