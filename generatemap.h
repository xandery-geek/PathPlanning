#ifndef GENERATEMAP_H
#define GENERATEMAP_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPoint>

class GenerateMap: public QWidget
{
    Q_OBJECT

private:

    enum PaintEnum
    {
        NotPaint,
        PaintMap=1,
        PaintPoint=2,
        PaintPath=3,
    };

    const int BASE_SIZE_ = 10;

    int **map_matrix_;
    int width_;
    int height_;
    QPoint start_point_;
    QPoint end_point_;

    int paint_flag_;
    QPainter painter_;

    QPixmap canvas_;        //canvas

    //resource images
    QPixmap road_image_;
    QPixmap wall_image_;
    QPixmap start_image_;
    QPixmap end_image_;

public:
    GenerateMap(QWidget* parent=nullptr);
    ~GenerateMap();

    void loadImage();   //load resource image

    void creatMap(int width, int height);    //creat image and save to map_matrix_
    void showMap();     //show generated map in the widget
    void showPoint();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void setStartPoint(int x, int y);
    void setEndPoint(int x, int y);

};

#endif // GENERATEMAP_H
