#include "clientwidget.h"
#include "ui_clientwidget.h"
#include<QHostAddress>
#include <QDebug>
#include "qbuffer.h"
#include "qimagereader.h"

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    //初始化套接字
    tcpSocket = NULL;
    blockSize = 0;
    //分配空间，指定父对象
    tcpSocket = new QTcpSocket(this);
    //建立连接的信号槽
    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
            {
                 ui->textEditRead->setText("恭喜，成功连接服务器！");
            }
            );
    //连接建立后收到服务器数据的信号槽
    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()
           {
            //获取对方发送的内容
            // QByteArray array = tcpSocket->readAll();
            //显示到编辑框
            // ui->textEditRead->append(array);//append，添加内容
            showPicture();
            }
            );

    //设置窗口标题
    setWindowTitle("TCP客户端");
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_buttonConnect_clicked()
{
    //获取服务器IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();
    //主动连接服务器
    tcpSocket->connectToHost(QHostAddress(ip),port);
}

void ClientWidget::on_buttonSend_clicked()
{
    //获取编辑框内容
     QString str = ui->textEditWrite->toPlainText();
     //发送数据
     // qDebug()<<"client send message size is "<<str.toUtf8().size();
     tcpSocket->write( str.toUtf8().data() );
}

void ClientWidget::on_buttonClose_clicked()
{ //主动断开连接
    tcpSocket->disconnectFromHost();
}

void ClientWidget::showPicture()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        if(blockSize==0)
        {
            QDataStream in(tcpSocket);
            in.setVersion(QDataStream::Qt_5_4);
            if(tcpSocket->bytesAvailable()<sizeof(quint32))
                return;

            in>>blockSize;
        }

        qDebug()<<"output image size is "<<blockSize;
        if(tcpSocket->bytesAvailable()<blockSize)
            return;

        if (blockSize<500)
        {
            // qDebug()<<"block size is "<<blockSize;
            //获取对方发送的内容
            QByteArray array = tcpSocket->read(blockSize);
            //显示到编辑框
            ui->textEditRead->append(array);//append，添加内容

            blockSize = 0;
            return;
        }

        QByteArray array = tcpSocket->read(blockSize);//blockSize作read()的参数。
        QBuffer buffer(&array);
        buffer.open(QIODevice::ReadOnly);
        QImageReader reader(&buffer,"jpg");
        QImage image = reader.read();
        blockSize=0;
        if(!image.isNull())
        {
            // image=image.scaled(ui->label_image->size());
            ui->label_image->setPixmap(QPixmap::fromImage(image));
        }
        else
        {
            qDebug()<<"image null !";
            ui->textEditRead->append(array);//append，添加内容
        }
    }
}
