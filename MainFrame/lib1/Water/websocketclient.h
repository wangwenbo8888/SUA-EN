#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QProcess>
#include <QTimer>

class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    static WebSocketClient* instance(const QString& id); // 根据标识符获取实例

    void initializeConnection(const QUrl &url); // 初始化连接

    void sendMessage(const QString &message);

signals:
    void connectedToServer();
    void messageReceived(const QString &message);
    void disconnectedFromServer();

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void retryConnection();

private:

    explicit WebSocketClient(const QString& id); // 私有构造函数

    QProcess process;

    WebSocketClient();

    QWebSocket webSocket;
    QString clientId; // 存储每个实例的标识符

    static QMap<QString, WebSocketClient*> clients; // 存储所有实例
    static QUrl url; // 全局 URL
};

#endif // WEBSOCKETCLIENT_H
