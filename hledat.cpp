#include "hledat.h"
#include "ui_hledat.h"

hledat::hledat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hledat)
{
    ui->setupUi(this);

    // Create the model and set up the table
    model = new QSqlRelationalTableModel(this);
    model->setTable("Faktury");

    // Set the model to the table view
    ui->tableView->setModel(model);
}

hledat::~hledat()
{
    delete ui;
}

void hledat::on_lineEdit_textChanged(const QString &arg1)
{
    // Get the entered search text
    QString searchText = arg1;

    // Set the filter on the model
    model->setFilter("NazevFirmy LIKE '%" + searchText + "%'");

    // Refresh the model to apply the filter
    model->select();
}

void hledat::on_pushButton_clicked()
{
    // Perform any additional actions or operations
}
