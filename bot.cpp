#include "bot.h"


QString Bot::commit(const QString &arg)
{
	QStringList arguments;
	
	QString commitMessage = arg;
	commitMessage.replace(QChar('\n'),"\n PRIVMSG "+channel+" :");
	commitMessage = "PRIVMSG "+channel+" :" + commitMessage + "\n";
	sendMessage(commitMessage);
	commitMessage.clear();
	return QString("commit(\"%1\") got called").arg(arg);
}

void Bot::onConnect()
{
	connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
	dataToSend = "NICK " + nick + "\r\n" + "USER " + iDent + ' ' + serverAddress +" bla :"+ realName  +"\r\n";
	sendMessage(dataToSend);
}

void Bot::onError( QAbstractSocket::SocketError socketError )
{

	qDebug() <<"There was a socket error";

}

QString Bot::serviceName()
{
	return DBusInterface;
}

Bot::Bot( const QString &configurationFileName )
{
	QSettings settings( configurationFileName, QSettings::IniFormat );
	serverAddress = settings.value("serverAddress").toString();
	nick = settings.value("nick").toString();
	realName = settings.value("realName").toString();
	port = settings.value("port").toInt();
	iDent = settings.value("iDent").toString();
	owner = settings.value("owner").toString();
	channel = settings.value("channel").toString();
	DBusInterface = settings.value("DBusInterface").toString();
	tcpSocket.abort();
	tcpSocket.connectToHost( serverAddress, port);
	connect(&tcpSocket ,SIGNAL(connected()), this, SLOT(onConnect()));
	connect(&tcpSocket ,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

	scriptFunctions = new ScriptFunctions(this);
}


void Bot::sendMessage(const QString &messageBlock)
{
	qDebug() << messageBlock; 
	qint64 i = tcpSocket.write(messageBlock.toAscii());            	
	tcpSocket.flush();
}

void Bot::sendHelp()
{
	QDir dir("scripts/");
	QString extension;
	QString commandsAvaliable;
	dir.setFilter(QDir::Files);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		extension = fileInfo.suffix();
		if ( extension == "js" )
		{ commandsAvaliable.append("!"+fileInfo.fileName().split(".")[0]+"  ");}
	}
	sendMessage("PRIVMSG "+channel+" :The following commands are available: "+commandsAvaliable+"\n");
}

static QScriptValue Foo(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue a = context->argument(0);
	QScriptValue b = context->argument(1);
	return "haha";
}

void Bot::executeScript(const QString &scriptName, const QString &arguments)
{
	QScriptEngine *engine = 0;
	if (engines.contains(scriptName)) {
		engine = engines[scriptName];
	} else {
		engine = new QScriptEngine(this);
		QScriptValue fooProto = engine->newObject();
		engine->globalObject().setProperty("Foo", engine->newFunction(Foo));
		engine->globalObject().setProperty("bot", engine->newQObject(scriptFunctions));

		QFile script("scripts/" + scriptName);
		script.open(QFile::ReadOnly);
		QString code = script.readAll();
		QScriptValue value = engine->evaluate(code);
		engines[scriptName] = engine;
	}
	
	QScriptValue execute = engine->globalObject().property("execute");
	QScriptValueList args;
 	args << arguments;
 	execute.call(QScriptValue(), args);

}

void Bot::onReadReady()
{
	QByteArray reply;
	QString serverThatPinged;
	QString scriptName;
	while(tcpSocket.canReadLine())
	{
		reply = tcpSocket.readLine();
		qDebug() << reply;
		if ( reply.indexOf("identd") >= 0)
		{
			sendMessage("JOIN "+channel+"\n");
		}
		if ( reply.indexOf("PRIVMSG") >= 0 && reply.indexOf(nick) >= 0)
		{
			if ( reply.indexOf("ping") >= 0)
			{
				sendMessage("PRIVMSG "+channel+" :PONG!\n");		
			}else 
			if ( reply.indexOf("PRIVMSG "+channel+" :"+nick+", !") >= 0 || reply.indexOf("PRIVMSG "+channel+" :"+nick+": !") >= 0 )
			{
				if ( reply.indexOf("!help") > 0 )  
				{
					sendHelp();	
				}
				else
				{
                	QString executeString = reply.split('!')[2].split('\0')[0];
					executeString = executeString.trimmed();
					QString scriptName = executeString.split(' ')[0];
					int numberOfArguments = executeString.split(' ').size();
					QString arguments;
					if ( numberOfArguments > 1 )
					{
						for (int i = 1; i < executeString.split(' ').size(); i++)
						{
							arguments.append(executeString.split(' ')[i]);
							arguments.append(' ');
						}
						arguments = " " + arguments;
					
					}
					int length = scriptName.size();
					scriptName.append(".js");
					qDebug() <<scriptName;
                	executeScript(scriptName, arguments);
				}
			}else		
			
			{
				sendMessage("PRIVMSG "+channel+" :I print commit messages. I also have other functions. Type !help with my name.\n");
			}
		}
		if ( reply.indexOf("PING") == 0 )
		{
			serverThatPinged = reply.split(':')[1];
			sendMessage("PONG :"+serverThatPinged);
		}
	}
}

ScriptFunctions::ScriptFunctions(Bot *bot)
	: QObject(bot), bot(bot)
{
}

void ScriptFunctions::sendPrivateMessage(const QString &message)
{
	bot->sendMessage("PRIVMSG "+bot->channel+" :"+ message + "\n");
}

void ScriptFunctions::execute(const QString &program)
{
	QScriptValue callbackFunction = context()->argument(1);
	qDebug() <<"%%%"<<program;

	QProcess *process = new QProcess(this);
	process->setProperty("engine", qVariantFromValue(engine()));
	process->setProperty("callbackFunction", qVariantFromValue(callbackFunction));
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(handleFinished(int, QProcess::ExitStatus)));
	process->start( "bash", QStringList() << "-c" << program );
	QTimer::singleShot(10000, process, SLOT(kill()));
}

void ScriptFunctions::handleFinished(int exitCode, QProcess::ExitStatus status)
{
	QProcess *process = qobject_cast<QProcess *>(sender());
	Q_ASSERT(process);
	QString output = process->readAll().trimmed();
	QScriptValue callback = qvariant_cast<QScriptValue>(process->property("callbackFunction"));
	QScriptEngine *engine = qvariant_cast<QScriptEngine *>(process->property("engine"));
	QScriptValueList list;
	list << output;
	callback.call(QScriptValue(), list);
	delete process;
}

