#ifndef PRIDATFORMULAR_H
#define PRIDATFORMULAR_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class PridatFormular;
}

class PridatFormular : public QDialog
{
    Q_OBJECT

public:
    explicit PridatFormular(QWidget *parent = nullptr);
    explicit PridatFormular(QSqlDatabase *db, QWidget *parent = nullptr);
    ~PridatFormular();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PridatFormular *ui;
    QSqlDatabase *m_db;
};

#endif // PRIDATFORMULAR_H
