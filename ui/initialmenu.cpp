#include "initialmenu.h"
#include "ui_widget.h"
#include "size.h"

InitialMenu::InitialMenu(QWidget *parent) : QWidget(parent),
  ui(new Ui::Widget)
{


    ui->setupUi(this);
    ui->nicknameRefusedLabel->hide();

    {
            ui->verticalLayout->setSpacing( static_cast<int>(6.0*Size::SIZE_FACTOR) );
            int vertLayMarge = static_cast<int>(11.0*Size::SIZE_FACTOR);
            ui->verticalLayout->setContentsMargins(vertLayMarge, vertLayMarge, vertLayMarge, vertLayMarge);
            ui->verticalSpacer->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR),
                                           static_cast<int>(40.0*Size::SIZE_FACTOR),
                                           QSizePolicy::Minimum, QSizePolicy::Expanding);


            QFont font;
            font.setFamily(QString::fromUtf8("Nimbus Sans"));
            font.setPointSize(FONT_SIZE_2);
            ui->nicknameRefusedLabel->setFont(font);


            ui->verticalSpacer_2->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR),
                                               static_cast<int>(200.0*Size::SIZE_FACTOR),
                                               QSizePolicy::Minimum, QSizePolicy::Expanding);


            ui->horizontalLayout->setSpacing(static_cast<int>(6.0*Size::SIZE_FACTOR));
            ui->label->setMinimumSize(static_cast<int>(200.0*Size::SIZE_FACTOR),
                                      static_cast<int>(50.0*Size::SIZE_FACTOR));
            ui->label->setMaximumSize(static_cast<int>(200.0*Size::SIZE_FACTOR),
                                      static_cast<int>(25.0*Size::SIZE_FACTOR));
            QFont font1;
            font1.setFamily(QString::fromUtf8("Nimbus Sans"));
            font1.setPointSize(FONT_SIZE_3);
            ui->label->setFont(font1);

            ui->horizontalLayout_3->setSpacing(static_cast<int>(6.0*Size::SIZE_FACTOR));
            ui->nickNameLineEdit->setMinimumSize(static_cast<int>(100.0*Size::SIZE_FACTOR),
                                                 static_cast<int>(50.0*Size::SIZE_FACTOR));
            ui->nickNameLineEdit->setMaximumSize(static_cast<int>(200.0*Size::SIZE_FACTOR), 16777215);
            QFont font2;
            font2.setFamily(QString::fromUtf8("Source Code Variable"));
            font2.setPointSize(FONT_SIZE_2);
            font2.setBold(true);
            font2.setWeight(75);
            ui->nickNameLineEdit->setFont(font2);

            ui->verticalSpacer_4->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR),
                                             static_cast<int>(200.0*Size::SIZE_FACTOR),
                                             QSizePolicy::Minimum, QSizePolicy::Expanding);

            ui->horizontalLayout_2->setSpacing(static_cast<int>(6.0*Size::SIZE_FACTOR));
            ui->joinServerButton->setMinimumSize(static_cast<int>(300.0*Size::SIZE_FACTOR),
                                                 static_cast<int>(60.0*Size::SIZE_FACTOR));
            ui->joinServerButton->setMaximumSize(static_cast<int>(300.0*Size::SIZE_FACTOR), 16777215);
            QFont font3;
            font3.setFamily(QString::fromUtf8("Nimbus Sans"));
            font3.setPointSize(FONT_SIZE_2);
            ui->joinServerButton->setFont(font3);

            ui->verticalSpacer_3->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR),
                                             static_cast<int>(400.0*Size::SIZE_FACTOR),
                                             QSizePolicy::Minimum, QSizePolicy::Expanding);
        }
    setFixedSize(Size::APP_SIZEW, Size::APP_SIZEH);
}

InitialMenu::~InitialMenu()
{
    delete ui;
}

void InitialMenu::on_joinServerButton_clicked()
{
    emit joinServerRequest(ui->nickNameLineEdit->text());
}

void InitialMenu::showNicknameRefusedLabel()
{
    ui->nicknameRefusedLabel->show();
}
