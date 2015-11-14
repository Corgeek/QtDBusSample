#ifndef DBUS_SENDER_H
#define DBUS_SENDER_H

#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include "sample_adaptor.h"
#include "sample_interface.h"
#include <QtDBus/QDBusConnection>

class DBusSender : public QDialog
{
	Q_OBJECT

	QCheckBox	m_async;
	QLineEdit	m_sender;

	QString	m_pathNameBase;
	QString	m_objNameBase;
	QString m_senderName;
	QString m_receiverName;

	com::servicename::ifname	*iface;

public:
	DBusSender(const QString &pathName,
			   const QString &objName,
			   const QString &senderName,
			   const QString &receiverName,
			   QWidget *parent = 0);

private:
	void setupUi();

	void setupDBus();
	void createInterface();

	void sendSyncMessage();
	void sendAsyncMessage();

private slots:
	void sendMessage();
	void asyncInvoke(const QByteArray &data);
};

#endif // DBUS_SENDER_H
