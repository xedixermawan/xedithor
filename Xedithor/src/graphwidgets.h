#ifndef GRAPHWIDGETS_H
#define GRAPHWIDGETS_H

#include <QtGui/QGraphicsView>
#include <qtableview.h>
#include "moduletablemodel.h"

#include "xscene.h"
#include "rectselectionitem.h"
#include "hlinegraphicsitem.h"
#include "rowdata.h"

QT_BEGIN_NAMESPACE
    class QLabel;
QT_END_NAMESPACE

namespace ModeView
{
    const int EDIT_MODE = 0;
    const int DRAG_MODE = 1;
};


const int WidthRectView  = 2000;
const int HeightRectView = 2000;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    RowData* rowDataSelected;
    QTableView* m_table;
    int posMouseX;
    int posMouseY;
    void setImageGraphicsItem(QPixmap* pxmap);
    QPixmap* getImageGraphicsItem();
    void setModeView(const int mode);
    GraphWidget(QWidget *parent = 0);
    QGraphicsRectItem* getRectSelectItem()
    {
        return rectSelect;
    }

public slots:
    void setLabelMouse(QLabel* labelmouseinv);

protected:
    //Holds the current centerpoint for the view, used for panning and zooming
    QPointF CurrentCenterPoint;

    //From panning the view
    QPoint LastPanPoint;
    QPoint FirstRectPoint;
    QPoint LastRectPoint;

    //Set the current centerpoint in the
    void SetCenter(const QPointF& centerPoint);
    QPointF GetCenter() { return CurrentCenterPoint; }

    //Take over the interaction
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);

    virtual void drawForeground(QPainter* painter,const QRectF &rect);

private:
    int timerId;
    int modeView;
    QPixmap* pixmap;
    QLabel* labelmouse;
    QRect* rectPointer;
    QGraphicsPixmapItem* pixmapGraphicsItem;
    RectSelectionItem* rectSelect;

    HLineGraphicsItem* lineH;
    XScene *m_scene;

};


#endif // GRAPHWIDGETS_H
