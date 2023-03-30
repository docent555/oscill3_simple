#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>

Dialog::Dialog(
    double &hh, double &LL, int &Nthh, double &Arr, double &Aii, double &deltaa, QWidget *parent)
    : h(hh), L(LL), Nth(Nthh), Ar(Arr), Ai(Aii), delta(deltaa), QDialog(parent), ui(new Ui::Dialog)
{
   ui->setupUi(this);

   this->setWindowTitle("Параметры");

   //   f.open("input_oscill.in", ios::in);
   //   if (!f) {
   //        qDebug() << "Error opening file input_oscill.in";
   //        exit(1);
   Ar = 0.134;
   Nth = 64;
   delta = 0.5;
   L = 30.6;
   h = 0.03;
   phase_space = 1;
   draw_trajectories = 0;
   //   } else {
   //      f >> h >> L >> Nth >> Ar >> delta >> phase_space >> draw_trajectories;
   //      f.close();
   //   }

   ui->lineEdit_Ar->insert(QString::number(Ar));
   //   ui->lineEdit_Nth->insert(QString::number(Nth));
   ui->lineEdit_delta->insert(QString::number(delta));
   ui->lineEdit_L->insert(QString::number(L));
   //   ui->lineEdit_h->insert(QString::number(h));

   //   if (phase_space == 0) {
   //      ui->radioButton_Ph->setChecked(false);
   //      ui->radioButton_Tr->setChecked(true);
   //   } else {
   //      ui->radioButton_Ph->setChecked(true);
   //      ui->radioButton_Tr->setChecked(false);
   //   }

   //   if (draw_trajectories == 1)
   //      ui->checkBox->setChecked(true);
   //   else
   //      ui->checkBox->setChecked(false);
}

Dialog::~Dialog()
{
   delete ui;
}

void Dialog::on_pushButton_OK_clicked()
{
   QString sAr = ui->lineEdit_Ar->text();
   //   QString sNth = ui->lineEdit_Nth->text();
   QString sdelta = ui->lineEdit_delta->text();
   QString sL = ui->lineEdit_L->text();
   //   QString sh = ui->lineEdit_h->text();

   Ar = sAr.toDouble();
   Nth = 64;
   //   Nth = sNth.toInt();
   delta = sdelta.toDouble();
   L = sL.toDouble();
   //   h = sh.toDouble();
   h = 0.05;

   f.open("input_oscill.in", ios::out);
   if (!f) {
      qDebug() << "Error opening file input_oscill.in";
      exit(1);
   }

   f << h << endl
     << L << endl
     << Nth << endl
     << Ar << endl
     << delta << endl
     << phase_space << endl
     << draw_trajectories << endl;

   qDebug() << phase_space << draw_trajectories;

   f.close();

   this->close();
}

void Dialog::on_radioButton_Tr_toggled(bool checked)
{
   phase_space = 0;
   //   qDebug() << "TR:" << checked;
}

void Dialog::on_radioButton_Ph_toggled(bool checked)
{
   phase_space = 1;
   //   qDebug() << "PH" << checked;
}

void Dialog::on_checkBox_stateChanged(int arg1)
{
   if (arg1 > 0)
      draw_trajectories = 1;
   else
      draw_trajectories = 0;

   //   qDebug() << arg1 << draw_trajectories;
}
