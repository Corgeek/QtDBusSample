#ifndef DBUS_RECEIVER_H
#define DBUS_RECEIVER_H

#include <QLineEdit>
#include "sample_adaptor.h"
#include "sample_interface.h"
#include <QtDBus/QDBusConnection>

class DBusReceiver : public QLineEdit
{
	Q_OBJECT

	QString	m_pathNameBase;
	QString	m_objNameBase;
	QString m_receiverName;
	QString m_senderName;

public:
	DBusReceiver(const QString &pathName,
				 const QString &objName,
				 const QString &receiverName,
				 const QString &senderName,
				 QWidget *parent = 0);

public slots:
	void recvSyncMessage(const QByteArray &data);

private:
	void setupDBus();
	void connectSyncMessage();

private slots:
	QString asyncInvoke(const QByteArray &data);
};

#endif // DBUS_RECEIVER_H

