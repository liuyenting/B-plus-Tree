#ifndef __B_PLUS_TREE_NODE_H
#define __B_PLUS_TREE_NODE_H

#include <vector>

template<class TKey>
class BpTreeNode
{
    enum class TreeNodeType
    {
        InnerNode,
        LeafNode
    };

private:
    int keyCount;
    std::vector<TKey> keys;
    
    BpTreeNode<TKey> parentNode;
    BpTreeNode<TKey> leftSibling;
    BpTreeNode<TKey> rightSibling;

public:
    BpTreeNode();
    ~BpTreeNode();

    int getKeyCound();
    bool isOverflow();
    bool isUnderflow();
    bool canLendAKey();
    
    TKey getKey(int);
    void setKey(int, const TKey&);
    
    BpTreeNode<TKey>& getParent();
    void setParent(const BpTreeNode<TKey>&);

    BpTreeNode<TKey>& geLeftSibling();
    void setLeftSibling(const BpTreeNode<TKey>&);

    BpTreeNode<TKey>& getRightSibling();
    void setRightSibling(const BpTreeNode<TKey>&);

    BpTreeNode<TKey>& dealOverflow();
    BpTreeNode<TKey>& dealUnderflow();

    TreeNodeType getNodeType();
    int search(const TKey&);
};

#endif