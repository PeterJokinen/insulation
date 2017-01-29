#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <time.h>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_buttonUusiMateriaali_clicked();

    void on_buttonTyhjenna_clicked();

    void on_buttoMuokkaaMat_toggled(bool checked);

    void on_pushButton_clicked();

    void on_buttonOikea_clicked();

    void on_buttonVasen_clicked();

    void on_buttonYlos_clicked();

    void on_buttonAlas_clicked();

    void kalkulaattori();

    void on_tableRakenne_cellChanged(int row, int column);

    void on_radioButtonLattia_clicked();

    void on_radioButtonSeina_clicked();

    void on_radioButtonKatto_clicked();

    void on_pushButtonPDF_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void closeEvent(QCloseEvent *event);

    void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
