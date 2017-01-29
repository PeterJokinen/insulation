#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtPrintSupport/QPrinter>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDateTime>

/*
 *  Peter Jokinen
 *  Eristys Versio 1.1 (15.8.2016)
 *
 *
 *
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableMateriaalit->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableMateriaalit->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableMateriaalit->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableRakenne->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->radioButtonLattia->clicked(true);

    //lue materiaalit
    QFile inputFile("Materiaalit.txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QTextStream in(&inputFile);
       int rowCount = 0;
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList list = line.split('|');
          ui->tableMateriaalit->insertRow(rowCount);

          QTableWidgetItem *materiaali1 = new QTableWidgetItem;
          materiaali1->setText(list[0]);
          ui->tableMateriaalit->setItem(rowCount,0,materiaali1);

          QTableWidgetItem *materiaali2 = new QTableWidgetItem;
          materiaali2->setText(list[1]);
          ui->tableMateriaalit->setItem(rowCount,1,materiaali2);

          rowCount++;

       }
       inputFile.close();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonUusiMateriaali_clicked()
{
    ui->tableMateriaalit->insertRow(0);

    QTableWidgetItem *materiaali = new QTableWidgetItem;
    materiaali->setText("Materiaalin nimi");
    ui->tableMateriaalit->setItem(0,0,materiaali);

    QTableWidgetItem *lampo = new QTableWidgetItem;
    lampo->setText("1");
    ui->tableMateriaalit->setItem(0,1,lampo);
}

void MainWindow::on_buttonTyhjenna_clicked()
{
    while(ui->tableRakenne->rowCount() > 0){
        ui->tableRakenne->removeRow(0);
    }
    kalkulaattori();
}

void MainWindow::on_buttoMuokkaaMat_toggled(bool checked)
{
    if(checked == false) {
        if(ui->tableMateriaalit->rowCount() > 0){
            ui->tableMateriaalit->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tableMateriaalit->setSelectionBehavior(QAbstractItemView::SelectRows);
        }

    }
    if(checked == true) {
        if(ui->tableMateriaalit->rowCount() > 0){
            ui->tableMateriaalit->setEditTriggers(QAbstractItemView::AllEditTriggers);
            ui->tableMateriaalit->setSelectionBehavior(QAbstractItemView::SelectItems);
        }

    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableMateriaalit->removeRow(ui->tableMateriaalit->currentRow());
}

void MainWindow::on_buttonOikea_clicked()
{
    bool oldState = ui->tableRakenne->blockSignals(true);
    ui->tableRakenne->insertRow(0);

    QTableWidgetItem *materiaali = new QTableWidgetItem;
    materiaali->setText(ui->tableMateriaalit->item(ui->tableMateriaalit->currentRow(),0)->text());
    materiaali->setFlags(materiaali->flags() & ~Qt::ItemIsEditable);
    ui->tableRakenne->setItem(0,0,materiaali);

    QTableWidgetItem *paksuus = new QTableWidgetItem;
    paksuus->setText("1");
    ui->tableRakenne->blockSignals(oldState);
    ui->tableRakenne->setItem(0,1,paksuus);

}

void MainWindow::on_buttonVasen_clicked()
{
    ui->tableRakenne->removeRow(ui->tableRakenne->currentRow());
    kalkulaattori();
}

void MainWindow::on_buttonYlos_clicked()
{
    if(ui->tableRakenne->currentRow() > 0) {
        //siirrettävä materiaali
        QTableWidgetItem *materiaaliNimi = new QTableWidgetItem;
        QTableWidgetItem *materiaaliData = new QTableWidgetItem;
        materiaaliNimi->setText(ui->tableRakenne->item(ui->tableRakenne->currentRow(),0)->text());
        materiaaliNimi->setFlags(materiaaliNimi->flags() & ~Qt::ItemIsEditable);
        materiaaliData->setText(ui->tableRakenne->item(ui->tableRakenne->currentRow(),1)->text());

        //yläpuolella oleva materiaali
        QTableWidgetItem *upNimi = new QTableWidgetItem;
        QTableWidgetItem *upData = new QTableWidgetItem;
        upNimi->setText(ui->tableRakenne->item((ui->tableRakenne->currentRow() - 1),0)->text());
        upNimi->setFlags(upNimi->flags() & ~Qt::ItemIsEditable);
        upData->setText(ui->tableRakenne->item((ui->tableRakenne->currentRow() - 1),1)->text());

        ui->tableRakenne->setItem(ui->tableRakenne->currentRow(),0,upNimi);
        ui->tableRakenne->setItem(ui->tableRakenne->currentRow(),1,upData);

        ui->tableRakenne->setItem((ui->tableRakenne->currentRow() -1),0,materiaaliNimi);
        ui->tableRakenne->setItem((ui->tableRakenne->currentRow() - 1),1,materiaaliData);

        ui->tableRakenne->setCurrentCell((ui->tableRakenne->currentRow() -1),ui->tableRakenne->currentColumn());
    }

}

void MainWindow::on_buttonAlas_clicked()
{
    if(ui->tableRakenne->currentRow() < (ui->tableRakenne->rowCount() - 1)) {
        //siirrettävä materiaali
        QTableWidgetItem *materiaaliNimi = new QTableWidgetItem;
        QTableWidgetItem *materiaaliData = new QTableWidgetItem;
        materiaaliNimi->setText(ui->tableRakenne->item(ui->tableRakenne->currentRow(),0)->text());
        materiaaliNimi->setFlags(materiaaliNimi->flags() & ~Qt::ItemIsEditable);
        materiaaliData->setText(ui->tableRakenne->item(ui->tableRakenne->currentRow(),1)->text());

        //alapuolella oleva materiaali
        QTableWidgetItem *downNimi = new QTableWidgetItem;
        QTableWidgetItem *downData = new QTableWidgetItem;
        downNimi->setText(ui->tableRakenne->item((ui->tableRakenne->currentRow() + 1),0)->text());
        downNimi->setFlags(downNimi->flags() & ~Qt::ItemIsEditable);
        downData->setText(ui->tableRakenne->item((ui->tableRakenne->currentRow() + 1),1)->text());

        ui->tableRakenne->setItem(ui->tableRakenne->currentRow(),0,downNimi);
        ui->tableRakenne->setItem(ui->tableRakenne->currentRow(),1,downData);

        ui->tableRakenne->setItem((ui->tableRakenne->currentRow() + 1),0,materiaaliNimi);
        ui->tableRakenne->setItem((ui->tableRakenne->currentRow() + 1),1,materiaaliData);

        ui->tableRakenne->setCurrentCell((ui->tableRakenne->currentRow() + 1),ui->tableRakenne->currentColumn());
    }
}

void MainWindow::kalkulaattori()
{
    if(ui->tableRakenne->rowCount() > 0) {
        QString otsikko = "Rakenne";
        QString stringLampo = "";
        QString stringPaksuus = "";
        double lampo = 1;
        double paksuus = 1;
        double summa = 0;

        QDateTime now = QDateTime::currentDateTime();
        QString pvm_aika = now.toString("dd.MM.yyyy hh:mm");

        if(ui->radioButtonLattia->isChecked()) {
            otsikko = "Lattia rakenne";
        }
        else if (ui->radioButtonSeina->isChecked()) {
            otsikko = "Seinä rakenne";
        }
        else if (ui->radioButtonKatto->isChecked()) {
            otsikko = "Katto rakenne";
        }

        QString tulostus = "<h><u>" + otsikko + "</u> " + pvm_aika + "</h>" ;
        tulostus = tulostus + "<p>" + ui->textEdit->toHtml() + "</p>" ;

        tulostus = tulostus + "<div class='tg-wrap'><table>";
        QString pintaVastusSisa;
        QString pintaVastusUlko = "0.04";

        if(ui->radioButtonLattia->isChecked()) {
            pintaVastusSisa = "0.17";
            summa = summa + 0.04 + 0.17;
        }
        else if (ui->radioButtonSeina->isChecked()) {
            pintaVastusSisa = "0.13";
            summa = summa + 0.04 + 0.13;
        }
        else if (ui->radioButtonKatto->isChecked()) {
            pintaVastusSisa = "0.10";
            summa = summa + 0.04 + 0.10;
        }

        tulostus = tulostus + "<tr><td>Ulkoinen pintavastus</td><td align='right'> = </td><td>" + pintaVastusUlko + "</td></tr>";

        for(int i =0; i < ui->tableRakenne->rowCount(); i++) {
            for(int j =0; j < ui->tableMateriaalit->rowCount(); j++) {
                if (ui->tableRakenne->item(i,0)->text() == ui->tableMateriaalit->item(j,0)->text()){
                    stringLampo = ui->tableMateriaalit->item(j,1)->text();
                    lampo = stringLampo.toDouble();

                    stringPaksuus = ui->tableRakenne->item(i,1)->text();
                    paksuus = stringPaksuus.toDouble();

                    double tulos = paksuus / lampo;
                    summa = summa + tulos;
                    QString stringTulos = QString::number(tulos);
                    tulostus = tulostus + "<tr><td>" + ui->tableRakenne->item(i,0)->text() + "</td><td align='right'>" + stringPaksuus + "m / " + stringLampo + " = </td><td>" + stringTulos + "</td></tr>";
                    break;
                }

            }
        }

        tulostus = tulostus + "<tr><td>Sisäinen pintavastus</td><td align='right'> = </td><td>" + pintaVastusSisa + "</td></tr>";

        tulostus = tulostus + "<tr ><td></td><td align='right'>M = </td><td>" + QString::number(summa) + "</td></tr>";
        double tulos = 1 / summa;
        tulostus = tulostus + "<tr><td></td><td align='right'>U = 1 / M = </td><td>" + QString::number(tulos) + "</td></tr>";

        ui->textBrowser->clear();
        ui->textBrowser->insertHtml(tulostus);
        QTextCursor cursor = ui->textBrowser->textCursor();
        ui->textBrowser->selectAll();
        ui->textBrowser->setFontPointSize(ui->comboBox->currentText().toInt());
        ui->textBrowser->setTextCursor( cursor );
        ui->textBrowser->show();


    }
    else {
        ui->textBrowser->clear();
        ui->textBrowser->insertHtml("Tyhjä");
        QTextCursor cursor = ui->textBrowser->textCursor();
        ui->textBrowser->selectAll();
        ui->textBrowser->setFontPointSize(ui->comboBox->currentText().toInt());
        ui->textBrowser->setTextCursor( cursor );
        ui->textBrowser->show();
    }
}

void MainWindow::on_tableRakenne_cellChanged(int row, int column)
{
    kalkulaattori();


}

void MainWindow::on_radioButtonLattia_clicked()
{
    kalkulaattori();

    QDateTime now = QDateTime::currentDateTime();
    QString tied = "lattia_" + now.toString("dd-MM-yyyy_hh-mm-ss");
    ui->lineEditTiedosto->setText(tied);
}

void MainWindow::on_radioButtonSeina_clicked()
{
    kalkulaattori();

    QDateTime now = QDateTime::currentDateTime();
    QString tied = "seinä_" + now.toString("dd-MM-yyyy_hh-mm-ss");
    ui->lineEditTiedosto->setText(tied);
}

void MainWindow::on_radioButtonKatto_clicked()
{
    kalkulaattori();

    QDateTime now = QDateTime::currentDateTime();
    QString tied = "katto_" + now.toString("dd-MM-yyyy_hh-mm-ss");
    ui->lineEditTiedosto->setText(tied);
}

void MainWindow::on_pushButtonPDF_clicked()
{
    QTextDocument *doc = ui->textBrowser->document();
    QPrinter printer;
    QString desktopLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    printer.setOutputFileName(desktopLocation + "\\" + ui->lineEditTiedosto->text() + ".pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc->print(&printer);
    printer.newPage();
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    kalkulaattori();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString filename = "Materiaalit.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&file);
        for(int i = 0; i < ui->tableMateriaalit->rowCount(); i++) {
            stream << ui->tableMateriaalit->item(i,0)->text() + "|" + ui->tableMateriaalit->item(i,1)->text() << endl;
        }

    }
    file.close();
}

void MainWindow::on_textEdit_textChanged()
{
    kalkulaattori();
}
