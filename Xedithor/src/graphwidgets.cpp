


#include <QtGui>
#include <QPainter>
#include <math.h>
#include <QGraphicsPixmapItem>

#include "graphwidgets.h"
#include "rectselectionitem.h"


#include <iostream>


GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    m_scene = new XScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    setScene(m_scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    posMouseX = 0;
    posMouseY = 0;
    pixmapGraphicsItem=NULL;

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Set-up the scene
    //QGraphicsScene* Scene = new QGraphicsScene(this);
    //setScene(m_scene);

    // X-axis component
    m_scene->addLine(0,HeightRectView / 2,WidthRectView, HeightRectView / 2);
    for(int x = 0; x < WidthRectView; x = x + 25) {
        if(x % 100 == 0 ) {
            QString pointString;
            QTextStream stream(&pointString);
            stream << x - WidthRectView/2;
            QGraphicsTextItem* item = m_scene->addText(pointString);
            item->setPos(x, HeightRectView / 2);
        }
        m_scene->addRect(x, HeightRectView / 2, 1, 1);
    }
    // Y-axis component
    m_scene->addLine(WidthRectView / 2,0,WidthRectView / 2, HeightRectView);
    for(int y = 0; y < HeightRectView; y = y + 25) {
        if(y % 100 == 0 ) {
            QString pointString;
            QTextStream stream(&pointString);
            stream << y - HeightRectView/2;
            QGraphicsTextItem* item = m_scene->addText(pointString);
            item->setPos(WidthRectView / 2,y);
        }
        m_scene->addRect(WidthRectView / 2, y, 1, 1);
    }

    //Set-up the view
    setSceneRect(0, 0, WidthRectView, HeightRectView);
    SetCenter(QPointF(WidthRectView / 2, HeightRectView / 2)); //A modified version of centerOn(), handles special cases
    setCursor(Qt::OpenHandCursor);

    //rect selection
    rectSelect = new RectSelectionItem;
    rectSelect->setRect(1000.0,1000.0,400.0,400.0);

    QPen pen;
    pen.setBrush(QBrush(Qt::NoBrush));
    pen.setColor(QColor(Qt::red));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    rectSelect->setPen(pen);
    rectSelect->setZValue(0.9);
    m_scene->addItem(rectSelect);

    //lineH = new HLineGraphicsItem;//(QPointF(100+WidthRectView / 2, 100+HeightRectView / 2),QPointF(200+WidthRectView / 2, 10+HeightRectView / 2));
    //lineH->setSourcePoint(QPointF(10+WidthRectView / 2, 10+HeightRectView / 2));
    //lineH->setDestPoint(QPointF(20+WidthRectView / 2, 10+HeightRectView / 2));

    //lineH->setFlag(QGraphicsItem::ItemIsMovable, true);
    //lineH->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //m_scene->addItem(lineH);

    modeView = ModeView::DRAG_MODE;

    rowDataSelected = NULL;

}

void GraphWidget::setImageGraphicsItem(QPixmap* pxmap)
{
    delete pixmapGraphicsItem;

    pixmap=pxmap;
    pixmapGraphicsItem= new QGraphicsPixmapItem(*pixmap);
    pixmapGraphicsItem->setPos(WidthRectView / 2 , HeightRectView / 2 );
    this->scene()->addItem(pixmapGraphicsItem);
}

QPixmap* GraphWidget::getImageGraphicsItem()
{
    return &pixmapGraphicsItem->pixmap();
}

void GraphWidget::setLabelMouse(QLabel* labelmouseinv)
{
    labelmouse=labelmouseinv;
}


/*
  * Sets the current centerpoint.  Also updates the scene's center point.
  * Unlike centerOn, which has no way of getting the floating point center
  * back, SetCenter() stores the center point.  It also handles the special
  * sidebar case.  This function will claim the centerPoint to sceneRec ie.
  * the centerPoint must be within the sceneRec.
  */
//Set the current centerpoint in the
void GraphWidget::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            CurrentCenterPoint = sceneBounds.center();
        } else {

            CurrentCenterPoint = centerPoint;

            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                CurrentCenterPoint.setX(bounds.x());
            }

            if(centerPoint.y() > bounds.y() + bounds.height()) {
                CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                CurrentCenterPoint.setY(bounds.y());
            }

        }
    }

    //Update the scrollbars
    centerOn(CurrentCenterPoint);
}

// Handles when the mouse button is pressed

void GraphWidget::mousePressEvent(QMouseEvent* event) {

    if(event->button() == Qt::RightButton){
        if(this->cursor().shape() == Qt::ArrowCursor){
           // setCursor(Qt::ClosedHandCursor);
        }else {
           // setCursor(Qt::ArrowCursor);
        }
    }
    else{

    }

    //For panning the view
    LastPanPoint = event->pos();
    FirstRectPoint= event->pos();

    QPointF coordMouse = mapToScene(FirstRectPoint.x(),FirstRectPoint.y());
    //std::cout<<" X: "<<coordMouse.x()<<std::endl;
    //std::cout<<" Y: "<<coordMouse.y()<<std::endl;
    //rectSelect->setRect(coordMouse.x(),coordMouse.y(),coordMouse.x(),coordMouse.x());
    //rectSelect->update();
    //std::cout<<"press mouse"<<std::endl;

    QGraphicsView::mousePressEvent(event);
}

// Handles when the mouse button is released

void GraphWidget::mouseReleaseEvent(QMouseEvent* event) {
    // setCursor(Qt::OpenHandCursor);
    LastPanPoint = QPoint();

    LastRectPoint= event->pos();

    QGraphicsView::mouseReleaseEvent(event);
}


//Handles the mouse move event

void GraphWidget::mouseMoveEvent(QMouseEvent* event) {
    if(modeView == ModeView::DRAG_MODE && m_scene->selectedItems().count()==0){
        if(!LastPanPoint.isNull()) {
            //Get how much we panned
            QPointF delta = mapToScene(LastPanPoint) - mapToScene(event->pos());
            LastPanPoint = event->pos();

            //Update the center ie. do the pan
            SetCenter(GetCenter() + delta);
        }
    }

    posMouseX=event->pos().x();
    posMouseY=event->pos().y();

    LastRectPoint= event->pos();

    QPointF coordMouse = mapToScene(posMouseX,posMouseY);

    QString xyStr = QString::number((coordMouse.x())-(WidthRectView/2));
    xyStr.append(", ");
    xyStr.append(QString::number((coordMouse.y())-(HeightRectView/2)));

    labelmouse->setText(xyStr);
    coordMouse = mapToScene(LastRectPoint.x(),LastRectPoint.y());



    ModuleTableModel* m =static_cast<ModuleTableModel*>(m_table->model());
    int rowSelected_ = m_table->currentIndex().row();

    if(rowSelected_!=-1)
    {

        std::cout<<"selected: "<<rowSelected_<<std::endl;
        RowData* rd = m->getDatainRow(rowSelected_);
        QString sh = QString::number(rectSelect->rect().width());
        QString sw = QString::number(rectSelect->rect().height());
        QString sx = QString::number(rectSelect->pos().x()+rectSelect->rect().x()-1000);
        QString sy = QString::number(rectSelect->pos().y()+rectSelect->rect().y()-1000);



        rd->setData(2,sx);
        rd->setData(3,sy);
        rd->setData(4,sh);
        rd->setData(5,sw);


        std::cout<<"XX "<<rectSelect->pos().x()<<std::endl;
        std::cout<<"YY "<<rectSelect->pos().y()<<std::endl;
        m->refresh();
   }

    QGraphicsView::mouseMoveEvent(event);
}


/*
  * Zoom the view in and out.
  */
void GraphWidget::wheelEvent(QWheelEvent* event) {

    //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));

    //Get the original screen centerpoint
    QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

    //Scale the view ie. do the zoom
    double scaleFactor = 1.15; //How fast we zoom
    if(event->delta() > 0) {
        //Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        //Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    //Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));

    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;

    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    SetCenter(newCenter);
}

/*
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  */
void GraphWidget::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    SetCenter(visibleArea.center());

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}


void GraphWidget::drawForeground(QPainter* painter,const QRectF &rect) {

    /*
    QPointF coordMouse = mapToScene(posMouseX,posMouseY);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
    painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter->drawRect(coordMouse.x(), coordMouse.y(), 400, 240);
    */


}

void GraphWidget::setModeView(const int mode) {
    modeView = mode;

    switch(modeView)
    {
    case ModeView::DRAG_MODE :  setCursor(Qt::SizeFDiagCursor);
                                break;
    case ModeView::EDIT_MODE :  setCursor(Qt::CrossCursor);
                                break;
    }
}

void GraphWidget::dragMoveEvent(QDragMoveEvent* event)
{
    std::cout<<"Drag event "<<std::endl;
}















