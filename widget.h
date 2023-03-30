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

#ifndef WIDGET_H
#define WIDGET_H

#include "rkn.h"

#include <QChartView>
#include <QScatterSeries>
#include <QValueAxis>

#if QT_VERSION < 0x060000
QT_CHARTS_USE_NAMESPACE
#endif

class Widget : public QWidget
{
   Q_OBJECT

private:
   double *z, // вектор координат z
       *t,    // вектор координат t
       *a,    // текущая амплитуда
       *kpd,  // текущий КПД
       **th, **dth, *ymin, *ymax, xmin_val, *xmin = &xmin_val, *xmax;
   complex<double> **p;
   int nz,                                      // число точек по z
       nt,                                      // число точек по времени
       ne, *it, phase_space, draw_trajectories; // текущая точка времени
   QValueAxis *xAxis;                           // Ось X
   QValueAxis *yAxis;                           // Ось Y

public:
   explicit Widget(Rkn *, QWidget *parent = 0);
   ~Widget();

private slots:
   void updateUI();

private:
   void connectSignals();
   QChart *createScatterChart_trj();
   QChart *createScatterChart_phs();

private:
   int m_listCount;
   int m_valueMax;
   int m_valueCount;
   QScatterSeries *seriesA = nullptr;
   QList<QScatterSeries *> series;
   QList<QChartView *> m_charts;
   Rkn *r;

public slots:
   void paintGraph();
};

#endif /* WIDGET_H */
