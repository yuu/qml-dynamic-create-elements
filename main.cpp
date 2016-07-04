#include <QGuiApplication>
#include <QQuickView>

#include <QObject>
#include <QQmlEngine>
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

    if(component.isError()) {
        qDebug() << "Component<error>:" << component.errors();
    }

    QObject *newObject = component.create(parentContext);
    if (newObject)
        reparentQmlObject(newObject, parent);

    QQmlEngine::setObjectOwnership(newObject, QQmlEngine::JavaScriptOwnership);
    QObject::connect(qobject_cast<QQuickItem*>(newObject), SIGNAL(push()), newObject, SLOT(selfDestroy()));
}

auto main(int argc, char** argv) -> int
{
    QString code = R"(
    import QtQuick 2.5

    Rectangle {
    id: root
    width: 200
    height: 200
    color: "blue"

    objectName: "dynamic"
    signal push

    MouseArea {
    anchors.fill: parent
    onPressed: {
    }
    onReleased: {
        push();
    }
    }

    function selfDestroy() {
       root.destroy();
    }

    }

    )";

    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    auto ctx = view.rootContext();
    auto robj = view.rootObject();
    createQmlObject(
            code,
            ctx,
            robj,
            "file"
            );

    return app.exec();
}
