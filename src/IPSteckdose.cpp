#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QQuickView>
#include <QGuiApplication>
#include <QQmlContext>

#include <sailfishapp.h>

#include "ipsmanager.h"

int main(int argc, char *argv[])
{
    int result = 0;

    QGuiApplication *app = SailfishApp::application(argc, argv);
    QQuickView *view = SailfishApp::createView();

    IPSManager ipsmanager;

    view->rootContext()->setContextProperty("ipsmanager", &ipsmanager);

    view->setSource(SailfishApp::pathTo("qml/IPSteckdose.qml"));
    view->show();

    result = app->exec();

    delete view;
    delete app;

    return result;

}

