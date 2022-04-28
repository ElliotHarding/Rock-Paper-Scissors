#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "dlg_settings.h"

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
    GameObject(QWidget *parent, GameObjectType goType, const QPoint& spawnPoint);

    void reset(GameObjectType goType, const QPoint& spawnPoint);

    void setType(GameObjectType type);
    GameObjectType getType();

    void checkCollided(GameObject* other);

private:
    void paintEvent(QPaintEvent* paintEvent) override;

    GameObjectType m_type;
    QColor m_color;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUpdateGameObjects();

    void onStart();
    void onStop();
    void onReset();
    void onUpdateMoveFrequency(int frequencyMs);

private:
    void reset();

    Ui::MainWindow *ui;
    DLG_Settings* m_pDlgSettings = nullptr;

    QTimer* m_pUpdateGameObjectsTimer;

    QList<GameObject*> m_gameObjects;
};
#endif // MAINWINDOW_H
