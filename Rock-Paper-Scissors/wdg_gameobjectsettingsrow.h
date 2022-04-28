#ifndef WDG_GAMEOBJECTSETTINGSROW_H
#define WDG_GAMEOBJECTSETTINGSROW_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class WDG_GameObjectSettingsRow;
}

class WDG_GameObjectSettingsRow : public QWidget
{
    Q_OBJECT

public:
    explicit WDG_GameObjectSettingsRow(QListWidgetItem* pListWidgetItem);
    ~WDG_GameObjectSettingsRow();

signals:
    void onDelete(QListWidgetItem* pListWidgetItem);

private slots:
    void on_btn_delete_clicked();

private:
    Ui::WDG_GameObjectSettingsRow *ui;

    QListWidgetItem* m_pListWidgetItem = nullptr;
};

#endif // WDG_GAMEOBJECTSETTINGSROW_H
