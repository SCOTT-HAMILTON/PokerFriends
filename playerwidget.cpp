#include "playerwidget.h"
#include "ui_playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerWidget)
{
    ui->setupUi(this);
}

void PlayerWidget::setName(const QString &name)
{
    ui->label->setText(name);
}

QString PlayerWidget::name() const
{
    return ui->label->text();
}

PlayerWidget::~PlayerWidget()
{
    delete ui;
}
