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
    QString name() const;
    ~PlayerWidget();

public slots:
    void setStatusText(const QString&);

private:
    Ui::PlayerWidget *ui;
};

#endif // PLAYERWIDGET_H
