/***************************************************************************
 *   Copyright (C) 2006-2008 by Tomasz Ziobrowski                          *
 *   http://www.3electrons.com                                             *
 *   e-mail: t.ziobrowski@3electrons.com                                   *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/************************************************************************
  			scalesgriddecorator.h - Copyright coder
**************************************************************************/

#ifndef SCALESGRIDDECORATOR_H
#define SCALESGRIDDECORATOR_H

#include "GUIWidgets/chart/chartdecorator.h"



namespace chart {
namespace Standard {
/**
 * Class ScalesGridDecorator
 */
class ScalesGridDecorator : public ChartDecorator {

public:

  ScalesGridDecorator (ChartDecorator * component );

  void paint (QPainter & painter, Chart * chart);

  void absPosition(QPoint & pos, QPolygonF & absPoints, Chart * chart,QRect & clip);
   
protected:

  void paintBackground(QPainter &painter, Chart * chart);
  void paintXScale(QPainter & painter, Chart * chart);
  void paintYScale(QPainter & painter, Chart * chart);
  void paintXGrid  (QPainter & painter, Chart * chart);
  void paintYGrid  (QPainter & painter, Chart * chart);

 //double YScaleHeight();
 private:
  double xScaleHeight;
  double yScaleHeight;
  double yScaleWidth;
  
  double m_min; 
  double m_max; 
  double scaleSize; 
  bool   m_channelsScale ;
//   
};
} // chart
} // Standard
#endif //SCALESGRIDDECORATOR_H

