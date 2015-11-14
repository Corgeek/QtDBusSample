#include "dbus_receiver.h"
#include <QDebug>

DBusReceiver::DBusReceiver(const QString &pathName,
						   const QString &objName,
						   const QString &receiverName,
						   const QString &senderName,
						   QWidget *parent) :
	QLineEdit(parent),
	m_pathNameBase(pathName),
	m_objNameBase(objName),
	m_receiverName(receiverName),
	m_senderName(senderName)
{
	setupDBus();
	connectSyncMessage();

	setWindowTitle(receiverName);
}

void DBusReceiver::setupDBus()
{
	QDBusConnection	sessionBus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, m_receiverName);

	if (!sessionBus.isConnected()) {
		qDebug() << m_receiverName << "connectToBus() failed";
		return;
	}

	if (!sessionBus.registerService(m_pathNameBase + m_receiverName))
		qDebug() << m_pathNameBase + m_receiverName << "registerService() failed";

	if (!sessionBus.registerObject(m_objNameBase + m_receiverName, this))
		qDebug() << m_objNameBase + m_receiverName << "registerObject() failed";

	new IfnameAdaptor(this);
}


void DBusReceiver::connectSyncMessage()
{
	QDBusConnection sessionBus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, m_senderName);

	if (!sessionBus.isConnected()) {
		qDebug() << m_senderName << "connectToBus() failed";
		return;
	}

	if (!sessionBus.connect(
				m_pathNameBase + m_senderName,					//sender's service name
				m_objNameBase  + m_senderName,					//sender's path name
				com::servicename::ifname::staticInterfaceName(),
																//interface
				"syncSend",										//sender's signal name
				this,											//receiver
				SLOT(recvSyncMessage(const QByteArray &)))) {	//slot
		qDebug() << m_receiverName << "connectToBus()::connect() failed";
	}
}

void DBusReceiver::recvSyncMessage(const QByteArray &data)
{
	setText("sync:" + data);
}

QString DBusReceiver::asyncInvoke(const QByteArray &data)
{
	setText("async:" + data);

	return	QString("reply:") + data;
}
