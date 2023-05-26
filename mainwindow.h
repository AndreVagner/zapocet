#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionKonec_triggered();

    void on_actionNovy_triggered();

    void on_actionOtevrit_triggered();

    void on_actionPridatFirmu_triggered();

    void on_actionPridatFakturu_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void filterTable(const QString &text);

    void on_actionUlozit_jako_triggered();

    void on_actionOdstranit_fakturu_triggered();

    void on_actionOdstranit_firmu_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    void otevritDatabazi();
    void createTables();
    void createmodel();
    void createdb();
    QSqlRelationalTableModel *model;
    QSortFilterProxyModel* vyhledavani;
    void updateStatusBar();
    QLabel* statusLabel;
};
#endif // MAINWINDOW_H
