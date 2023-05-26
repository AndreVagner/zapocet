#ifndef EDITFIRMA_H
#define EDITFIRMA_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class EditFirma;
}

class EditFirma : public QDialog
{
    Q_OBJECT

public:
    explicit EditFirma(QWidget *parent = nullptr);
    explicit EditFirma(QSqlDatabase *db, QWidget *parent = nullptr);
    ~EditFirma();

private slots:
    void on_buttonBox_accepted();
    void updateICO(int index);
private:
    Ui::EditFirma *ui;
    QSqlDatabase *m_db;
};

#endif // EDITFIRMA_H
