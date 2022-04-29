#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "gameobjecttype.h"
#include <QWidget>

class GameObject : public QWidget
{
    Q_OBJECT

public:
    GameObject(QWidget *parent, GameObjectType goType, const QPoint& spawnPoint);

    void reset(GameObjectType goType, const QPoint& spawnPoint);

    void setType(GameObjectType type);
    GameObjectType getType();

    void checkCollided(GameObject* other, QMap<QPair<GameObjectType, GameObjectType>, GameObjectType>& collisionResults);

private:
    void paintEvent(QPaintEvent* paintEvent) override;

    GameObjectType m_type;
    QColor m_color;
};

#endif // GAMEOBJECT_H
