#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>

namespace Constants {
    const float UpdateGameObjectsFrequency = 10;

    const QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> CollisionResults = {
        {QPair<GameObjectType, GameObjectType>(GO_ROCK, GO_ROCK), GO_ROCK},
        {QPair<GameObjectType, GameObjectType>(GO_ROCK, GO_PAPER), GO_PAPER},
        {QPair<GameObjectType, GameObjectType>(GO_ROCK, GO_SCISSORS), GO_ROCK},
        {QPair<GameObjectType, GameObjectType>(GO_PAPER, GO_ROCK), GO_PAPER},
        {QPair<GameObjectType, GameObjectType>(GO_PAPER, GO_PAPER), GO_PAPER},
        {QPair<GameObjectType, GameObjectType>(GO_PAPER, GO_SCISSORS), GO_SCISSORS},
        {QPair<GameObjectType, GameObjectType>(GO_SCISSORS, GO_ROCK), GO_ROCK},
        {QPair<GameObjectType, GameObjectType>(GO_SCISSORS, GO_PAPER), GO_SCISSORS},
        {QPair<GameObjectType, GameObjectType>(GO_SCISSORS, GO_SCISSORS), GO_SCISSORS}
    };
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 5, 5));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 5, 25));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 5, 45));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 25, 5));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 25, 25));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 25, 45));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 45, 5));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 45, 25));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 45, 45));

    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 175, 175));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 175, 155));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 175, 135));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 155, 175));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 155, 155));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 155, 135));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 135, 175));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 135, 155));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 135, 135));

    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 175, 5));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 175, 25));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 175, 45));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 155, 5));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 155, 25));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 155, 45));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 135, 5));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 135, 25));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 135, 45));

    m_pUpdateGameObjectsTimer = new QTimer(this);
    connect(m_pUpdateGameObjectsTimer, SIGNAL(timeout()), this, SLOT(onUpdateGameObjects()));
    m_pUpdateGameObjectsTimer->start(Constants::UpdateGameObjectsFrequency);
}

void MainWindow::reset()
{
    //Wrong - hard coded values

    m_gameObjects[0]->reset(GO_ROCK, 5, 5);
    m_gameObjects[1]->reset(GO_ROCK, 5, 25);
    m_gameObjects[2]->reset(GO_ROCK, 5, 45);
    m_gameObjects[3]->reset(GO_ROCK, 25, 5);
    m_gameObjects[4]->reset(GO_ROCK, 25, 25);
    m_gameObjects[5]->reset(GO_ROCK, 25, 45);
    m_gameObjects[6]->reset(GO_ROCK, 45, 5);
    m_gameObjects[7]->reset(GO_ROCK, 45, 25);
    m_gameObjects[8]->reset(GO_ROCK, 45, 45);

    m_gameObjects[9]->reset(GO_PAPER, 175, 175);
    m_gameObjects[10]->reset(GO_PAPER, 175, 155);
    m_gameObjects[11]->reset(GO_PAPER, 175, 135);
    m_gameObjects[12]->reset(GO_PAPER, 155, 175);
    m_gameObjects[13]->reset(GO_PAPER, 155, 155);
    m_gameObjects[14]->reset(GO_PAPER, 155, 135);
    m_gameObjects[15]->reset(GO_PAPER, 135, 175);
    m_gameObjects[16]->reset(GO_PAPER, 135, 155);
    m_gameObjects[17]->reset(GO_PAPER, 135, 135);

    m_gameObjects[18]->reset(GO_SCISSORS, 175, 5);
    m_gameObjects[19]->reset(GO_SCISSORS, 175, 25);
    m_gameObjects[20]->reset(GO_SCISSORS, 175, 45);
    m_gameObjects[21]->reset(GO_SCISSORS, 155, 5);
    m_gameObjects[22]->reset(GO_SCISSORS, 155, 25);
    m_gameObjects[23]->reset(GO_SCISSORS, 155, 45);
    m_gameObjects[24]->reset(GO_SCISSORS, 135, 5);
    m_gameObjects[25]->reset(GO_SCISSORS, 135, 25);
    m_gameObjects[26]->reset(GO_SCISSORS, 135, 45);
}

MainWindow::~MainWindow()
{
    for(GameObject* go : m_gameObjects)
    {
        delete go;
        go = nullptr;
    }
    m_gameObjects.clear();

    delete ui;
}

void MainWindow::onUpdateGameObjects()
{
    //Update positions
    for(GameObject* go : m_gameObjects)
    {
        int randomValue = QRandomGenerator::global()->generateDouble() * 5;
        if(randomValue == 0)
        {
            if(go->geometry().right() < geometry().right())
            {
                go->setGeometry(go->geometry().translated(1, 0));
            }
        }
        else if(randomValue == 1)
        {
            if(go->geometry().left() > 0)
            {
                go->setGeometry(go->geometry().translated(-1, 0));
            }
        }
        else if(randomValue == 2)
        {
            if(go->geometry().bottom() < geometry().bottom())
            {
                go->setGeometry(go->geometry().translated(0, 1));
            }
        }
        else if(randomValue == 3)
        {
            if(go->geometry().top() > 0)
            {
                go->setGeometry(go->geometry().translated(0, -1));
            }
        }
        else
        {
            const int deltaX = geometry().center().x() - geometry().left() - go->geometry().x();
            const int deltaY = geometry().center().y() - geometry().top() - go->geometry().y();
            go->setGeometry(go->geometry().translated(deltaX > 0 ? 1 : -1, deltaY > 0 ? 1 : -1));
        }
    }

    //Check collisions - update types - check if all the same
    const GameObjectType firstGOT = m_gameObjects[0]->getType();
    bool allSame = true;
    for(GameObject* go1 : m_gameObjects)
    {
        for(GameObject* go2 : m_gameObjects)
        {
            go1->checkCollided(go2);
        }

        if(allSame && go1->getType() != firstGOT)
        {
            allSame = false;
        }
    }

    if(allSame)
    {
        //Todo update winner

        reset();
    }

    update();
}

QColor getTypeColor(GameObjectType type)
{
    switch (type)
    {
    case GO_PAPER:
        return Qt::red;
    case GO_ROCK:
        return Qt::blue;
    case GO_SCISSORS:
        return Qt::green;
    default:
        qDebug() << "getTypeColor - unknown type";
    }
}

GameObject::GameObject(QWidget *parent, GameObjectType goType, const int& xPos, const int& yPos) : QWidget(parent)
{
    setType(goType);
    setGeometry(xPos, yPos, 10, 10);//Todo constify
}

void GameObject::reset(GameObjectType goType, const int &xPos, const int &yPos)
{
    setType(goType);
    setGeometry(xPos, yPos, 10, 10);//Todo constify
}

void GameObject::setType(GameObjectType type)
{
    m_type = type;
    m_color = getTypeColor(m_type);
}

GameObjectType GameObject::getType()
{
    return m_type;
}

void GameObject::checkCollided(GameObject *other)
{
    if(geometry().intersects(other->geometry()))
    {
        setType(Constants::CollisionResults[QPair<GameObjectType, GameObjectType>(m_type, other->getType())]);
    }
}

void GameObject::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0,geometry().width(),geometry().height()), m_color);
}
