#ifndef PRIDATFIRME_H
#define PRIDATFIRME_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class pridatfirme;
}

class pridatfirme : public QDialog
{
    Q_OBJECT

public:
    explicit pridatfirme(QWidget *parent = nullptr);
    explicit pridatfirme(QSqlDatabase *db, QWidget *parent = nullptr);
    ~pridatfirme();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::pridatfirme *ui;
    QSqlDatabase *m_db;
};

#endif // PRIDATFIRME_H
