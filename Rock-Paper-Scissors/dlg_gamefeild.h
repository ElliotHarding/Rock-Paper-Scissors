#ifndef DLG_GAMEFEILD_H
#define DLG_GAMEFEILD_H

#include <QDialog>

namespace Ui {
class DLG_GameFeild;
}

class DLG_GameFeild : public QDialog
{
    Q_OBJECT

public:
    explicit DLG_GameFeild(QWidget *parent = nullptr);
    ~DLG_GameFeild();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::DLG_GameFeild *ui;

    QWidget* m_pParent;
};

#endif // DLG_GAMEFEILD_H
