#include "graph.h"
#include <qmath.h>

Graph::Graph()
{

}

Graph::~Graph()
{

}

void Graph::addVertex(const QVector<QPoint> &points)
{
    QVector<QPoint>::const_iterator it = points.cbegin();
    for(; it!=points.cend(); it++)
    {
        graph_.push_back(Vertex(*it));
    }
}

void Graph::addVertex(const QPoint& point)
{
    graph_.push_back(point);
}

const QVector<Graph::Vertex> &Graph::getVertex() const
{
    return graph_;
}

const Graph::Vertex &Graph::getVertex(int index) const
{
    assert(index >=0 && index < graph_.size());

    return graph_[index];
}

void Graph::addArc(int start, int end)
{
    Graph::Arc *arc = graph_[start].head_arc;
    while(arc)
    {
        arc = arc->next_arc;
    }

    arc = new Graph::Arc(end);
    arc->distance = getDistance(graph_[start].pos, graph_[end].pos);
    arc->next_arc = nullptr;
}

void Graph::addArc(int start, const QVector<int>& end_points)
{

    Graph::Arc *arc = graph_[start].head_arc;
    while(arc)
    {
        arc = arc->next_arc;
    }

    for(int i=0; i<end_points.size(); i++)
    {
        arc = new Graph::Arc(end_points[i]);
        arc->distance = getDistance(graph_[start].pos, graph_[end_points[i]].pos);
        arc->next_arc = nullptr;

        arc = arc->next_arc;
    }
}

/*
* @retutn:
*/
const Graph::Arc Graph::getArc(int vertex, int index) const
{
    assert(vertex >=0 && vertex < graph_.size());

    int i=0;

    Graph::Arc *arc = graph_[vertex].head_arc;
    while(arc->next_arc && i<index)
    {
        arc = arc->next_arc;
        i++;
    }

    return (*arc);
}

void Graph::destroyGraph()
{
    graph_.clear();
}

float Graph::getDistance(const QPoint &point1, const QPoint &point2)
{
    return sqrt( pow(point1.x() - point2.x(), 2) + pow(point2.y() - point2.y(), 2));
}
