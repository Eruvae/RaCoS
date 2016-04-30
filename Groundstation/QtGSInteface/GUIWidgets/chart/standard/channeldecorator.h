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
  			channeldecorator.h - Copyright coder
**************************************************************************/

#ifndef CHANNELDECORATOR_H
#define CHANNELDECORATOR_H

#include "GUIWidgets/chart/chartdecorator.h"



namespace chart {
namespace Standard {




class ChannelDecorator :  public ChartDecorator 
{
  
public:

  ChannelDecorator (ChartDecorator * component ):ChartDecorator(component){}

   void paint (QPainter & painter, Chart * chart);
   

 void absPosition(QPoint & pos, QPolygonF & absPoints, Chart * chart,QRect & clip);
  
private:

   void translateToChannel (QPainter & painter, Chart * chart, Channel & channel);

   void paintChannel(QPainter & painter,Chart * chart, Channel & channel);

   double xfactor;
   double yfactor;
   double dx,dy;

   double dxw; 

   QRect window; 

   QRect viewport; 

};// class ChannelDecorator
} // chart
} // Standard
#endif //CHANNELDECORATOR_H

