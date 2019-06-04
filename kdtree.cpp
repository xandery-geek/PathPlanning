#include "kdtree.h"
#include <algorithm>

KdTree::KdTree()
{
    KdTree(1);
}

KdTree::KdTree(int k)
{
    this->k_ = k;
    this->root_ = nullptr;
}

KdTree::~KdTree()
{
    destroyTree(root_);
}

void KdTree::initKdTree(const QVector<QPoint> &points)
{
    QVector<KdNode*> nodes;

    //initialize node vector
    QVector<QPoint>::const_iterator it = points.cbegin();
    int index = 0;
    for(; it!=points.cend(); it++)
    {
        KdNode *temp = new KdNode;
        temp->pos.setX(it->x());
        temp->pos.setY(it->y());
        temp->index = index;
        temp->left = nullptr;
        temp->right = nullptr;
        nodes.push_back(temp);
    }

    //construct kd tree
    int axis = X_Axis;  //division axis
    constructKdTree(nodes, &root_, 0, nodes.size(), axis);
}

void KdTree::setKValue(int k)
{
    this->k_ = k;
}

void KdTree::addNode(const KdTree::KdNode *parent, const KdTree::KdNode *child)
{

}

void KdTree::addNode(const KdTree::KdNode *parent, const QPoint &pos)
{

}

void KdTree::deleteNode(const KdTree::KdNode *node)
{

}

void KdTree::deleteNode(const QPoint &pos)
{

}

const KdTree::KdNode *KdTree::getNode(const KdTree::KdNode *node) const
{

}

const KdTree::KdNode *KdTree::getNode(const QPoint &pos) const
{

}

QVector<int> KdTree::getKNN(const QPoint &point)
{
    QVector<int> neightbor;

    return neightbor;
}

void KdTree::constructKdTree(QVector<KdTree::KdNode *> nodes,
                             KdNode** root, int left, int right, int axis)
{
    if(left >= right)
    {
        root = nullptr;
        return;
    }

    //sort, get the mid elem
    int mid = (left+right)>>1;

    if(axis == X_Axis)
    {
        std::sort(nodes.begin()+left, nodes.begin()+right,
                  [](const KdNode* node1, const KdNode* node2)->bool
        {return node1->pos.x() < node2->pos.x(); });
        axis = Y_Axis;
    }
    else
    {
        std::sort(nodes.begin()+left, nodes.begin()+right,
                  [](const KdNode* node1, const KdNode* node2)->bool
        {return node1->pos.y() < node2->pos.y(); } );

        axis = X_Axis;
    }

    *root = nodes[mid];

    constructKdTree(nodes, &((*root)->left), left, mid, axis);
    constructKdTree(nodes, &((*root)->right), mid+1, right, axis);
}

void KdTree::destroyTree(KdTree::KdNode *node)
{
    if(node == nullptr)
    {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
    node = nullptr;
}

bool KdTree::XAxisCompare(const KdTree::KdNode *node1, const KdTree::KdNode *node2)
{
    return node1->pos.x() < node2->pos.x();
}

bool KdTree::YAxisCompare(const KdTree::KdNode *node1, const KdTree::KdNode *node2)
{
    return node1->pos.y() < node2->pos.y();
}


