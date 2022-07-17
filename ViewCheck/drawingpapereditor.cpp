/*
 * @Name       QtDrawingPaperEditor
 * @Author    Robert Zhang
 * @E-mail    zhangzhiguo1207@163.com
 * @Date       2019-09-17
 */
#include <QDebug>
#include "drawingpapereditor.h"
#include <QPainter>
#include <QFileDialog>


QtDrawingPaperEditor::QtDrawingPaperEditor(QWidget *parent,int width,int height)
    : QLabel(parent)
{

    paper_x_ = 0;
    paper_y_ = 0;
    paper_width_ = width;
    paper_height_ = height;

    scale_value_ = 1;

    is_mouse_left_btn_down_ = false;

    pixmap = new QPixmap;

    UpdatePaperWidthOfPerPixel();

}

QtDrawingPaperEditor::~QtDrawingPaperEditor()
{

}

QPixmap* QtDrawingPaperEditor::getPixmap()
{
    return pixmap;
}

void QtDrawingPaperEditor::setPaintX(int x)
{
    paper_x_ = x;
}

void QtDrawingPaperEditor::setPaintY(int y)
{
    paper_y_ = y;
}

void QtDrawingPaperEditor::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event)
    QPainter painter(this);

    DrawPaper(painter);

}

void QtDrawingPaperEditor::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    if(event->buttons() & Qt::LeftButton) {

        is_mouse_left_btn_down_ = true;
        mouse_down_pos_ = event->pos();
        //求出当前光标在画布中的位置
        

        orgin_dif_x_ = mouse_down_pos_.x() - paper_x_;
        orgin_dif_y_ = mouse_down_pos_.y() - paper_y_;
        
    }
}

void QtDrawingPaperEditor::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    if(is_mouse_left_btn_down_ == true) {

        paper_x_ = event->pos().x() - orgin_dif_x_;
        paper_y_ = event->pos().y() - orgin_dif_y_;

        update();
        
    }

}

void QtDrawingPaperEditor::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)

    is_mouse_left_btn_down_ = false;
    qDebug() << "paper_x_: " << paper_x_ << ",paper_y_: " << paper_y_;
    
}

void QtDrawingPaperEditor::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    
    //重新计算画布和窗口的比例
    UpdatePaperWidthOfPerPixel();

    CentenThePaper();
}

void QtDrawingPaperEditor::setPaperWH(int width,int height)
{
    paper_width_ = width;
    paper_height_ = height;
}

void QtDrawingPaperEditor::wheelEvent(QWheelEvent *event) {
    Q_UNUSED(event)

    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    //qDebug() << numPixels;
    //qDebug() << numDegrees;
    if (!numPixels.isNull()) {
        OnWheelValueChanged(event->pos(),numPixels);
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        OnWheelValueChanged(event->pos(), numSteps);
    }
    event->accept();
}

//scale_value : 0.5~ 20
//paper_width :
//widget_width :
double QtDrawingPaperEditor::CalcPaperWidthOfPerPixel(double scale_value,int paper_width,int widget_width) {
    int paper_width_of_final_show = paper_width ;//+ static_cast<int>(static_cast<double>(paper_width )* 0.2);
    int scaled_widget_width = qRound(static_cast<double>(widget_width) * scale_value);

    double paper_width_of_per_pixel = static_cast<double>(paper_width_of_final_show) / static_cast<double>(scaled_widget_width);
    //求出画布和当前窗口大小的比例
    //limit readable per-pixel value
    if(paper_width_of_per_pixel < 0.0005) paper_width_of_per_pixel = 0.0005;
    //qDebug() << "scale_value" << scale_value;
    //qDebug() << "widget_width" << widget_width;
    //qDebug() << "paper_width" << paper_width;
    //qDebug() << "scaled_widget_width" << scaled_widget_width;
    //qDebug()<<"paper_width_of_per_pixel" << paper_width_of_per_pixel;
    //qDebug() << "";
    
    return paper_width_of_per_pixel;
}

//re-calc  "paper_width_of_per_pixel" only when : init, resize, scale_value changed
void QtDrawingPaperEditor::UpdatePaperWidthOfPerPixel() {
    if(this->height() >= this->width()) {
        //该比例 方便 画布到窗口之间的转换
        paper_width_of_per_pixel_ = CalcPaperWidthOfPerPixel(scale_value_, paper_width_, this->width());
    } else {
        paper_width_of_per_pixel_ = CalcPaperWidthOfPerPixel(scale_value_, paper_width_, this->height());
    }
    //qDebug() << "比率:" << paper_width_of_per_pixel_;
}

int QtDrawingPaperEditor::PaperWidth2DrawWidth(int paper_width) {
    //求出画布上的图形多大才能在窗口上显示，求出当前图片可显示的宽度或高度
    double draw_width = static_cast<double>(paper_width) / paper_width_of_per_pixel_;
    return qRound(draw_width);
}

int QtDrawingPaperEditor::DrawWidth2PaperWidth(int draw_width) {
    double paper_width = static_cast<double>(draw_width) * paper_width_of_per_pixel_;
    return static_cast<int>(qRound(paper_width));
}

void QtDrawingPaperEditor::OnWheelValueChanged(QPoint mouse_pos, QPoint step) {

    //if mouse point in paper
    if(draw_paper_rect_.contains(mouse_pos)) {
        //将光标转换成画布中的位置
        QPoint before_resize_mouse_point_at_paper =  MousePoint2PaperPoint(mouse_pos);
        //qDebug() << "before_resize_mouse_point_at_paper: " << before_resize_mouse_point_at_paper;
        
        //将光标转换成 画布在 窗体中的位置
        int temp_paper_point_x = DrawWidth2PaperWidth(before_resize_mouse_point_at_paper.x());
        int temp_paper_point_y = DrawWidth2PaperWidth(before_resize_mouse_point_at_paper.y());

        //resize
        int step_value =  step.y();
        scale_value_ += static_cast<double>(step_value) /20.0;
        if(scale_value_ > SCALE_VALUE_MAX) scale_value_ = SCALE_VALUE_MAX;
        if(scale_value_ < SCALE_VALUE_MIN) scale_value_ = SCALE_VALUE_MIN;

        UpdatePaperWidthOfPerPixel();
        //qDebug() << "value: " << scale_value_;
        int temp_draw_point_x = PaperWidth2DrawWidth(temp_paper_point_x);
        int temp_draw_point_y = PaperWidth2DrawWidth(temp_paper_point_y);
        QPoint after_resize_mouse_point_at_paper(temp_draw_point_x, temp_draw_point_y);
        //qDebug() << "after_resize_mouse_point_at_paper: " << after_resize_mouse_point_at_paper;
        
        QPoint should_move_length = after_resize_mouse_point_at_paper - before_resize_mouse_point_at_paper;

        paper_x_ -= should_move_length.x();
        paper_y_ -= should_move_length.y();

        update();

    } else { //else using center resize
        //先获得图片的宽高
        int old_width = draw_paper_rect_.width();
        int old_height = draw_paper_rect_.height();

        //resize
        int step_value =  step.y();
        scale_value_ += static_cast<double>(step_value) /15.0;
        if(scale_value_ > SCALE_VALUE_MAX) scale_value_ = SCALE_VALUE_MAX;
        if(scale_value_ < SCALE_VALUE_MIN) scale_value_ = SCALE_VALUE_MIN;
        UpdatePaperWidthOfPerPixel();

        //求出当前窗体的大小
        int new_width = PaperWidth2DrawWidth(paper_width_);
        int new_height = PaperWidth2DrawWidth(paper_height_);

        int adjusted_height = new_height - old_height;
        int adjusted_width = new_width - old_width;

        paper_x_ -= adjusted_width/2;
        paper_y_ -= adjusted_height/2;

        update();
    }
}

void QtDrawingPaperEditor::DrawPaper(QPainter &painter) {

    /*int draw_width = PaperWidth2DrawWidth(paper_width_);
    int draw_height = PaperWidth2DrawWidth(paper_height_);*/
    int draw_height = PaperWidth2DrawWidth(paper_width_);
    int draw_width = PaperWidth2DrawWidth(paper_width_);
    //*pixmap = pixmap->scaled(draw_width, draw_height, Qt::KeepAspectRatio);
    //qDebug()<<"cur pix size:" << draw_width << "," << draw_height;
    draw_paper_rect_.setX(paper_x_);
    draw_paper_rect_.setY(paper_y_);
    draw_paper_rect_.setWidth(draw_width);
    draw_paper_rect_.setHeight(draw_height);
    painter.drawPixmap(draw_paper_rect_, *pixmap);

}

QPoint QtDrawingPaperEditor::MousePoint2PaperPoint(QPoint point) {
    QPoint ret;
    ret.setX(point.x() - paper_x_);
    ret.setY(point.y() - paper_y_);
    return ret;
}
//只要大小改变了，会重新调整位置和大小
void QtDrawingPaperEditor::CentenThePaper() {

    scale_value_ = 1;
    //UpdatePaperWidthOfPerPixel();
    int adjust_distance_x = (this->width() - PaperWidth2DrawWidth(paper_width_)) / 2;
    int adjust_distance_y = (this->height() - PaperWidth2DrawWidth(paper_height_)) / 2;
    //qDebug() << this->size()<<"," <<paper_width_ << "," << paper_height_ <<"pix size: " << pixmap->size();

    //qDebug() << adjust_distance_x<< "," << adjust_distance_y;
    paper_x_ = adjust_distance_x;
    paper_y_ = adjust_distance_y;
    update();
}


