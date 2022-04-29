#include "gameobject.h"

#include <QPainter>
#include <QMap>

namespace Constants
{
const int GameObjectSize = 10;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///GameObject
///
GameObject::GameObject(QWidget *parent, GameObjectType goType, const QPoint& spawnPoint) : QWidget(parent)
{
    reset(goType, spawnPoint);
}

void GameObject::reset(GameObjectType goType, const QPoint& spawnPoint)
{
    setType(goType);
    setGeometry(spawnPoint.x(), spawnPoint.y(), Constants::GameObjectSize, Constants::GameObjectSize);
}

void GameObject::setType(GameObjectType type)
{
    if(m_type != type)
    {
        m_type = type;
        m_color = QColor(type);
        repaint();
    }
}

GameObjectType GameObject::getType()
{
    return m_type;
}

void GameObject::checkCollided(GameObject *other, QMap<QPair<GameObjectType, GameObjectType>, GameObjectType>& collisionResults)
{
    if(geometry().intersects(other->geometry()))
    {
        setType(collisionResults[QPair<GameObjectType, GameObjectType>(m_type, other->getType())]);
    }
}

void GameObject::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0,Constants::GameObjectSize,Constants::GameObjectSize), m_color);
}
