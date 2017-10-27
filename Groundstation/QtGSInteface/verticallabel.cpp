#include "verticallabel.h"

#include <QPainter>

VerticalLabel::VerticalLabel(QWidget *parent)
    : QLabel(parent)
{
}

VerticalLabel::VerticalLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
}

void VerticalLabel::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.setPen(QPen(QColor(200, 200, 255)));

    painter.translate( sizeHint().width(), sizeHint().height() );
    painter.rotate(270);

    //QLabel::paintEvent (e);
    painter.drawText(0,0, text());
}

QSize VerticalLabel::minimumSizeHint() const
{
    QSize s = QLabel::minimumSizeHint();
    return QSize(s.height(), s.width());
}

QSize VerticalLabel::sizeHint() const
{
    QSize s = QLabel::sizeHint();
    return QSize(s.height(), s.width());
}
