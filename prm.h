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
    const float K_COEFFICIENT = 0.02;
    const float ROAD_WEIGHT = 1;
    const float SAND_WEIGHT = 2;

    QPoint start_;
    QPoint end_;

    QVector<QPoint> path_;

    Graph prm_graph_;
    KdTree *kd_tree_;
    int vertex_k_;

    int graph_mat_row_;
    int graph_mat_col_;

public:
    PRM();
    ~PRM();

    void constructGraph(const int** mat, int row, int col);
    void generateArc(const int **mat, const QVector<QPoint>& points); //KNN
    void setStartPoint(const QPoint& point);
    void setEndPoint(const QPoint& point);
    void searchPath(bool option);
    const QVector<QPoint>& getPath() const;
    const Graph& getGraph() const;

private:
    void AStar(bool option);
    bool checkPath(const int** mat, const QPoint& point1, const QPoint& point2);
    float getDistance(const QPoint& point1, const QPoint& point2);
    float getOil(const int** mat, const QPoint& point1, const QPoint& point2);
    bool isCrash(const int** mat, int row, int col, const QPoint& point);
};

#endif // PRM_H
