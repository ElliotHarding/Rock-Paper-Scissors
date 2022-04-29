#include "collisioncombobox.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///CollisionCombobox
///
CollisionCombobox::CollisionCombobox(QPair<GameObjectType, GameObjectType> typePair) : QComboBox(),
    m_typePair(typePair)
{
    connect(this, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onChangedInternal(const QString&)));
}

void CollisionCombobox::onChangedInternal(const QString& type)
{
    emit onChanged(m_typePair, type);
}
