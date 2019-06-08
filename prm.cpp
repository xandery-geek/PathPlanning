#include "prm.h"
#include <QTime>
#include <cmath>

PRM::PRM()
    :start_(-1, -1), end_(-1, -1)
{
    kd_tree_ = nullptr;
}

PRM::~PRM()
{
    //release kd tree
    if(kd_tree_ != nullptr)
    {
        delete kd_tree_;
    }

    prm_graph_.destroyGraph();
}

void PRM::constructGraph(const int **mat, int row, int col)
{
    assert(start_.x() != -1 && end_.x() != -1);

    //clear graph
    prm_graph_.destroyGraph();

    //select points randomly
    graph_mat_row_ = row;
    graph_mat_col_ = col;

    int n = VERTEX_COEFFICIENT * row * col;
    vertex_k_ = K_COEFFICIENT * row * col;

    QVector<QPoint> points;

    points.push_back(start_);   //add start point
    points.push_back(end_);     //add end point

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    int i=0, j=0;
    while(n)
    {
        i = qrand()%row;
        j = qrand()%col;

        if(mat[i][j] == 0)
        {
            points.push_back(QPoint(j, i));
            n--;
        }
    }

    prm_graph_.addVertex(points);   //set vertex table of graph

    this->generateArc(mat, points);      //generate arc of graph
}

void PRM::generateArc(const int** mat, const QVector<QPoint>& points)
{
    if(prm_graph_.getVertex().size() == 0)
    {
        return;
    }

    //KNN

    //construct Kd tree
    if(kd_tree_ != nullptr)
    {
        delete kd_tree_;
    }

    kd_tree_ = new KdTree(vertex_k_);
    kd_tree_->initKdTree(points);

    QVector<int> neighbors;

    //search the K nearest neighbors
    QVector<QPoint>::const_iterator it = points.begin();
    int index = 0;
    float dis, oil;

    QPoint mat_index1;
    QPoint mat_index2;

    for(; it !=points.end(); it++)
    {
        neighbors.clear();
        neighbors = kd_tree_->getKNN(*it);

        for(int i=0; i<neighbors.size(); i++)
        {
            mat_index1.setX(points[index].y()); // change to mat index, i=y
            mat_index1.setY(points[index].x()); // change to mat index, j=x

            mat_index2.setX(points[neighbors[i]].y()); // change to mat index, i=y
            mat_index2.setY(points[neighbors[i]].x()); // change to mat index, j=x

            if(checkPath(mat, mat_index1, mat_index2))
            {
                dis = getDistance(mat_index1, mat_index2);
                oil = getOil(mat, mat_index1, mat_index2);

                prm_graph_.addArc(index, neighbors[i], dis, oil);     //add to arc table
                prm_graph_.addArc(neighbors[i], index, dis, oil);     //add to arc table
            }
        }
        index++;
    }
}

void PRM::setStartPoint(const QPoint &point)
{
    start_.setX(point.x());
    start_.setY(point.y());
}

void PRM::setEndPoint(const QPoint &point)
{
    end_.setX(point.x());
    end_.setY(point.y());
}

void PRM::searchPath(bool option)
{
    AStar(option);
}

const QVector<QPoint> &PRM::getPath() const
{
    return path_;
}

const Graph &PRM::getGraph() const
{
    return prm_graph_;
}

void PRM::AStar(bool option)
{
    //clear previous path
    path_.clear();
    path_.push_back(start_);

    //oil first
    if(option)
    {

    }
    else
    {

    }

    path_.push_back(end_);
}

bool PRM::checkPath(const int **mat, const QPoint &point1, const QPoint &point2)
{
    float num = 5 * std::max(abs(point1.x() - point2.x()), abs(point1.y() - point2.y()));

    //the same path
    if(num == 0)
    {
        return false;
    }

    float x_step = (float)((point2.x() - point1.x())/num);
    float y_step = (float)((point2.y() - point1.y())/num);

    QVector<QPoint> vec;

    for(int i=0; i<=num; i++)
    {
        vec.push_back(QPoint(point1.x() + i*x_step, point1.y()+ i*y_step));
    }

    QVector<QPoint>::const_iterator it = vec.cbegin();
    for(; it != vec.cend(); it++)
    {
        //if(isCrash(mat, graph_mat_row_, graph_mat_col_, *it))
        if(mat[(int)it->x()][(int)it->y()] == 1)
        {
            return false;
        }
    }

    return true;
}

float PRM::getDistance(const QPoint &point1, const QPoint &point2)
{
    return sqrt(pow(point1.x() - point2.x(), 2) + pow(point1.y() - point2.y(), 2));
}

float PRM::getOil(const int **mat, const QPoint &point1, const QPoint &point2)
{
    float oil = 0.0f;

    int num = std::max(abs(point1.x() - point2.x()), abs(point1.y() - point2.y()));

    if(num == 0)
    {
        return 0.0f;
    }

    float x_step = (float)((point2.x() - point1.x())/num);
    float y_step = (float)((point2.y() - point1.y())/num);

    QVector<QPoint> vec;

    for(int i=0; i<=num; i++)
    {
        vec.push_back(QPoint(point1.x() + i*x_step, point1.y()+ i*y_step));
    }

    QVector<QPoint>::const_iterator it = vec.cbegin();
    for(; it != vec.cend(); it++)
    {
        if(mat[(int)it->x()][(int)it->y()] == 2)
        {
            oil += SAND_WEIGHT;
        }
        else if(mat[(int)it->x()][(int)it->y()] == 0)
        {
            oil += ROAD_WEIGHT;
        }
    }

    return oil;
}

bool PRM::isCrash(const int **mat, int row, int col, const QPoint &point)
{
    for(int i= point.x() - 1; i <= point.x() + 1; i++)
    {
        for(int j= point.y() - 1; j <= point.y() + 1; j++)
        {
            if(i >= 0 && i < row && j >= 0 && j <col)
            {
              if(mat[i][j] == 1)
              {
                  return true;
              }
            }
        }
    }

    return false;
}
