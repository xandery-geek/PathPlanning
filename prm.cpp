#include "prm.h"
#include <QTime>

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
}

void PRM::constructGraph(const int **mat, int row, int col)
{
    assert(start_.x() != -1 && end_.x() != -1);

    //clear graph
    prm_graph_.destroyGraph();

    //select points randomly
    int n = VERTEX_COEFFICIENT * row *col;

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

    this->generateArc(points);      //generate arc of graph
}

void PRM::generateArc(const QVector<QPoint>& points)
{
    if(prm_graph_.getVertex().size() == 0)
    {
        return;
    }

    //KNN search the K nearest neighbors

    //construct Kd tree
    if(kd_tree_ != nullptr)
    {
        delete kd_tree_;
    }

    kd_tree_ = new KdTree(VERTEX_K);
    kd_tree_->initKdTree(points);

    QVector<int> nerghbors;

    //search the K nearest neighbors
    QVector<QPoint>::const_iterator it = points.begin();
    int i = 0;
    for(; it !=points.end(); it++)
    {
        nerghbors.clear();
        nerghbors = kd_tree_->getKNN(*it);
        prm_graph_.addArc(i, nerghbors);    //add to arc table
        i++;
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

void PRM::searchPath()
{
    //clear previous path
    path_.clear();

    path_.push_back(start_);
    path_.push_back(end_);
}

const QVector<QPoint> &PRM::getPath() const
{
    return path_;
}

const Graph &PRM::getGraph() const
{
    return prm_graph_;
}


