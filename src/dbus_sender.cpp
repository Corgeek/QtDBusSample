#include "dbus_sender.h"
#include <QHBoxLayout>
#include <QDebug>

DBusSender::DBusSender(const QString &pathBase,
					   const QString &objBase,
					   const QString &senderName,
					   const QString &receiverName,
					   QWidget *parent) :
	QDialog(parent),
	m_pathNameBase(pathBase),
	m_objNameBase(objBase),
	m_senderName(senderName),
	m_receiverName(receiverName)
{
	setupUi();

	setupDBus();

	createInterface();
	connect(&m_sender, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

	setWindowTitle(senderName);
}

void DBusSender::setupUi()
{
	QHBoxLayout	*mainLayout = new QHBoxLayout(this);
	{
		mainLayout->addWidget(&m_async);
		mainLayout->addWidget(&m_sender);
	}
}

void DBusSender::setupDBus()
{
	QDBusConnection	sessionBus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, m_senderName);

	if (!sessionBus.isConnected()) {
		qDebug() << m_senderName << "connectToBus() failed";
		return;
	}

	if (!sessionBus.registerService(m_pathNameBase + m_senderName))
		qDebug() << "registerService() failed";

	if (!sessionBus.registerObject(m_objNameBase + m_senderName, this))
		qDebug() << "registerObject() failed";
}

void DBusSender::createInterface()
{
	QDBusConnection	sessionBus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, m_receiverName);

	if (!sessionBus.isConnected()) {
		qDebug() << m_receiverName << "connectToBus() failed";
		return;
	}

	iface = new com::servicename::ifname(m_pathNameBase + m_receiverName,	//target's service name
										 m_objNameBase + m_receiverName,	//target's path name
										 sessionBus,						//connection
										 this);								//parent
}

void DBusSender::sendMessage()
{
	if (m_async.isChecked())
		sendAsyncMessage();
	else
		sendSyncMessage();

	m_sender.clear();
}

void DBusSender::sendSyncMessage()
{
	QDBusConnection	sessionBus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, m_senderName);

	if (!sessionBus.isConnected()) {
		qDebug() << m_senderName << "connectToBus() failed";
		return;
	}

	QDBusMessage stream = QDBusMessage::createSignal(
								m_objNameBase + m_senderName,
								com::servicename::ifname::staticInterfaceName(),
								"syncSend");

	stream << m_sender.text().toLocal8Bit();
	sessionBus.send(stream);
}

void DBusSender::sendAsyncMessage()
{
	QDBusReply<QString> reply = iface->asyncInvoke(m_sender.text().toLocal8Bit());

	setWindowTitle(reply.value());
}

void DBusSender::asyncInvoke(const QByteArray &text)
{
	qDebug() << text;
}
