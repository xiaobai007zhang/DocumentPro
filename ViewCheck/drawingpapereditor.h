#ifndef QTDRAWINGPAPEREDITOR_H
#define QTDRAWINGPAPEREDITOR_H

#include "attribute.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>
#include <QWidget>
#define SCALE_VALUE_MAX 15.0
#define SCALE_VALUE_MIN 0.5

class QtDrawingPaperEditor : public QLabel
{
    Q_OBJECT
public:
    explicit QtDrawingPaperEditor(QWidget* parent, int width, int height);
    ~QtDrawingPaperEditor() override;
    QPixmap* getPixmap();

    void setPaintX(int);
    void setPaintY(int);
    void resizeEvent(QResizeEvent* event) override;

    void setPaperWH(int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

private:
    double CalcPaperWidthOfPerPixel(double scale_value, int paper_width, int widget_width);
    void UpdatePaperWidthOfPerPixel();
    void DrawPaper(QPainter& painter);

    int PaperWidth2DrawWidth(int paper_width);
    int DrawWidth2PaperWidth(int draw_width);
    void OnWheelValueChanged(QPoint mouse_pos, QPoint step);

    QPoint MousePoint2PaperPoint(QPoint point);
    void CentenThePaper();

private slots:
    void slot_okBtnClick(PixStatus);

private:
    int paper_x_;
    int paper_y_;
    int paper_width_;
    int paper_height_;
    QLabel label;
    double scale_value_;
    double paper_width_of_per_pixel_;

    bool is_mouse_left_btn_down_;

    QPoint mouse_down_pos_;
    int orgin_dif_x_;
    int orgin_dif_y_;

    QRect draw_paper_rect_;

    QPixmap* m_pixmap;

    PixStatus M_FLAG;
};

#endif // QTDRAWINGPAPEREDITOR_H
