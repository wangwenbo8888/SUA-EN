#include "websocketclient.h"

QUrl WebSocketClient::url;
QProcess process;
static WebSocketClient* client = nullptr;

WebSocketClient::WebSocketClient()
{
    connect(&webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&webSocket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &WebSocketClient::onErrorOccurred); // 修正此行
}

WebSocketClient* WebSocketClient::instance()
{
    if (!client)
    {
        client = new WebSocketClient();
    }

    if (url.isEmpty()) {
        url = QUrl("ws://127.0.0.1:9706");
    }
    return client;
}

void WebSocketClient::initializeConnection(const QUrl &url)
{
    if (url.isEmpty()) { // 检查url是否为空
        this->url = url;
    }

    webSocket.open(this->url);
}

void WebSocketClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
//    QString programPath = "D:/image_fusion.exe";
//    qDebug() << "WebSocket error occurred: " << error << "  start:" << programPath << endl;

//    // 设置外部exe文件的路径
//    process.setProgram(programPath);

//    // 启动外部exe文件
//    process.start();

    // 使用 QTimer 来等待5秒后重新连接
     QTimer::singleShot(5000, this, &WebSocketClient::retryConnection);
 }

 void WebSocketClient::retryConnection()
 {
     qDebug() << "Retrying WebSocket connection..." << endl;
     webSocket.open(this->url);
 }

void WebSocketClient::sendMessage(const QString &message)
{
    qDebug()<<"before sendMessage";
    webSocket.sendTextMessage(message);
    qDebug()<<"after sendMessage";
}

void WebSocketClient::onConnected()
{
    emit connectedToServer();
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    emit messageReceived(message);
}

void WebSocketClient::onDisconnected()
{
    emit disconnectedFromServer();
}

