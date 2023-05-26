#include "pridatfirmu.h"
#include "ui_pridatfirmu.h"

PridatFirmu::PridatFirmu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PridatFirmu)
{
    ui->setupUi(this);
}

PridatFirmu::~PridatFirmu()
{
    delete ui;
}
