#ifndef BOT_H
#define BOT_h

#include <QtNetwork>
#include <QtCore>
#include <QtDBus/QtDBus>
#include <QtScript>
#include <QSettings>

class Bot : public QObject
{
	Q_OBJECT

public:
	Bot(const QString &);
	void sendMessage(const QString &);
	void executeScript(const QString &);
	QString serviceName();

private:
	QString serverAddress;
	QString nick;
	int port;
	QString iDent;
	QString realName;
	QString owner;
	QString channel;
	QString readBuffer;
	QString dataToSend;
	QString DBusInterface;
	QByteArray block;
	QTcpSocket tcpSocket;
	quint16 blockSize;

private slots:
	void onReadReady();
	void onConnect();
	void onError(QAbstractSocket::SocketError);
	void sendHelp();
public slots:
	Q_SCRIPTABLE QString commit(const QString &arg);
};

#endif 
