#include "hifuhttpserver.h"
#include "JQHttpServer.h"
#include <QDebug>
#include <QThread>
#include <QtCore>

using namespace JQHttpServer;

HifuHttpServer::HifuHttpServer(QObject *parent) : QObject(parent)
{
    _pHttpServer=this;
    //qDebug()<<"HifuHttpServer Thread ID"<<QThread::currentThreadId();
    m_pTcpServerManage=new TcpServerManage(2);
    m_pTcpServerManage->setHttpAcceptedCallback( []( const QPointer< JQHttpServer::Session > &session )
    {
        //qDebug()<<"HifuHttpServer HttpAcceptedCallback Thread ID"<<QThread::currentThreadId()<<" "<<session->requestUrl();
        QString commandStr=session->requestUrlPath();
        if(commandStr=="/checkConnection"){
            session->replyText("{\"status\",\"connection is ok !\"}");
        }
        else if(commandStr=="/robotControl"){
            QMap< QString, QString > map=session->requestUrlQuery();
            //qDebug()<<"axis:"<<map["axis"]<<"position:"<<map["position"]<<"test:"<<map["test"];
            int iAxis,iPosition;
            bool ok1,ok2;
            iAxis=map["axis"].toInt(&ok1);
            iPosition=map["position"].toInt(&ok2);
            if(ok1&&ok2&&(iAxis>=0)&&(iAxis<=3)){
                //set Pos
                _pHttpServer->emitSetPos(iAxis,iPosition);
                session->replyText("{\"status\",\"set position ok !\"}");
            }
            else{
                session->replyText("{\"status\",\"set position error !\"}");
            }
        }
        else
        {
            session->replyText( QString( "Invalid command" ));
        }
    } );

    //qDebug() << "listen:" << m_pTcpServerManage->listen( QHostAddress::Any, 8838 );
}

HifuHttpServer::~HifuHttpServer()
{
    _pHttpServer=NULL;
    if(m_pTcpServerManage!=NULL)
    {
        delete m_pTcpServerManage;
        m_pTcpServerManage=NULL;
    }
}

bool HifuHttpServer::StartServer()
{
    return  m_pTcpServerManage->listen( QHostAddress::Any, 8838 );
}

void HifuHttpServer::emitSetPos(int axis,int value)
{
    qDebug()<<"emitSetPos Thread ID"<<QThread::currentThreadId();
    emit setLocationPos(axis,(float)value);
}
