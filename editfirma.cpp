#include "editfirma.h"
#include "ui_editfirma.h"
#include <QSqlQuery>
#include <QSqlError>

EditFirma::EditFirma(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditFirma)
{
    ui->setupUi(this);
}

EditFirma::EditFirma(QSqlDatabase *db, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::EditFirma),
    m_db(db)
{
    ui->setupUi(this);
    // Fill the firm names in the combobox
    QSqlQuery query(*m_db);
    query.prepare("SELECT Nazev, ICO FROM Firmy");
    if(query.exec())
    {
        while(query.next())
        {
            QString nazev = query.value(0).toString();
            QString ico = query.value(1).toString();
            ui->Vybrat->addItem(nazev);
            int index = ui->Vybrat->count() - 1;
            ui->Vybrat->setItemData(index, ico, Qt::UserRole);
        }
    }

    updateICO(0);
    connect(ui->Vybrat, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EditFirma::updateICO);
}

void EditFirma::updateICO(int index)
{
    QString ico = ui->Vybrat->itemData(index, Qt::UserRole).toString();
    ui->ICO->setText(ico);
    QString nazev = ui->Vybrat->currentText();
    ui->Nazev->setText(nazev);
}

EditFirma::~EditFirma()
{
    delete ui;
}

void EditFirma::on_buttonBox_accepted()
{
    // Get the selected firm's name and ICO
    QString selectedNazev = ui->Vybrat->currentText();
    QString selectedICO = ui->ICO->text();
    QString newNazev = ui->Nazev->text();

    QSqlQuery query(*m_db);
    query.prepare("UPDATE Firmy SET ICO = :ico WHERE Nazev = :nazev, Nazev = :novyNazev WHERE ICO = :ico ");
    query.bindValue(":novyNazev", newNazev);
    query.bindValue(":ico", selectedICO);
    query.bindValue(":nazev", selectedNazev);

    if (!query.exec()) {
        qDebug() << "Failed to update firm's name and ICO:" << query.lastError().text();
        return;
    }

    // Update the firm's ICO in the corresponding rows of the Faktury table
    QSqlQuery updateFakturyQuery(*m_db);
    updateFakturyQuery.prepare("UPDATE Faktury SET NazevFirmy = :novyNazev WHERE NazevFirmy = :nazev");
    updateFakturyQuery.bindValue(":novyNazev", selectedNazev);
    updateFakturyQuery.bindValue(":nazev", selectedNazev);

    if (!updateFakturyQuery.exec()) {
        qDebug() << "Failed to update corresponding rows in Faktury table:" << updateFakturyQuery.lastError().text();
        return;
    }

    // Handle the successful update, e.g., show a message or update UI
}





