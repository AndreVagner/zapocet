#ifndef DIALOGPRIDATFIRMU_H
#define DIALOGPRIDATFIRMU_H

#include <QDialog>

namespace Ui {
class Dialogpridatfirmu;
}

class Dialogpridatfirmu : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogpridatfirmu(QWidget *parent = nullptr);
    ~Dialogpridatfirmu();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialogpridatfirmu *ui;
};

#endif // DIALOGPRIDATFIRMU_H
