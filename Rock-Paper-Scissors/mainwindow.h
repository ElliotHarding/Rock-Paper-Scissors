#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>

#include "wdg_gameobjectsettingsrow.h"
#include "gameobject.h"
#include "dlg_gamefeild.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void moveEvent(QMoveEvent *moveEvent);

private slots:
    ///Game loop slot
    void onUpdateGameObjects();

    ///Button slots
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_reset_clicked();
    void on_btn_addGameObjectSettings_clicked();
    void on_btn_defaultSettings_clicked();

    ///Spawn settings row slots
    void onSpawnSettingsDelete(QListWidgetItem* pListWidgetItem);
    void onSpawnSettingsTypeChanged(GameObjectType type);

    ///Other slots
    void on_sb_updateFrequency_valueChanged(int frequencyMs);
    void onCollisionResultChanged(QPair<GameObjectType, GameObjectType> typePair, GameObjectType goTypeResult);
    void on_sb_gameSizeX_valueChanged(int width);
    void on_sb_gameSizeY_valueChanged(int height);

private:
    ///Reset and default params
    void reset();
    void setDefaultSettings();

    ///Collision table
    void updateCollisionTableWidgets();
    QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> m_collisionResults;

    ///UI
    Ui::MainWindow *ui;
    DLG_GameFeild* m_pDlgGameFeild = nullptr;

    QTimer* m_pUpdateGameObjectsTimer;

    QList<GameObject*> m_gameObjects;

    void addGameObjectSettingsRow(GameObjectSpawnSettings spawnSettings);
};
#endif // MAINWINDOW_H
