#ifndef CLICKABLEVIEW_H
#define CLICKABLEVIEW_H

#include <QGraphicsView>


class ClickableView : public QGraphicsView
{
    Q_OBJECT
public:
    ClickableView(QWidget *parent);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void mouseClicked(int row, int col);
};

#endif // CLICKABLEVIEW_H
