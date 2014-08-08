#include "ipsmanager.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QCryptographicHash>

IPSManager::IPSManager(QObject *parent) :
    QObject(parent), m_nonceReply(NULL), m_actionReply(NULL), m_status(tr("Idle")), m_progress(PROGRESS_IDLE)
{
    m_settings = new QSettings("tkolb", "IPSteckdose");
    loadSettings();

    m_nam = new QNetworkAccessManager(this);
}

IPSManager::~IPSManager()
{
    saveSettings();
    delete m_settings;
}

void IPSManager::sendCommand(const QString &cmd)
{
    if(m_nonceReply != NULL || m_actionReply != NULL) {
        setstatus(tr("A request is still pending."));
        return;
    }

    m_cmd = cmd[0].toLatin1();

    // request nounce
    QUrl url(QString("http://%1:%2/ipsp?n").arg(m_host).arg(m_port));

    setstatus(tr("Requesting nonce"));
    setprogress(PROGRESS_ACTIVE);

    m_nonceReply = m_nam->get(QNetworkRequest(url));
    connect(m_nonceReply, SIGNAL(finished()), this, SLOT(nonceRequestFinished()));
}

void IPSManager::loadSettings()
{
    m_host = m_settings->value("host", "example.com").toString();
    m_port = m_settings->value("port", 80).toInt();
    m_password = m_settings->value("password", "password").toString();
}

void IPSManager::saveSettings()
{
    m_settings->setValue("host", m_host);
    m_settings->setValue("port", m_port);
    m_settings->setValue("password", m_password);
}

void IPSManager::cancelRequest()
{
    if(m_nonceReply != NULL) {
        m_nonceReply->abort();
    }

    if(m_actionReply != NULL) {
        m_actionReply->abort();
    }
}

void IPSManager::nonceRequestFinished(void)
{
    if(m_nonceReply == NULL) {
        return;
    }

    if(m_nonceReply->error() == QNetworkReply::NoError) {
        // read server nonce
        QString serverNonce(m_nonceReply->readAll());

        setstatus(tr("Nonce received, sending action"));

        // build client nonce
        int val1 = qrand() & 0xFFFF;
        int val2 = qrand() & 0xFFFF;

        QString clientNonce = QString("0x%1%2").arg(val1, 4, 16, QChar('0')).arg(val2, 4, 16, QChar('0'));

        // build authentication string
        QString authString = QString("%1 %2 %3 %4").arg(serverNonce).arg(clientNonce).arg(m_cmd).arg(m_password);
        QString auth = QCryptographicHash::hash(authString.toLatin1(), QCryptographicHash::Md5).toHex();

        // send request
        QUrl url(QString("http://%1:%2/ipsp?%3&auth=%4&csec=%5").arg(m_host).arg(m_port).arg(m_cmd).arg(auth).arg(clientNonce));

        m_actionReply = m_nam->get(QNetworkRequest(url));
        connect(m_actionReply, SIGNAL(finished()), this, SLOT(actionRequestFinished()));
    } else {
        setstatus(tr("Cannot get nonce"));
        setprogress(PROGRESS_ERROR);
    }

    m_nonceReply->deleteLater();

    m_nonceReply = NULL;
}

void IPSManager::actionRequestFinished(void)
{
    if(m_actionReply == NULL) {
        return;
    }

    if(m_actionReply->error() == QNetworkReply::NoError) {
        setstatus(tr("Action executed successfully"));
        setprogress(PROGRESS_SUCCESS);
    } else {
        setstatus(tr("Cannot execute action"));
        qDebug() << m_actionReply->errorString();
        setprogress(PROGRESS_ERROR);
    }

    m_actionReply->deleteLater();

    m_actionReply = NULL;
}

void IPSManager::setstatus(const QString &arg)
{
    if (m_status != arg) {
        m_status = arg;
        emit statusChanged(arg);
    }
}

void IPSManager::setprogress(int arg)
{
    if (m_progress != arg) {
        m_progress = arg;
        emit progressChanged(arg);
    }
}
