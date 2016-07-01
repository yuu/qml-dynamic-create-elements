#include <QGuiApplication>
#include <QQuickView>

#include <QObject>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>
#include <QString>
#include <QByteArray>

// qquickviewinspector.cpp::142
void reparentQmlObject(QObject *object, QObject *newParent)
{
    if (!newParent)
        return;

    object->setParent(newParent);
    QQuickItem *newParentItem = qobject_cast<QQuickItem*>(newParent);
    QQuickItem *item = qobject_cast<QQuickItem*>(object);
    if (newParentItem && item)
        item->setParentItem(newParentItem);
}

//abstractviewinspector.cpp::92
void createQmlObject(const QString &qml, QQmlContext *parentContext, QObject *parent, const QString &filename)
{
    if (!parent)
        return;

    QQmlComponent component(parentContext->engine());
    QByteArray constructedQml = QString(qml).toLatin1();

    component.setData(constructedQml, QUrl::fromLocalFile(filename));
    QObject *newObject = component.create(parentContext);
    if (newObject)
        reparentQmlObject(newObject, parent);
}

auto main(int argc, char** argv) -> int
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    auto ctx = view.rootContext();
    auto robj = view.rootObject();
    createQmlObject(
            "import QtQuick 2.5;Rectangle{id:sample;width:40; height:40;color:\"blue\";}",
            ctx,
            robj,
            "file"
            );

    return app.exec();
}
