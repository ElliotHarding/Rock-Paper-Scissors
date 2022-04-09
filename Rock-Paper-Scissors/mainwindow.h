#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum GameObjectType
{
    GO_ROCK,
    GO_PAPER,
    GO_SCISSORS
};

class GameObject : public QWidget
{
    Q_OBJECT

public:
    GameObject(QWidget *parent, GameObjectType goType);

    void setType(GameObjectType type);
    GameObjectType getType();

    void checkCollided(GameObject* other);

private:
    void paintEvent(QPaintEvent* paintEvent) override;

    GameObjectType m_type;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
