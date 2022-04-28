#ifndef DLG_SETTINGS_H
#define DLG_SETTINGS_H

#include <QDialog>
#include <QListWidgetItem>

#include "wdg_gameobjectsettingsrow.h"

namespace Ui {
class DLG_Settings;
}

class DLG_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit DLG_Settings(QWidget *parent = nullptr);
    ~DLG_Settings();

    ///Loop settings
    bool loopGame(); //Auto restart game
    int msBetweenLoops() const; //Seconds between auto restarted game

    ///Game object properties
    QList<GameObjectSpawnSettings> spawnSettings();

    ///GameObject move settings
    int moveUpdateFrequencyMs() const; //How often gameobjects positions are updated
    int moveRandomDirectionPercentage() const; //Percentage chance a game object moves in a random direction versus heading towards center
    int centerPushRange() const; //Once within x blocks of center, tend to move game object away from center

signals:
    void onStart();
    void onStop();
    void onReset();
    void onUpdateMoveFrequency(int moveFrequencyMs);

private slots:
    void on_btn_addGameObjectSettings_clicked();

    void onDelete(QListWidgetItem* pListWidgetItem);

    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_reset_clicked();

    void on_sb_updateFrequency_valueChanged(int value);

private:
    Ui::DLG_Settings *ui;

    void addGameObjectSettingsRow(GameObjectSpawnSettings spawnSettings);

    void setDefaultSettings();
};

#endif // DLG_SETTINGS_H
