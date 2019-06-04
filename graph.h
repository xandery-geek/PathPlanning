#ifndef GRAPH_H
#define GRAPH_H

#include <QPoint>
#include <QVector>

class Graph
{
public:
    class Arc
    {
    public:
        int vertex_index;   //vertex
        float distance;     //the length of arc
        Arc *next_arc;      //next arc

        Arc()
        {
            Arc(0);
        }

        Arc(int vertex)
        {
            vertex_index = vertex;
            distance = 0.0f;
            next_arc = nullptr;
        }

        ~Arc()
        {
            Arc *temp;
            while(next_arc)
            {
                temp = next_arc;
                next_arc = next_arc->next_arc;
                delete temp;
            }
        }
    };

    class Vertex
    {
    public:
        QPoint pos;     //the position on the map
        Arc *head_arc;  //the first arc

        Vertex()
        {
            Vertex(QPoint(0, 0));
        }

        Vertex(const QPoint& pos)
        {
            this->pos.setX(pos.x());
            this->pos.setY(pos.y());
            head_arc = nullptr;
        }

        ~Vertex()
        {
            if(head_arc != nullptr)
            {
                delete head_arc;
            }
        }
    };

private:
    QVector<Vertex> graph_;

public:
    Graph();
    ~Graph();

    void addVertex(const QVector<QPoint>& points);
    void addVertex(const QPoint& point);
    const QVector<Vertex>& getVertex() const;
    const Vertex& getVertex(int index) const;
    void addArc(int start, int end);
    void addArc(int start, const QVector<int> &end_points);
    const Graph::Arc getArc(int vertex, int index) const;

    void destroyGraph();

private:
    float getDistance(const QPoint& point1, const QPoint& point2);
};

#endif // GRAPH_H
