#include "pridatformular.h"
#include "ui_pridatformular.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QIntValidator>
#include <QDoubleValidator>

PridatFormular::PridatFormular(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PridatFormular)
{
    ui->setupUi(this);
}

PridatFormular::PridatFormular(QSqlDatabase *db, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::PridatFormular),
    m_db(db)
{
    ui->setupUi(this);

    //Ošetření vkládání
    QIntValidator* cisloValidator = new QIntValidator(this);
    ui->Cislo->setValidator(cisloValidator);

    //Ošetření vkládání
    QDoubleValidator* castkaValidator = new QDoubleValidator(this);
    ui->Castka->setValidator(castkaValidator);

    //Naplnění comboBoxu
    QSqlQuery query(*m_db);
    query.prepare("SELECT ICO, Nazev FROM Firmy");
    if(query.exec())
    {
        while(query.next())
        {
            QString ico = query.value(0).toString();
            QString nazev = query.value(1).toString();
            ui->NazevFirmy->addItem(nazev, ico);
        }
    }
}

PridatFormular::~PridatFormular()
{
    delete ui;
}

void PridatFormular::on_buttonBox_accepted()
{
    //Získání dat z formuláře
    int cislo = ui->Cislo->text().toInt();
    int nazev = ui->NazevFirmy->currentData().toInt();
    double castka = ui->Castka->text().toDouble();
    bool zaplaceno = ui->Zaplaceno->isChecked();

    //Kontrola, jestli číslo faktury už neexistuje
    QSqlQuery checkQuery(*m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM Faktury WHERE CisloFaktury = :cislo");
    checkQuery.bindValue(":cislo", cislo);
    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            qDebug() << "Faktura s číslem" << cislo << "již existuje.";
            return;
        }
    }

    //Vytvoření dotazu
    QSqlQuery insertQuery(*m_db);
    insertQuery.prepare("INSERT INTO Faktury (CisloFaktury, NazevFirmy, Castka, Zaplaceno) "
                        "VALUES (:cislo, :nazev, :castka, :zaplaceno)");
    insertQuery.bindValue(":cislo", cislo);
    insertQuery.bindValue(":nazev", nazev);
    insertQuery.bindValue(":castka", castka);
    insertQuery.bindValue(":zaplaceno", zaplaceno);

    if (!insertQuery.exec()) {
        qDebug() << "Chyba při vkládání do tabulky faktury: " << insertQuery.lastError().text();
    }
}

