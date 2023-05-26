#include "odstranitfirmu.h"
#include "ui_odstranitfirmu.h"
#include <QSqlQuery>
#include <QSqlError>

OdstranitFirmu::OdstranitFirmu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OdstranitFirmu)
{
    ui->setupUi(this);
}

OdstranitFirmu::OdstranitFirmu(QSqlDatabase *db, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OdstranitFirmu),
    m_db(db)
{
    ui->setupUi(this);
    //Naplnění comboBoxu
    QSqlQuery query(*m_db);
    query.prepare("SELECT Nazev, ICO FROM Firmy");
    if(query.exec())
    {
        while(query.next())
        {
            QString nazev = query.value(0).toString();
            QString ico = query.value(1).toString();
            ui->Nazev_2->addItem(nazev);
            int index = ui->Nazev_2->count() - 1;
            ui->Nazev_2->setItemData(index, ico, Qt::UserRole);
        }
    }

    updateICO(0);
    connect(ui->Nazev_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OdstranitFirmu::updateICO);
}

//Funkce, která aktualizuje IČO podle vybraného indexu (jména) z comboBoxu
void OdstranitFirmu::updateICO(int index)
{
    QString ico = ui->Nazev_2->itemData(index, Qt::UserRole).toString();
    ui->ICO_2->setText(ico);
}

OdstranitFirmu::~OdstranitFirmu()
{
    delete ui;
}

void OdstranitFirmu::on_buttonBox_2_accepted()
{
    //Získání dat z formuláře
    QString selectedNazev = ui->Nazev_2->currentText();
    QString selectedICO = ui->ICO_2->text();

    // Odstranění všech faktur podle IČO, které je propojené s vybraným jménem, z tabulky Firmy
    QSqlQuery queryFaktury(*m_db);
    queryFaktury.prepare("DELETE FROM Faktury WHERE NazevFirmy = :ico");
    queryFaktury.bindValue(":ico", selectedICO);
    if (!queryFaktury.exec()) {
        qDebug() << "Chyba při odstraňování z tabulky Faktury: " << queryFaktury.lastError().text();
    }

    // Odstranění firmy dle vybraného názvu z tabulky Firmy
    QSqlQuery queryFirmy(*m_db);
    queryFirmy.prepare("DELETE FROM Firmy WHERE Nazev = :nazev");
    queryFirmy.bindValue(":nazev", selectedNazev);
    if (!queryFirmy.exec()) {
        qDebug() << "Chyba při odstraňování z tabulky Firmy: " << queryFirmy.lastError().text();
    }
    accept();
}





