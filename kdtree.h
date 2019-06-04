#ifndef KDTREE_H
#define KDTREE_H

#include <QPoint>
#include <QVector>

//division axis

class KdTree
{
private:

    enum Axis
    {
       X_Axis,
       Y_Axis
    };

    class KdNode
    {
    public:
        QPoint pos;
        int index;  //index in vector
        int division_axis;

        KdNode *left;
        KdNode *right;
    };

    int k_;
    KdNode *root_;

public:
    KdTree();
    KdTree(int k);

    ~KdTree();

    void initKdTree(const QVector<QPoint>& points);
    void setKValue(int k);
    void addNode(const KdNode* parent, const KdNode* child);
    void addNode(const KdNode* parent, const QPoint& pos);
    void deleteNode(const KdNode* node);
    void deleteNode(const QPoint& pos);
    const KdNode* getNode(const KdNode* node) const;
    const KdNode* getNode(const QPoint& pos) const;

    QVector<int> getKNN(const QPoint& point);   //get K nearest neightbors

private:
    void constructKdTree(QVector<KdNode*> nodes, KdNode **root, int left, int right, int axis);
    void destroyTree(KdNode *node);

    static bool XAxisCompare(const KdNode* node1, const KdNode* node2);
    static bool YAxisCompare(const KdNode* node1, const KdNode* node2);
};

#endif // KDTREE_H


