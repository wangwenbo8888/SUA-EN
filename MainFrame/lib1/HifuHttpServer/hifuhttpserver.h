#ifndef HIFUHTTPSERVER_H
#define HIFUHTTPSERVER_H

#include "hifuhttpserver_global.h"
#include <QObject>

namespace JQHttpServer
{
    class TcpServerManage;
}
using namespace JQHttpServer;

class HifuHttpServer;
static HifuHttpServer *_pHttpServer;

class HIFUHTTPSERVERSHARED_EXPORT HifuHttpServer : public QObject
{
    Q_OBJECT
public:
    HifuHttpServer(QObject* parent = 0);
    ~HifuHttpServer();

    bool StartServer();

signals:
    void setLocationPos(int axis,float value);

private:
    TcpServerManage* m_pTcpServerManage;

    void emitSetPos(int axis,int value);
};

#endif // HIFUHTTPSERVER_H
