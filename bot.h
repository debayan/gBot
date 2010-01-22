#ifndef BOT_H
#define BOT_h

#include <QtNetwork>
#include <QtCore>
#include <QtDBus/QtDBus>
#include <QtScript>
#include <QSettings>

class ScriptFunctions;

class Bot : public QObject, protected QScriptable
{
	Q_OBJECT

public:
	Bot(const QString &);
	void executeScript(const QString &);
	QString serviceName();

public slots:
	void sendMessage(const QString &);

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
	QString arguments;
	QByteArray block;
	QTcpSocket tcpSocket;
	quint16 blockSize;
	QHash<QString, QScriptEngine *> engines;
	ScriptFunctions *scriptFunctions;
	friend class ScriptFunctions;

private slots:
	void onReadReady();
	void onConnect();
	void onError(QAbstractSocket::SocketError);
	void sendHelp();
public slots:
	Q_SCRIPTABLE QString commit(const QString &arg);
};

class ScriptFunctions : public QObject, protected QScriptable
{
	Q_OBJECT
public:
	ScriptFunctions(Bot *bot);

public slots:
	void sendPrivateMessage(const QString &message);
	void execute(const QString &program);

private slots:
	void handleFinished(int, QProcess::ExitStatus);

private:
	Bot *bot;
};

Q_DECLARE_METATYPE(QScriptValue);
Q_DECLARE_METATYPE(QScriptEngine *);
#endif 
