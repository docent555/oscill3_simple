/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "widget.h"
#include "rkn.h"

#include <QDebug>
#include <QGridLayout>

Widget::Widget(Rkn *r, QWidget *parent)
    : QWidget(parent), m_listCount(3), m_valueMax(10), m_valueCount(7)
{
   z = r->getz();
   nz = r->getnz();
   //   th = r->get_theta();
   //   dth = r->get_dtheta();
   p = r->get_p();
   it = r->getit();
   ne = r->getNe();
   phase_space = r->get_phase_space();
   draw_trajectories = r->get_draw_trajectories();

   QGridLayout *baseLayout = new QGridLayout();
   QChartView *chartView;

   if (phase_space == 0) {
      ymin = r->get_abspmin();
      ymax = r->get_abspmax();
      //      xmin = r->get_premin();
      //      xmax = r->get_premax();
      *xmin = 0;
      xmax = &z[nz - 1];
      chartView = new QChartView(createScatterChart_trj());
      QSize min(300, 300);
      QSize max(900, 900);
      chartView->setMinimumSize(min);
      chartView->setMaximumSize(max);
   } else {
      ymin = r->get_pimmin();
      ymax = r->get_pimmax();
      xmin = r->get_premin();
      xmax = r->get_premax();
      chartView = new QChartView(createScatterChart_phs());
      QSize min(300, 300);
      QSize max(900, 900);
      chartView->setMinimumSize(min);
      chartView->setMaximumSize(max);
   }
   baseLayout->addWidget(chartView, 0, 0);
   m_charts << chartView;

   setLayout(baseLayout);
   updateUI();

   connect(r, SIGNAL(paintSignal()), this, SLOT(paintGraph()), Qt::BlockingQueuedConnection);
}

Widget::~Widget() {}

QChart *Widget::createScatterChart_trj()
{
   QChart *chart = new QChart();
   chart->setTitle("Траектории");
   chart->setTheme(QChart::ChartThemeDark);
   QFont font = chart->titleFont();
   font.setPointSize(16);
   chart->setTitleFont(font);

   for (int i = 0; i < ne; i++) {
      series.append(new QScatterSeries());
      series[i]->setUseOpenGL(true);
   }

   xAxis = new QValueAxis; // Ось X
                           //    xAxis->setRange(0, z[nz - 1]);
   xAxis->setRange(*xmin, *xmax);
   xAxis->setTitleText(tr("z")); // Название оси X
   //    xAxis->setTitleBrush(Qt::black); // Цвет названия
   //    xAxis->setLabelsColor(Qt::black); // Цвет элементов оси
   font = xAxis->titleFont();
   font.setPointSize(16);
   xAxis->setTitleFont(font);
   font = xAxis->labelsFont();
   font.setPointSize(12);
   xAxis->setLabelsFont(font);

   yAxis = new QValueAxis;          // Ось Y
   yAxis->setRange(*ymin, *ymax);   // Диапазон от -20 до +20 Вольт
   yAxis->setTitleText(tr("|p|"));  // Название оси Y
   //    yAxis->setTitleBrush(Qt::black); // Цвет названия
   //    yAxis->setLabelsColor(Qt::black); // Цвет элементов оси
   font = xAxis->titleFont();
   font.setPointSize(16);
   yAxis->setTitleFont(font);
   font = xAxis->labelsFont();
   font.setPointSize(12);
   yAxis->setLabelsFont(font);

   chart->addAxis(xAxis, Qt::AlignBottom);
   chart->addAxis(yAxis, Qt::AlignLeft);

   for (int i = 0; i < ne; ++i) {
      chart->addSeries(series[i]);
      //    chart->setAxisX(xAxis, serieskpd); // Назначить ось xAxis, осью X для diagramA
      series[i]->attachAxis(xAxis);
      //    chart->setAxisY(yAxis, serieskpd); // Назначить ось yAxis, осью Y для diagramA
      series[i]->attachAxis(yAxis);
   }

   return chart;
}

QChart *Widget::createScatterChart_phs()
{
   QChart *chart = new QChart();
   //   chart->setTitle("Фазовая плоскость");
   chart->setTheme(QChart::ChartThemeDark);
   QFont font = chart->titleFont();
   font.setPointSize(16);
   chart->setTitleFont(font);

   for (int i = 0; i < ne; i++) {
      series.append(new QScatterSeries());
      series[i]->setUseOpenGL(true);      
   }

   xAxis = new QValueAxis; // Ось X
   xAxis->setRange(*xmin, *xmax);
   xAxis->setTitleText(tr("Re(p)")); // Название оси X
   //    xAxis->setTitleBrush(Qt::black); // Цвет названия
   //    xAxis->setLabelsColor(Qt::black); // Цвет элементов оси
   font = xAxis->titleFont();
   font.setPointSize(16);
   xAxis->setTitleFont(font);
   font = xAxis->labelsFont();
   font.setPointSize(12);
   xAxis->setLabelsFont(font);

   yAxis = new QValueAxis;               // Ось Y
   yAxis->setRange(*ymin, *ymax);        // Диапазон от -20 до +20 Вольт
   yAxis->setTitleText(tr("Im(p)"));     // Название оси Y
   //    yAxis->setTitleBrush(Qt::black); // Цвет названия
   //    yAxis->setLabelsColor(Qt::black); // Цвет элементов оси
   font = xAxis->titleFont();
   font.setPointSize(16);
   yAxis->setTitleFont(font);
   font = xAxis->labelsFont();
   font.setPointSize(12);
   yAxis->setLabelsFont(font);

   chart->addAxis(xAxis, Qt::AlignBottom);
   chart->addAxis(yAxis, Qt::AlignLeft);

   for (int i = 0; i < ne; ++i) {
      chart->addSeries(series[i]);
      //    chart->setAxisX(xAxis, serieskpd); // Назначить ось xAxis, осью X для diagramA
      series[i]->attachAxis(xAxis);
      //    chart->setAxisY(yAxis, serieskpd); // Назначить ось yAxis, осью Y для diagramA
      series[i]->attachAxis(yAxis);
   }

   return chart;
}

void Widget::updateUI()
{
   const auto charts = m_charts;

   bool checked = true;
   for (QChartView *chart : charts)
      chart->setRenderHint(QPainter::Antialiasing, checked);

   for (QChartView *chartView : charts) {
      chartView->chart()->legend()->hide();
   }
}

void Widget::paintGraph()
{
   static int j;
   j = *it;
   QColor green(Qt::green);

   if (phase_space == 0) {
      yAxis->setRange((*ymin) - 0.2, (*ymax) + 0.2);

      for (int i = 0; i < ne; i++) {
         if (draw_trajectories == 0)
            series[i]->clear();
         series[i]->setBrush(green);
         series[i]->append(z[j], abs(p[i][j]));
      }
   } else {
      xAxis->setRange((*xmin) - 0.2, (*xmax) + 0.2);
      yAxis->setRange((*ymin) - 0.2, (*ymax) + 0.2);

      for (int i = 0; i < ne; i++) {
         if (draw_trajectories == 0)
            series[i]->clear();
         series[i]->setBrush(green);
         series[i]->append(real(p[i][j]), imag(p[i][j]));
      }
   }
}
