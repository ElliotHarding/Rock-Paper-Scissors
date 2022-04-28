#ifndef DLG_SETTINGS_H
#define DLG_SETTINGS_H

#include <QDialog>
#include <QListWidgetItem>

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
    int secondsBetweenLoops() const; //Seconds between auto restarted game

    ///Game object properties
    int gameObjectSize() const;

    ///GameObject move settings
    int moveUpdateFrequencyMs() const; //How often gameobjects positions are updated
    int moveRandomDirectionPercentage() const; //Percentage a game object moves in a random direction versus heading towards center
    int centerPushRange() const; //Once within x blocks of center, tend to move game object away from center

private slots:
    void on_btn_addGameObjectSettings_clicked();

    void onDelete(QListWidgetItem* pListWidgetItem);

private:
    Ui::DLG_Settings *ui;
};

#endif // DLG_SETTINGS_H
