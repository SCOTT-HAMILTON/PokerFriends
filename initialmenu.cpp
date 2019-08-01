#include "initialmenu.h"
#include "ui_widget.h"

InitialMenu::InitialMenu(QWidget *parent) : QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
}

InitialMenu::~InitialMenu()
{
    delete ui;
}

void InitialMenu::on_joinServerButton_clicked()
{
    emit joinServerRequest(ui->nickNameLineEdit->text());
}
