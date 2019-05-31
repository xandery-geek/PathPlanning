#include "generatemap.h"
#include <QTime>
#include <QMouseEvent>
#include <QRegion>

GenerateMap::GenerateMap(QWidget *parent)
    :QWidget(parent)
{
    paint_flag_ = NotPaint;
    map_matrix_ = nullptr;

    loadImage();
}

GenerateMap::~GenerateMap()
{
    //release map_matrix_
    for(int i=0; i<height_; i++)
    {
        delete [] map_matrix_[i];
    }

    delete [] map_matrix_;
}

void GenerateMap::loadImage()
{
    road_image_.load(":/new/prefix1/image/road.png");
    wall_image_.load(":/new/prefix1/image/wall.png");
    start_image_.load(":/new/prefix1/image/start.png");
    end_image_.load(":/new/prefix1/image/end.png");
}

void GenerateMap::creatMap(int width, int height)
{
    //release map_matrix_
    if(map_matrix_ != nullptr)
    {
        for(int i=0; i<height_; i++)
        {
            delete [] map_matrix_[i];
        }

        delete [] map_matrix_;
    }

    width_ = width;     //save width and height
    height_ = height;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    map_matrix_ = new int*[height];
    for(int i=0; i<height; i++)
    {
        map_matrix_[i] = new int[width];
        for(int j=0; j<width; j++)
        {
            map_matrix_[i][j] = qrand()%2;
        }
    }

    paint_flag_ = PaintMap;
}

void GenerateMap::showMap()
{
    if(map_matrix_ == nullptr)
    {
        return;
    }

    this->resize(width_*BASE_SIZE_, height_*BASE_SIZE_); //resize the map widget

    painter_.begin(this);

    for(int i=0; i<height_; i++)
    {
        for(int j=0; j<width_; j++)
        {
            if(map_matrix_[i][j])
            {
                painter_.drawPixmap(j*BASE_SIZE_, i*BASE_SIZE_, BASE_SIZE_, BASE_SIZE_, wall_image_);
            }
            else
            {
                painter_.drawPixmap(j*BASE_SIZE_, i*BASE_SIZE_, BASE_SIZE_, BASE_SIZE_, road_image_);
            }
        }
    }

    painter_.end();

    setStartPoint(-1, -1);
    setEndPoint(-1, -1);
}

void GenerateMap::showPoint()
{
    painter_.begin(this);

    if(start_point_.x() != -1)
    {
        painter_.drawPixmap(start_point_.x(), start_point_.y(), 64, 64, start_image_);
        update(QRect(start_point_.x(), start_point_.y(), 64, 64));
    }

    if(end_point_.x() != -1)
    {
        painter_.drawPixmap(end_point_.x(), end_point_.y(), 64, 64, end_image_);
        update(QRect(end_point_.x(), end_point_.y(), 64, 64));
    }

    painter_.end();
}

void GenerateMap::paintEvent(QPaintEvent *event)
{
    switch (paint_flag_)
    {
    case PaintMap:
        this->showMap();
        break;
    case PaintPoint:
        this->showPoint();
        break;
    case PaintPath:
        break;
    default:
        break;
    }
}

void GenerateMap::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        if(start_point_.x() == -1)
        {
            setStartPoint(event->pos().x(), event->pos().y());
        }
        else if(end_point_.x() == -1)
        {
            setEndPoint(event->pos().x(), event->pos().y());
        }
        else
        {
            setStartPoint(end_point_.x(), end_point_.y());
            setEndPoint(event->pos().x(), event->pos().y());
        }
    }

    paint_flag_ = PaintPoint;

}

void GenerateMap::setStartPoint(int x, int y)
{
    start_point_.setX(x);
    start_point_.setY(y);
}

void GenerateMap::setEndPoint(int x, int y)
{
    end_point_.setX(x);
    end_point_.setY(y);
}
