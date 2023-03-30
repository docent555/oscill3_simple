#include "mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QProcess>
#include <QString>

extern QMutex mx;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   r = new Rkn();
   widget = new Widgetui(r, this);
   thread = new QThread();
   pause = 0;

   this->setCentralWidget(widget);
   this->resize(1040, 700);

   // Запуск выполнения метода run будет осуществляться по сигналу запуска от соответствующего потока
   //   connect(thread, &QThread::started, r, &Rkn::calculate);
   connect(thread, SIGNAL(started()), r, SLOT(calculate()));
   // Остановка потока же будет выполняться по сигналу finished от соответствующего объекта в потоке
   //   connect(r, &Rkn::finished, thread, &QThread::terminate);
   connect(r, SIGNAL(finished()), thread, SLOT(terminate()));
   //    connect(r, &Rkn::finished, &thread, &QThread::deleteLater);

   //    // EventLoop для ожидания записи результата
   //    connect(this, SIGNAL(freeEventLoop()), &r->loop, SLOT(quit()));

   //   // Передаём объект r в поток
   //   r->moveToThread(thread);
   //   //    r->moveToThread(&thread);
   //   // Запускаем счет
   //   r->setStop(false);
   //   r->setCalculating(true);
   //   thread->start();
   //   //    thread.start();
   //   start_calculating();
}

void MainWindow::start_calculating()
{
   // Передаём объект r в поток
   r->moveToThread(thread);
   //    r->moveToThread(&thread);
   // Запускаем счет
   r->setStop(false);
   r->setCalculating(true);
   thread->start();
   //    thread.start();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
   if (event->key() == Qt::Key_Space) {
      make_pause();
      //      if (pause == 0) {
      //         mx.lock();
      //         pause++;
      //      } else {
      //         mx.unlock();
      //         pause--;
      //      }
   } else if (event->key() == Qt::Key_Escape) {
      this->close();
   } else if (event->key() == Qt::Key_R) {
      reboot();
   }
}

void MainWindow::make_pause()
{
   if (pause == 0) {
      mx.lock();
      pause++;
      widget->disable_enable_on_stop();
   } else {
      mx.unlock();
      pause--;
      widget->disable_enable_on_start();
   }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   //   QMutexLocker ml(&mx);
   mx.tryLock();
   //   QMessageBox::StandardButton resBtn = QMessageBox::question(this,
   //                                                              " ",
   //                                                              tr("Сохранить траектории?\n"),
   //                                                              QMessageBox::No | QMessageBox::Yes,
   //                                                              QMessageBox::No);

   //   if (resBtn != QMessageBox::Yes) {
   if (true) {
      r->setCalculating(false);
      event->accept();
   } else {
      r->setStop(true);

      QString fileName = QFileDialog::getSaveFileName(this,
                                                      tr("Save File"),
                                                      "results_oscill.dat",
                                                      tr("Data files (*.dat)"));
      double *z = r->getz();
      int ne = r->getNe();
      int nz = r->getnz();
      //      int *it = r->getit();
      //      double **th = r->get_theta();
      //      double **dth = r->get_dtheta();
      complex<double> **p = r->get_p();
      //      QString result, dir_a = dir, dir_k = dir;
      QString result;
      QTextStream ts(&result);
      ts.setFieldWidth(10);
      ts.setFieldAlignment(QTextStream::AlignRight);
      //      QString fileName_a = dir_a.append("\\").append("a.dat");
      QFile file_tr(fileName);

      if (file_tr.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
         for (int i = 0; i < nz; i++) {
            //            QTextStream(&result) << << z[i] << "\t\t";
            ts << z[i] << ' ';
            for (int j = 0; j < ne; j++)
               //               QTextStream(&result) << th[i][j] << "\t\t";
               ts << real(p[j][i]) << ' ';
            for (int j = 0; j < ne; j++)
               //               QTextStream(&result) << dth[i][j] << "\t\t";
               ts << imag(p[j][i]) << ' ';
            //            QTextStream(&result) << '\n';
            ts << '\n';
         }
         file_tr.write(result.toUtf8());
         file_tr.close();
      } else {
         qDebug() << "\nError of file open: " << fileName.toUtf8();
      }

      file_tr.close();

      //      QString fileName_k = dir_k.append("\\").append("eff.dat");
      //      QFile file_k(fileName_k);
      //      result.clear();

      //      if (file_k.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      //         for (int i = 0; i <= *it; i++)
      //            //            QTextStream(&result) << k[i] << '\n';
      //            file_k.write(result.toUtf8());
      //         file_k.close();
      //      } else {
      //         qDebug() << "\nError of file open: " << fileName_k.toUtf8();
      //      }

      //      file_k.close();

      qDebug() << "\nResults recorded!";

      //        emit freeEventLoop();
      r->setCalculating(false);
      //        thread.wait(100);
      event->accept();
   }
   mx.unlock();
}

void MainWindow::reboot()
{
   QString program = QApplication::applicationFilePath();
   QStringList arguments = QApplication::arguments();
   QString workingDirectory = QDir::currentPath();
   QProcess::startDetached(program, arguments, workingDirectory);
   QApplication::exit();
}

MainWindow::~MainWindow() {}
