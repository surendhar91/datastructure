#ifndef BINARYTREE_H_
#define	BINARYTREE_H_
struct bnode
{
    int data;
    struct bnode* left;
    struct bnode* right;
};
struct bnode* newBNode(int data);
#endif