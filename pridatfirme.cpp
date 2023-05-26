#include "pridatfirme.h"
#include "ui_pridatfirme.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QIntValidator>

pridatfirme::pridatfirme(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pridatfirme)
{
    ui->setupUi(this);
}

pridatfirme::pridatfirme(QSqlDatabase *db, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::pridatfirme),
    m_db(db)
{
    ui->setupUi(this);

    //Ošetření vkládání
    QIntValidator* validator = new QIntValidator(this);
    ui->ICO->setValidator(validator);
}

pridatfirme::~pridatfirme()
{
    delete ui;
}

void pridatfirme::on_buttonBox_accepted()
{
    //Získání dat z formuláře
    int ico = ui->ICO->text().toInt();
    QString nazev = ui->Nazev->text();

    //Vytvoření dotazu
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO Firmy (ICO, Nazev) "
                  "VALUES (:ICO, :Nazev)");
    query.bindValue(":ICO", ico);
    query.bindValue(":Nazev", nazev);

    if (!query.exec()) {
        qDebug() << "Chyba při vkládání do tabulky firmy: " << query.lastError().text();
    }
}
