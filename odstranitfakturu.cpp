#include "odstranitfakturu.h"
#include "ui_odstranitfakturu.h"
#include <QSqlQuery>
#include <QSqlError>

OdstranitFakturu::OdstranitFakturu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OdstranitFakturu)
{
    ui->setupUi(this);
}

OdstranitFakturu::OdstranitFakturu(QSqlDatabase *db, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OdstranitFakturu),
    m_db(db)

{
    ui->setupUi(this);
    // naplnění comboBoxu
    QSqlQuery query(*m_db);
    query.prepare("SELECT CisloFaktury FROM Faktury");
    if(query.exec())
    {
        while(query.next())
        {
            QString CisloFaktury = query.value(0).toString();
            ui->CisloFaktury_2->addItem(CisloFaktury);
        }
    }
}

OdstranitFakturu::~OdstranitFakturu()
{
    delete ui;
}

void OdstranitFakturu::on_buttonBox_2_accepted()
{
   //Získání dat z formuláře
    int cisloFaktury = ui->CisloFaktury_2->currentText().toInt();

    //Odstranění řádku podle čísla faktury
    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM Faktury WHERE CisloFaktury = ?");
    query.addBindValue(cisloFaktury);
    if (!query.exec()) {
        qDebug() << "Chyba při odstraňování řádku z tabulky Faktury: " << query.lastError().text();
    }
    accept();
}

