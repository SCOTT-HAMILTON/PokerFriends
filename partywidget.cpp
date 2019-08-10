#include "partywidget.h"
#include "ui_gamewidget.h"

PartyWidget::PartyWidget(QWidget *parent) : QWidget(parent),
  ui(new Ui::GameWidget)
{
    ui->setupUi(this);
}

PartyWidget::~PartyWidget()
{
    delete ui;
}
