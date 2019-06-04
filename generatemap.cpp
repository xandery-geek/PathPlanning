#include "generatemap.h"
#include <QTime>
#include <QMouseEvent>
#include <QRegion>
#include <QVector>
#include <QMessageBox>
#include <QColor>

GenerateMap::GenerateMap(QWidget *parent)
    :QWidget(parent), LIMIT_WIDTH_(11, 51), LIMIT_HEIGHT_(11, 31)
{
    map_matrix_ = nullptr;
    canvas_ = nullptr;

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
    road_image_.load(":/new/prefix1/image/road1.png");
    wall_image_.load(":/new/prefix1/image/wall1.png");
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

    this->setMapSize(width, height);

    //initlizlize map
    map_matrix_ = new int*[height_];
    for(int i=0; i<height_; i++)
    {
        map_matrix_[i] = new int[width_];
    }

    for(int i=0; i<height_; i++)
    {
        for(int j=0; j<width_;j++)
        {
            if(i % 2 == 0 || j % 2==0)
            {
                map_matrix_[i][j] = 1;  //wall
            }
            else
            {
                map_matrix_[i][j] = 0;  //road
            }
        }
    }

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    map_matrix_[1][1] = 2;
    this->generateMaze(1, 1);

    for(int i=0; i<height_; i++)
    {
        for(int j=0; j<width_;j++)
        {
            if(map_matrix_[i][j] == 2)
            {
                map_matrix_[i][j] = 0;
            }
        }
    }


    int count = 1;
    for(int i=0; i<20; i++)
    {
        this->pileObstacle(1, 1, D_DOWN, qrand()%PILE_COEFFICIENT_+1, &count);
    }

    this->removeObstacleRandomly(EROSING_COEFFICIENT_);
    this->obstacleClustering(); //waiting for improve
}

//depth first search to generate maze
void GenerateMap::generateMaze(int pos_i, int pos_j)
{
    //arrive boarder
    if(pos_j < 0 || pos_j >= width_ || pos_i < 0 || pos_i >= height_)
    {
        return;
    }

    QVector<int> vec = existedRoad((const int**)map_matrix_, pos_i, pos_j);

    //no existed road
    if(vec.size() == 0)
    {
        return;
    }

    //find next road randomly
    for(int i=0; i < vec.size();)
    {
        int index = qrand()%vec.size();
        switch(vec[index])
        {
        case D_LEFT:
            if(map_matrix_[pos_i][pos_j-2] != 2)
            {
                map_matrix_[pos_i][pos_j-1] = 2;
                map_matrix_[pos_i][pos_j-2] = 2;
                this->generateMaze(pos_i, pos_j-2);
            }
            break;
        case D_RIGHT:
            if(map_matrix_[pos_i][pos_j+2] != 2)
            {
                map_matrix_[pos_i][pos_j+1] = 2;
                map_matrix_[pos_i][pos_j+2] = 2;
                this->generateMaze(pos_i, pos_j+2);
            }
            break;
        case D_UP:
            if(map_matrix_[pos_i-2][pos_j] != 2)
            {
                map_matrix_[pos_i-1][pos_j] = 2;
                map_matrix_[pos_i-2][pos_j] = 2;
                this->generateMaze(pos_i-2, pos_j);
            }
            break;
        case D_DOWN:
            if(map_matrix_[pos_i+2][pos_j] != 2)
            {
                map_matrix_[pos_i+1][pos_j] = 2;
                map_matrix_[pos_i+2][pos_j] = 2;
                this->generateMaze(pos_i+2, pos_j);
            }
            break;
        }
        vec.remove(index);
    }
}

void GenerateMap::pileObstacle(int pos_i, int pos_j, int dir, int pile_coeff, int* count)
{
    //arrive boarder
    if(pos_j < 0 || pos_j >= width_ || pos_i < 0 || pos_i >= height_)
    {
        return;
    }

    //find road
    int next_i=0, next_j=0;
    QVector<int> vec;
    vec = existedRoad((const int**)map_matrix_, pos_i, pos_j, dir);

    if(vec.size() == 0)
    {
        map_matrix_[pos_i][pos_j] = 1;
        (*count)++;
        return;
    }

    for(int i=0; i<vec.size(); i++)
    {
        switch (vec[i])
        {
        case D_LEFT:
            next_i = pos_i;
            next_j = pos_j-1;
            break;
        case D_RIGHT:
            next_i = pos_i;
            next_j = pos_j+1;
            break;
        case D_UP:
            next_i = pos_i-1;
            next_j = pos_j;
            break;
        case D_DOWN:
            next_i = pos_i+1;
            next_j = pos_j;
            break;
        }

        this->pileObstacle(next_i, next_j, vec[i], pile_coeff, count);
//        if(map_matrix_[pos_i][ pos_j-1] == 1)
//        {
//            vec = existedRoad(map_matrix_, pos_i, pos_j, dir);

//            if(vec.size() == 0)
//            {
//                map_matrix_[pos_i][pos_j] = 1;
//                (*count)++;
////                if(*count >= pile_coeff)
////                {
////                    *count = 0;
////                    pile_coeff = qrand()%PILE_COEFFICIENT_+1;
////                    return;
////                }
//                return;
//            }
//        }
    }
}

void GenerateMap::removeObstacleRandomly(float erosing_coeff)
{
    int index_i = 0, index_j = 0;

    for(int count=0; count < erosing_coeff*width_*height_;)
    {
        index_i = qrand()%(height_-2) + 1;
        index_j = qrand()%(width_-2) + 1;

        //remove obstacle which is bordered by road randomly
        if(map_matrix_[index_i][index_j] == 1 &&
          (map_matrix_[index_i-1][index_j] == 0 || map_matrix_[index_i+1][index_j]==0 ||
           map_matrix_[index_i][index_j-1] == 0 || map_matrix_[index_i][index_j+1] == 0))
        {
            map_matrix_[index_i][index_j] = 0;
            count++;
        }
    }
}

void GenerateMap::obstacleClustering()
{
    int core_size = 3;

    int count=0;

    for(int i=1; i+core_size < height_; i+=core_size)
    {
        count = 0;
        for(int j=1; j+core_size <width_; j+=3)
        {
            count = getMatrixSum(map_matrix_, height_, width_, i, j, 3);
            if(count < 3)
            {
                setMatrixValue(map_matrix_, height_, width_, i, j, 3, 0);
            }
        }
    }
}

const QVector<int> GenerateMap::existedRoad(const int **mat, int i, int j)
{
    QVector<int> vec;

    if(j-2 >= 0 && mat[i][j-2] == 0)
    {
        vec.push_back(D_LEFT);
    }

    if(j+2 < width_ && mat[i][j+2] == 0)
    {
        vec.push_back(D_RIGHT);
    }

    if(i-2 >= 0 && mat[i-2][j] == 0)
    {
        vec.push_back(D_UP);
    }

    if(i+2 < height_ && mat[i+2][j] == 0)
    {
        vec.push_back(D_DOWN);
    }

    return vec;
}

const QVector<int> GenerateMap::existedRoad(const int **mat, int i, int j, int dir)
{
    QVector<int> vec;

    //left
    if(j-1 >= 0 && dir != D_RIGHT && mat[i][j-1] == 0)
    {
        vec.push_back(D_LEFT);
    }

    //right
    if(j+1 < width_ && dir != D_LEFT && mat[i][j+1] == 0)
    {
        vec.push_back(D_RIGHT);
    }

    //up
    if(i-1 > 0 && dir != D_DOWN && mat[i-1][j] == 0)
    {
        vec.push_back(D_UP);
    }

    //down
    if(i+1 > 0 && dir != D_UP && mat[i+1][j] == 0)
    {
        vec.push_back(D_DOWN);
    }

    return vec;
}

int GenerateMap::getMatrixSum(int **mat, int row, int col, int start_i, int start_j, int size)
{
    assert(start_i >= 0 && start_i + size <= row &&
           start_j >= 0 && start_j + size <= col);

    int count = 0;
    for(int i=start_i; i<start_i + size; i++)
    {
        for(int j=start_j; j<start_j + size; j++)
        {
            count += mat[i][j];
        }
    }
    return count;
}

void GenerateMap::setMatrixValue(int **mat, int row, int col, int start_i, int start_j, int size, int value)
{
    assert(start_i >= 0 && start_i + size <= row &&
           start_j >= 0 && start_j + size <= col);

    for(int i=start_i; i<start_i + size; i++)
    {
        for(int j=start_j; j<start_j + size; j++)
        {
            mat[i][j] = value;
        }
    }
}

void GenerateMap::showMap()
{
    this->resize(width_*BASE_SIZE_, height_*BASE_SIZE_);    //adjust widget size
    this->refreashMap();

    setStartPoint(-1, -1);
    setEndPoint(-1, -1);
}

void GenerateMap::showRobot(const QVector<QPoint> &points)
{

}

void GenerateMap::showPath(const Graph &graph, const QVector<QPoint> &points)
{
    QVector<Graph::Vertex> vertex;
    vertex = graph.getVertex();

    QPainter painter;
    painter.begin(canvas_);

    //draw points
    QPen pen;
    pen.setColor(QColor(0, 0, 255));    //blue
    pen.setWidth(2);
    painter.setPen(pen);

    QVector<Graph::Vertex>::const_iterator it1 = vertex.cbegin();
    for(; it1!=vertex.cend(); it1++)
    {
        painter.drawEllipse(it1->pos.x()*BASE_SIZE_, it1->pos.y()*BASE_SIZE_, 10, 10);
    }

    //draw path
    pen.setColor(QColor(0, 255, 0));    //green
    painter.setPen(pen);

    if(points.size() >= 2)
    {
        //queue, first in first out
        QVector<QPoint> points_queue;
        QVector<QPoint>::const_iterator it2 = points.cbegin();

        for(; it2 != points.cend(); it2++)
        {
            points_queue.push_back(QPoint(*it2));
        }

        QPoint start = points_queue.at(0);
        points_queue.pop_front();

        QPoint end;

        while (points_queue.size())
        {
            end = points_queue.at(0);
            points_queue.pop_front();

            painter.drawLine(QPoint(start.x()*BASE_SIZE_, start.y()*BASE_SIZE_),
                             QPoint(end.x()*BASE_SIZE_, end.y()*BASE_SIZE_));
            start = end;
        }
    }

    painter.end();
    update();
}

const int **GenerateMap::getMapMatrix() const
{
    return (const int**)map_matrix_;
}

int GenerateMap::getMapHeight() const
{
    return height_;
}

int GenerateMap::getMapWidth() const
{
    return width_;
}

void GenerateMap::showPoint()
{
    QPainter painter;

    painter.begin(canvas_);

    if(start_point_.x() != -1)
    {
        painter.drawPixmap(start_point_.x()-32, start_point_.y()-32, 64, 64, start_image_);
        update(QRect(start_point_.x(), start_point_.y(), 64, 64));
    }

    if(end_point_.x() != -1)
    {
        painter.drawPixmap(end_point_.x()-32, end_point_.y()-32, 64, 64, end_image_);
        update(QRect(end_point_.x(), end_point_.y(), 64, 64));
    }

    painter.end();
}

void GenerateMap::refreashMap()
{
    if(map_matrix_ == nullptr)
    {
        return;
    }
    QPainter painter;

    if(canvas_ != nullptr)
    {
        delete canvas_;
    }

    canvas_ = new QPixmap(width_*BASE_SIZE_, height_*BASE_SIZE_);

    painter.begin(canvas_);

    for(int i=0; i<height_; i++)
    {
        for(int j=0; j<width_; j++)
        {
            if(map_matrix_[i][j]==1)
            {
                painter.drawPixmap(j*BASE_SIZE_, i*BASE_SIZE_, BASE_SIZE_, BASE_SIZE_, wall_image_);
            }
            else
            {
                painter.drawPixmap(j*BASE_SIZE_, i*BASE_SIZE_, BASE_SIZE_, BASE_SIZE_, road_image_);
            }
        }
    }

    painter.end();
    update();
}

void GenerateMap::paintEvent(QPaintEvent *event)
{
    if(canvas_ == nullptr)
    {
        return;
    }

    painter_.begin(this);
    painter_.drawPixmap(0, 0, *canvas_);
    painter_.end();
}

void GenerateMap::mousePressEvent(QMouseEvent *event)
{
    if(canvas_ == nullptr)
    {
        return;
    }

    if(event->button() == Qt::RightButton)
    {
        if(map_matrix_[event->pos().y()/BASE_SIZE_][event->pos().x()/BASE_SIZE_] == 1)
        {
            QMessageBox::about(this, "Error", "起点和终点不能设置在障碍上!");
            return;
        }

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

        this->refreashMap();
        this->showPoint();
        emit startEndChanged((const QPoint)start_point_, (const QPoint)end_point_);
    }
}

void GenerateMap::setStartPoint(int x, int y)
{
    start_point_.setX(x);
    start_point_.setY(y);
}

const QPoint GenerateMap::getStartPoint() const
{
    return QPoint(start_point_.x()/BASE_SIZE_, start_point_.y()/BASE_SIZE_);
}

void GenerateMap::setEndPoint(int x, int y)
{
    end_point_.setX(x);
    end_point_.setY(y);
}

const QPoint GenerateMap::getEndPoint() const
{
    return QPoint(end_point_.x()/BASE_SIZE_, end_point_.y()/BASE_SIZE_);
}

void GenerateMap::setMapSize(int width, int height)
{
    width_ = width;
    height_ = height;

    if(width_ < LIMIT_WIDTH_.x())
    {
        width_ = LIMIT_WIDTH_.x();
    }
    else if(width_ > LIMIT_WIDTH_.y())
    {
        width_ = LIMIT_WIDTH_.y();
    }

    if(height_ < LIMIT_HEIGHT_.x())
    {
        height_ = LIMIT_HEIGHT_.x();
    }
    else if(height_ > LIMIT_HEIGHT_.y())
    {
        height_ = LIMIT_HEIGHT_.y();
    }
}
