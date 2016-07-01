import QtQuick 2.5

Item {
    property var obj
    id: container
    width: 500; height: 100;
    Rectangle {
        id: second; width:50;height:50;x:70;color:'green';
        Component.onCompleted: {
            var dynamicObject = Qt.createQmlObject('import QtQuick 2.5;Rectangle{id:sample;width:40; height:40;color:"blue";}', container,'firstObject');
            console.log("Dynamic create to firstObject");
        }
    }

    MouseArea {
        id: mouse1
        anchors.fill: parent;
        onPressed: {
            obj  = Qt.createQmlObject('import QtQuick 2.5;Rectangle{id:container;width:40; height:40;x:80;y:40;color:"pink"; MouseArea { onClicked: { obj.destroy(9000) } }}', container,'firstObject')
            console.log("Dynamic create to secondObject");
        }
        onReleased: {
            release(container.obj);
        }
    }

    function release(objectToDelete) {
        console.log("A dynamic object is deleted");
        objectToDelete.destroy();
    }
}
