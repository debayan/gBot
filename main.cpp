#include "bot.h"
#include <QCoreApplication>
#include <string.h>


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QString configurationFileName = "botSettings.ini";
	if ( argc == 3 )
	{

		if ( strcmp( argv[1], "-c" ) == 0 )
		{
			configurationFileName = argv[2];
		}
		else
		{
			qDebug() <<"Usage is:  ./bot -c configFile.ini ";
			exit(1);
		}
	}
	Bot bot( configurationFileName );
	
	if (!QDBusConnection::sessionBus().isConnected()) {                        
		fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"           
				"To start it, run:\n"                                          
				"\teval `dbus-launch --auto-syntax`\n");                       
    }

	if (!QDBusConnection::sessionBus().registerService(bot.serviceName())) {
		fprintf(stderr, "%s\n",
			qPrintable(QDBusConnection::sessionBus().lastError().message()));               
		exit(1);
    }
	

	QDBusConnection::sessionBus().registerObject("/", &bot, QDBusConnection::ExportAllSlots);
    
	qDebug() <<"Listening on DBUS...";
	return app.exec();
}
