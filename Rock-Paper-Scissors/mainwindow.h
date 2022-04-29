#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "wdg_gameobjectsettingsrow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    ///Game loop slot
    void onUpdateGameObjects();

    ///Button slots
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_reset_clicked();
    void on_btn_addGameObjectSettings_clicked();

    ///Other slots
    void on_sb_updateFrequency_valueChanged(int frequencyMs);
    void onDelete(QListWidgetItem* pListWidgetItem);

private:
    void reset();
    void setDefaultSettings();

    Ui::MainWindow *ui;

    QTimer* m_pUpdateGameObjectsTimer;

    QList<GameObject*> m_gameObjects;

    void addGameObjectSettingsRow(GameObjectSpawnSettings spawnSettings);



    void updateCollisionTable();
    QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> m_collisionResults;
};
#endif // MAINWINDOW_H
