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

private slots:
    void on_btn_addGameObjectSettings_clicked();

    void onDelete(QListWidgetItem* pListWidgetItem);

private:
    Ui::DLG_Settings *ui;
};

#endif // DLG_SETTINGS_H
