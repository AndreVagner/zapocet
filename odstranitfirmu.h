#ifndef ODSTRANITFIRMU_H
#define ODSTRANITFIRMU_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class OdstranitFirmu;
}

class OdstranitFirmu : public QDialog
{
    Q_OBJECT

public:
    explicit OdstranitFirmu(QWidget *parent = nullptr);
    explicit OdstranitFirmu(QSqlDatabase *db, QWidget *parent = nullptr);
    ~OdstranitFirmu();

private slots:
    void on_buttonBox_2_accepted();
    void updateICO(int index);

private:
    Ui::OdstranitFirmu *ui;
    QSqlDatabase *m_db;

};

#endif // ODSTRANITFIRMU_H
