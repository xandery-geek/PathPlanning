#ifndef PRM_H
#define PRM_H

#include <QVector>
#include <QPoint>
#include "graph.h"
#include "kdtree.h"

class PRM
{
private:

    const float VERTEX_COEFFICIENT = 0.05;
    const int VERTEX_K = 3;

    QPoint start_;
    QPoint end_;

    QVector<QPoint> path_;

    Graph prm_graph_;
    KdTree *kd_tree_;

public:
    PRM();
    ~PRM();

    void constructGraph(const int** mat, int row, int col);
    void generateArc(const QVector<QPoint>& points); //KNN
    void setStartPoint(const QPoint& point);
    void setEndPoint(const QPoint& point);
    void searchPath();
    const QVector<QPoint>& getPath() const;
    const Graph& getGraph() const;
};

#endif // PRM_H
