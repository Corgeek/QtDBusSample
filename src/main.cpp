#include <QApplication>
#include "dbus_sender.h"
#include "dbus_receiver.h"

int main(int argc, char *argv[])
{
	QString pathBase = "com.servicename.";
	QString objBase = "/com/servicename/";
	QString	senderName = "sender";
	QString receiverName = "recevier";

	QApplication	app(argc, argv);

	DBusSender		sender(pathBase, objBase, senderName, receiverName);
	DBusReceiver	receiver(pathBase, objBase, receiverName, senderName);

	sender.show();
	receiver.show();

	return	app.exec();
}
