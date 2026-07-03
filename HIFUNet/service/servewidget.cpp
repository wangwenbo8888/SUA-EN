#include "servewidget.h"
#include "ui_servewidget.h"

#include "imagegrabber.h"
#include <QtNetwork>

ServeWidget::ServeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServeWidget)
{
    ui->setupUi(this);
    //初始化套接字
    tcpServer = NULL;
    tcpSocket = NULL;
    //监听套接字，指定父对象自动回收空间
    tcpServer = new QTcpServer(this);
    //设定端口
    tcpServer->listen(QHostAddress::Any,8888);
    //新连接建立的信号槽
    connect(tcpServer,&QTcpServer::newConnection,[=]()
    {
    //取出建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();
    //获取对方的IP和端口
    QString ip = tcpSocket->peerAddress().toString();
    qint16 port = tcpSocket->peerPort();
    //将信息显示到UI
    QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);
    ui->textEditRead->setText(temp);
    //连接建立后，读取到数据的信号槽
    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()
            {
             //从通信套接字取出内容
             QByteArray array = tcpSocket->readAll();
             //显示到UI
             ui->textEditRead->append(array);
            } );
    });

    initGrabber();

    setWindowTitle("TCP服务器，端口:8888");
    QString ip("本机IP: ");
    ip.append(getIP());
    ui->label_IP->setText(ip);
}

ServeWidget::~ServeWidget()
{
    delete ui;
}

QString ServeWidget::getIP()
{
//    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
//    foreach(QHostAddress address,info.addresses())
//    {
//        if(address.protocol() == QAbstractSocket::IPv4Protocol)
//            return address.toString();
//    }

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }

    return "";
}

void ServeWidget::initGrabber()
{
    m_pImageGrabber = new ImageGrabber(this);
    if (m_pImageGrabber->open())
    {
        connect(m_pImageGrabber, SIGNAL(imageRefreshed(QImage)), this, SLOT(refresh(QImage)));
        m_pImageGrabber->work();
    }
}

void ServeWidget::refresh(QImage image)
{
    QBuffer buffer;
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    image.save(&buffer,"jpg");
    sendPicture(buffer);
}

void ServeWidget::on_ButtonSend_clicked()
{   if(NULL == tcpSocket)
    {
        return;
    }
    //获取编辑区内容
    QString str = ui->textEditWrite->toPlainText();

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out<<(quint32)str.toUtf8().size();
    block.append(str.toUtf8().data());
    tcpSocket->write(block);
}

void ServeWidget::on_ButtonClose_clicked()
{   if(NULL == tcpSocket)
    {return;}
    //主动和客户端断开连接
    tcpSocket->disconnectFromHost();
    //关闭套接字
    tcpSocket->close();
    tcpSocket = NULL;

}

void ServeWidget::sendPicture(QBuffer& buffer)
{
    if(tcpSocket==NULL)
        return;

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out<<(quint32)buffer.data().size();
    qDebug()<<"input image size is "<<buffer.data().size();
    block.append(buffer.data());
    tcpSocket->write(block);
}
