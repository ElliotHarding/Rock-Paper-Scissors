#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>

#include "wdg_gameobjectsettingsrow.h"
#include "gameobject.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void onSpawnSettingsTypeChanged(GameObjectType type);
    void onCollisionResultChanged(QPair<GameObjectType, GameObjectType> typePair, GameObjectType goTypeResult);

    void on_btn_defaultSettings_clicked();

private:
    void reset();
    void setDefaultSettings();

    Ui::MainWindow *ui;

    QTimer* m_pUpdateGameObjectsTimer;

    QList<GameObject*> m_gameObjects;

    void addGameObjectSettingsRow(GameObjectSpawnSettings spawnSettings);


    void updateCollisionTableWidgets();
    QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> m_collisionResults;
};
#endif // MAINWINDOW_H
