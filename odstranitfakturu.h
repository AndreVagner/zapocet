#ifndef ODSTRANITFAKTURU_H
#define ODSTRANITFAKTURU_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class OdstranitFakturu;
}

class OdstranitFakturu : public QDialog
{
    Q_OBJECT

public:
    explicit OdstranitFakturu(QWidget *parent = nullptr);
    explicit OdstranitFakturu(QSqlDatabase *db, QWidget *parent = nullptr);
    ~OdstranitFakturu();

private slots:
    void on_buttonBox_2_accepted();

private:
    Ui::OdstranitFakturu *ui;
    QSqlDatabase *m_db;
};

#endif // ODSTRANITFAKTURU_H
