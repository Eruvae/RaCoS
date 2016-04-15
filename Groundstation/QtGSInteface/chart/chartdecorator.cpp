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

#include "chart/chartdecorator.h" 
 
using namespace chart;  

   void ChartDecorator::paint (QPainter & painter, Chart * chart)
   {
     if (m_component) 
       m_component->paint(painter,chart);
   }
   

   void ChartDecorator::absPosition(QPoint & pos, QPolygonF & absPoints, Chart * chart,QRect & clip)
   {
      if (m_component) 
        m_component->absPosition(pos,absPoints,chart,clip);
   }

    ChartDecorator::ChartDecorator (ChartDecorator * component):m_component(component)
    {}

   ChartDecorator::~ChartDecorator()
   {
    if (m_component) 
       delete m_component;
    m_component=0;
   }
