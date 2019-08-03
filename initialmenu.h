#ifndef INITIALMENU_H
#define INITIALMENU_H

#include <QWidget>

namespace Ui {
class Widget;
}

class InitialMenu : public QWidget
{
    Q_OBJECT
public:
    explicit InitialMenu(QWidget *parent = nullptr);
    ~InitialMenu();

signals:
    void joinServerRequest(QString);

public slots:
    void on_joinServerButton_clicked();
    void showNicknameRefusedLabel();

private:
    Ui::Widget *ui;

};

#endif // INITIALMENU_H
