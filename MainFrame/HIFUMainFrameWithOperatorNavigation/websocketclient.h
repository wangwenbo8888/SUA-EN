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
    static WebSocketClient* instance();

    void initializeConnection(const QUrl &url);
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
    static QUrl url;
    QProcess process;

    WebSocketClient();

    QWebSocket webSocket;
};

#endif // WEBSOCKETCLIENT_H
