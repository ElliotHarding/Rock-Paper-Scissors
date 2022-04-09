#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

GameObject::GameObject(QWidget *parent, GameObjectType goType) : QWidget(parent),
    m_type(goType)
{
}

void GameObject::setType(GameObjectType type)
{
    m_type = type;
}

GameObjectType GameObject::getType()
{
    return m_type;
}

QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> collisionResult = {
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

void GameObject::checkCollided(GameObject *other)
{
    if(geometry().intersects(other->geometry()))
    {
        m_type = collisionResult[QPair<GameObjectType, GameObjectType>(m_type, other->getType())];
    }
}
