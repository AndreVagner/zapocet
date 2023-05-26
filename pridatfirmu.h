#ifndef PRIDATFIRMU_H
#define PRIDATFIRMU_H

#include <QDialog>

namespace Ui {
class PridatFirmu;
}

class PridatFirmu : public QDialog
{
    Q_OBJECT

public:
    explicit PridatFirmu(QWidget *parent = nullptr);
    ~PridatFirmu();

private:
    Ui::PridatFirmu *ui;
};

#endif // PRIDATFIRMU_H
