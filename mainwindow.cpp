#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QFileDialog>
#include "pridatformular.h"
#include "pridatfirme.h"
#include "odstranitfakturu.h"
#include "odstranitfirmu.h"
#include <QStyledItemDelegate>
#include <QTextOption>
#include <QPainter>
#include <QRegularExpression>
#include <QSpinBox>

//Vykreslení fajky nebo křížku, místo 1,0 do sloupce Zaplaceno
class ZaplacenoDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        bool zaplaceno = index.data(Qt::DisplayRole).toBool();
        painter->save();

        QTextOption textOption;
        textOption.setAlignment(Qt::AlignCenter);

        QPen pen;
        if (zaplaceno) {
            pen.setColor(Qt::green);
        } else {
            pen.setColor(Qt::red);
        }
        painter->setPen(pen);

        if (zaplaceno) {
            painter->drawText(option.rect, QString(QChar(0x2713)), textOption);
        } else {
            painter->drawText(option.rect, QString(QChar(0x2717)), textOption);
        }
        painter->restore();
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    ui->setupUi(this);
    setWindowTitle("Kvalitní fakturační program");

    model = new QSqlRelationalTableModel(this); //Vytvoření modelu
    vyhledavani = new QSortFilterProxyModel(this); //Vytvoření proxy modelu
    vyhledavani->setSourceModel(model); //Nastavení původního modelu jako zdroje pro proxy model
    ui->tableView->setModel(vyhledavani); //Nastavení modelu do tabulky
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_textChanged);

    //Vytvoření, nastavení a přidání labelu do statusBaru
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("statusLabel");
    ui->statusbar->addWidget(statusLabel);

}

MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------
void MainWindow::on_actionNovy_triggered()
{

    QString filePath = QFileDialog::getSaveFileName(this, "Uložit databázi jako", "", "SQLite Database (*.db)");

    if (!filePath.isEmpty()) {
        db.setDatabaseName(filePath);

        if (db.open()) {
            createdb();
        }
    }
}

void MainWindow::on_actionOtevrit_triggered()
{
    otevritDatabazi();
}

void MainWindow::on_actionKonec_triggered()
{
    close();
}

void MainWindow::on_actionUlozit_jako_triggered()
{
    QString currentFilePath = db.databaseName();

    QString newFilePath = QFileDialog::getSaveFileName(this, "Uložit databázi jako", "", "SQLite Database (*.db)");

    if (!newFilePath.isEmpty()) {
        db.close();

        //Odstraní existující soubor stejného jména
        if (QFile::exists(newFilePath)) {
            if (!QFile::remove(newFilePath)) {
                qDebug() << "Chyba při odstraňování souboru: " << db.lastError().text();
                return;
            }
        }

        // Kopíruje databázi do nové lokace
        if (QFile::copy(currentFilePath, newFilePath)) {
            qDebug() << "Databáze uložena do:" << newFilePath;

            //Otevře zkopírovanou databázi
            db.setDatabaseName(newFilePath);
            if (db.open()) {
                createmodel();
            }
        } else {
            qDebug() << "Chyba při ukládání databáze: " << db.lastError().text();
        }
    }
}


//-----------------------------------------------------------------
void MainWindow::on_actionPridatFirmu_triggered()
{
    pridatfirme dialog(&db);
    dialog.exec();
}

void MainWindow::on_actionOdstranit_firmu_triggered()
{
    OdstranitFirmu dialog(&db);
    dialog.exec();
    model->select();
    updateStatusBar();
}

void MainWindow::on_actionPridatFakturu_triggered()
{
    PridatFormular dialog(&db);
    dialog.exec();
    model->select();
    updateStatusBar();
}

void MainWindow::on_actionOdstranit_fakturu_triggered()
{
    OdstranitFakturu dialog(&db);
    dialog.exec();
    model->select();
    updateStatusBar();
}

//-----------------------------------------------------------------
void MainWindow::createTables()
{
    if (!db.open()) {
        qDebug() << "Chyba při otvírání databáze: " << db.lastError().text();
        return;
    }
    createdb();
}


void MainWindow::otevritDatabazi()
{
    if(!db.open())
    {
        qDebug() << "Chyby při otvírání databáze: " << db.lastError().text();
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this, "Otevřít databázi", "", "SQLite Database (*.db)");

    if (!filePath.isEmpty()) {
        db.setDatabaseName(filePath);

        if (db.open()) {
            createmodel();
            updateStatusBar();
        } else {
            qDebug() << "Chyby při otvírání databáze: " << db.lastError().text();
        }
    }
}

void MainWindow::createdb()
{
    QSqlQuery query(db);

    //Vytvoření tabulky Firmy
    query.exec("DROP TABLE Firmy; ");
    query.exec("CREATE TABLE Firmy ("
               "ICO INTEGER PRIMARY KEY,"
               "Nazev TEXT"
               ")");

    //Vytvoření tabulky Faktury
    query.exec("DROP TABLE Faktury; ");
    query.exec("CREATE TABLE Faktury ("
               "CisloFaktury INTEGER PRIMARY KEY,"
               "NazevFirmy INTEGER,"
               "Castka REAL,"
               "Zaplaceno BOOLEAN,"
               "FOREIGN KEY(NazevFirmy) REFERENCES Firmy(ICO)"
               ")");

    if (query.lastError().isValid()) {
        qDebug() << "Chyba při vytváření databáze:" << query.lastError().text();
    } else {
        createmodel();
        updateStatusBar();
    }
}

void MainWindow::createmodel()
{
    //Znovu zapne vypnutý toolBar, lineEdit (pro vyhledávání) a uložit jako
    ui->toolBar->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->actionUlozit_jako->setEnabled(true);

    model = new QSqlRelationalTableModel(this, db);
    model->setTable("Faktury"); //Nastavení tabulky
    model->setRelation(model->fieldIndex("NazevFirmy"), QSqlRelation("Firmy", "ICO", "Nazev"));
    model->select(); //Načtení dat

    //Vytvoří instanci delegate a aplikuje jí pro sloupec Zaplaceno
    ZaplacenoDelegate *delegate = new ZaplacenoDelegate();
    ui->tableView->setItemDelegateForColumn(model->fieldIndex("Zaplaceno"), delegate);

    vyhledavani->setSourceModel(model); //Nastavení modelu pro vyhledávání

    //Nastaví tableView na read only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    updateStatusBar();
}

//Filtrování tabulky dle jména
void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    filterTable(arg1);
}

void MainWindow::filterTable(const QString &text)
{
    QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
    vyhledavani->setFilterRegularExpression(regex);
    vyhledavani->setFilterKeyColumn(1);
}

void MainWindow::updateStatusBar()
{
    QSqlQuery query(db);
    //Celkový počet faktur v tabulce
    query.exec("SELECT COUNT(*) FROM Faktury");
    if (query.next()) {
        int numRows = query.value(0).toInt();
        statusLabel->setText("Počet faktur: " + QString::number(numRows));
    }

    //Počet řádku, kde je Zaplaceno true (1)
    query.exec("SELECT COUNT(*) FROM Faktury WHERE Zaplaceno = 1");
    if (query.next()) {
        int numZaplacenoTrue = query.value(0).toInt();
        statusLabel->setText(statusLabel->text() + " | Zaplaceno: " + QString::number(numZaplacenoTrue));
    }

    //Počet řádku, kde je Zaplaceno false (0)
    query.exec("SELECT COUNT(*) FROM Faktury WHERE Zaplaceno = 0");
    if (query.next()) {
        int numZaplacenoFalse = query.value(0).toInt();
        statusLabel->setText(statusLabel->text() + " | Nezaplaceno: " + QString::number(numZaplacenoFalse));
    }
}

