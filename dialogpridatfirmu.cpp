#include "dialogpridatfirmu.h"
#include "ui_dialogpridatfirmu.h"

Dialogpridatfirmu::Dialogpridatfirmu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogpridatfirmu)
{
    ui->setupUi(this);
}

Dialogpridatfirmu::~Dialogpridatfirmu()
{
    delete ui;
}

void Dialogpridatfirmu::on_buttonBox_accepted()
{

}

