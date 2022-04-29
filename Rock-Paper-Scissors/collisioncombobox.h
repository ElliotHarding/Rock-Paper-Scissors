#ifndef COLLISIONCOMBOBOX_H
#define COLLISIONCOMBOBOX_H

#include "gameobjecttype.h"
#include <QComboBox>

class CollisionCombobox : public QComboBox
{
    Q_OBJECT

public:
    CollisionCombobox(QPair<GameObjectType, GameObjectType> typePair);

signals:
    void onChanged(QPair<GameObjectType, GameObjectType> typePair, GameObjectType goTypeResult);

private slots:
    void onChangedInternal(const QString& type);

private:
    QPair<GameObjectType, GameObjectType> m_typePair;
};

#endif // COLLISIONCOMBOBOX_H
