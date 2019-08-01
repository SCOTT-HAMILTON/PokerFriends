#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

namespace Ui {
class PlayerWidget;
}

class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget *parent = nullptr);
    void setName(const QString &name);
    const QString &name() const;
    ~PlayerWidget();

private:
    Ui::PlayerWidget *ui;
};

#endif // PLAYERWIDGET_H
