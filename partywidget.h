#ifndef PARTYWIDGET_H
#define PARTYWIDGET_H

#include <QWidget>

namespace Ui {
class GameWidget;
}

class PartyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PartyWidget(QWidget *parent = nullptr);
    virtual ~PartyWidget();
signals:

public slots:

public:

    Ui::GameWidget *ui;
};

#endif // PARTYWIDGET_H
