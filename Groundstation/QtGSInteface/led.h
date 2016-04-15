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

#ifndef QLED_H
#define QLED_H
#include <QColor> 
#include "widgetwithbackground.h"


   class Led : public WidgetWithBackground
   {
     Q_OBJECT 
     Q_PROPERTY( bool checked READ isChecked 	WRITE setChecked)
     Q_PROPERTY( QColor color READ color 	WRITE setColor) 
     
     
     public:  
     
     Led(QWidget *parent = 0);
     virtual ~Led() {}
 
     void paintEvent(QPaintEvent * event);
     
     bool isChecked () const ; 
     
     
     QColor color() const; 
     void setColor(QColor); 
     
     public slots: 
     
     void setChecked(bool); 
     
     signals: 
     
     void checkChanged(bool val); 
     
     protected:
     
     void initCoordinateSystem(QPainter & painter);
     
     void paintBackground(QPainter & painter);
    
     protected: 
     
     bool m_checked; 
     QColor m_color; 

   }; 
   
#endif // QLED_H 
