#ifndef IPSMANAGER_H
#define IPSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QDebug>

class IPSManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString host READ host WRITE sethost NOTIFY hostChanged)
    Q_PROPERTY(QString password READ password WRITE setpassword NOTIFY passwordChanged)
    Q_PROPERTY(quint16 port READ port WRITE setport NOTIFY portChanged)
    Q_PROPERTY(QString status READ status WRITE setstatus NOTIFY statusChanged)
    Q_PROPERTY(int progress READ progress WRITE setprogress NOTIFY progressChanged)

public:
    static const int PROGRESS_IDLE = 0;
    static const int PROGRESS_ACTIVE = 1;
    static const int PROGRESS_ERROR = 2;
    static const int PROGRESS_SUCCESS = 3;

private:
    QSettings *m_settings;
    QNetworkAccessManager *m_nam;

    QNetworkReply *m_nonceReply;
    QNetworkReply *m_actionReply;

    char m_cmd;

    QString m_host;
    QString m_password;
    quint16 m_port;
    QString m_status;
    int     m_progress;

public:
    explicit IPSManager(QObject *parent = 0);
    ~IPSManager();

    QString host() const { return m_host; }
    QString password() const { return m_password; }
    quint16 port() const { return m_port; }
    QString status() const { return m_status; }
    int progress() const { return m_progress; }

    Q_INVOKABLE void sendCommand(const QString &cmd);
    Q_INVOKABLE void loadSettings(void);
    Q_INVOKABLE void saveSettings(void);
    Q_INVOKABLE void cancelRequest(void);

signals:
    void hostChanged(const QString &arg);
    void passwordChanged(const QString &arg);
    void portChanged(quint16 arg);
    void statusChanged(QString arg);

    void progressChanged(int arg);

public slots:
    void sethost(const QString &arg)
    {
        m_host = arg;
        emit hostChanged(arg);
    }

    void setpassword(const QString &arg)
    {
        m_password = arg;
        emit passwordChanged(arg);
    }

    void setport(quint16 arg)
    {
        m_port = arg;
        emit portChanged(arg);
    }

private slots:
    void nonceRequestFinished(void);
    void actionRequestFinished(void);

    void setstatus(const QString &arg);
    void setprogress(int arg);

};

#endif // IPSMANAGER_H
