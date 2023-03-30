#ifndef DIALOG_H
#define DIALOG_H

#include "rkn.h"
#include <fstream>
#include <QDialog>

using namespace std;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
   Q_OBJECT

public:
   explicit Dialog(double &hh,
                   double &LL,
                   int &Nthh,
                   double &Ar,
                   double &Ai,
                   double &deltaa,
                   QWidget *parent = nullptr);
   ~Dialog();

private slots:
   void on_pushButton_OK_clicked();

   void on_radioButton_Tr_toggled(bool checked);

   void on_radioButton_Ph_toggled(bool checked);

   void on_checkBox_stateChanged(int arg1);

private:
   Ui::Dialog *ui;
   fstream f;

   double &h, &L, &delta, &Ar, &Ai;
   int &Nth, draw_trajectories = 0, phase_space = 0;
};

#endif // DIALOG_H
