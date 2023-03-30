#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "rkn.h"
#include "widgetui.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   explicit MainWindow(QWidget *parent = nullptr);
   ~MainWindow();   

protected:
   void closeEvent(QCloseEvent *) override;
   void keyPressEvent(QKeyEvent *) override;  

private:
   QThread *thread;
   Rkn *r;
   Widgetui *widget;
   int pause;

signals:
   //    void freeEventLoop();

public slots:
   void start_calculating();
   void make_pause();
   void reboot();
};

#endif // MAINWINDOW_H
