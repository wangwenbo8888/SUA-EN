
#include "constant.h"
#include "network.h"


Network::Network(QObject *parent) : QObject(parent)
{
}

void Network::loadSettings() {
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    if (localIP().isNull()) m_rxIp = "";
    m_rxIp = localIP().toString();
    m_rxPort = settings->value("Network/rx_port").toString().toUShort(0,10);
    m_txIp = settings->value("Network/tx_ip").toString();
    m_txPort = settings->value("Network/tx_port").toString().toUShort(0,10);
    delete settings;
}
