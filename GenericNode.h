#ifndef GENERICNODE_H
#define GENERICNODE_H

#include "Node.h"
#include <iostream>

/*
* Class Tree : Generic Node
* This class is used to create the separate nodes in a basic AVL tree.
*/
template <class T>
class GenericNode : Node<T> {
public:

    /*
    * Constructor of GenericNode class
    * @param - none
    * @return - A new instance of GenericNode
    */ 
    GenericNode();

    /*
    * Copy Constructor and Assignment Operator of GenericNode class
    * streaming does not allow two of the same user, movie, or group (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    */
    GenericNode(const GenericNode&) = delete;
    GenericNode& operator=(const GenericNode& other) = delete;

    /*
    * Destructor of GenericNode class
    * @param - none
    * @return - void
    */
    ~GenericNode() = default;

    /*
    * Returns the height of a node
    * @param - none
    * @return - height
    */
    int get_height() const;

private:

    /*
     * Left-Right Rotation
     * @param - Node with balance factor of +2
     * @return - pointer to ComplexNode
     */
    GenericNode* ll_rotation(GenericNode* node);

    /*
     * Right-Right Rotation
     * @param - Node with balance factor of -2
     * @return - pointer to ComplexNode
     */
    GenericNode* rr_rotation(GenericNode* node);

    /*
     * Right-Left Rotation
     * @param - Node with balance factor of -2
     * @return - pointer to ComplexNode
    */
    GenericNode* rl_rotation(GenericNode* node);

    /*
     * Left-Left Rotation
     * @param - Node with balance factor of +2
     * @return - pointer to ComplexNode
    */
    GenericNode* lr_rotation(GenericNode* node);

    /*
     * Update balance factor of the current node
     * @param - none
     * @return - void
    */
    void update_bf();
    
    /*
     * Update height of the current node
     * @param - none
     * @return - void
    */
    void update_height();

    /*
     * Helper function for remove_group in streaming:
     * Removes each user from the group
     * @param - none
     * @return - void
    */
    void inorder_remove();

    /*
     * The following class is a friend class in order to allow full access to private fields and functions of
     * GenericNode, allowing GenericNode to be a mostly private class, while allowing the system to run smoothly.
    */
    template <class N, class M>
    friend class Tree;

    /*
     * The internal fields of GenericNode: pointers to the parent node and two child nodes
     */
    GenericNode* m_parent;
    GenericNode* m_left;
    GenericNode* m_right;

};

//--------------------------------------------Constructor---------------------------------------------------

template <class T>
GenericNode<T>::GenericNode() :
        Node<T>(),
        m_parent(nullptr),
        m_left(nullptr),
        m_right(nullptr)
{}

//--------------------------------Public Helper Function for streaming--------------------------------------------

template<class T>
int GenericNode<T>::get_height() const {
    return this->m_height;
}

//--------------------------------------------Rotations---------------------------------------------------

//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::ll_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmpToReturn = node;
    //Changing A->B to A->Parent
    m_left->m_parent = m_parent;
    //Changing Parent->B to Parent->A
    if (m_parent != nullptr) {
        if (m_parent->m_left == this) {
            m_parent->m_left = m_left;
        }
        else {
            m_parent->m_right = m_left;
        }
    }
    else {
        tmpToReturn = m_left;
    }
    //Changing B->Parent to B->A
    m_parent = m_left;
    //Changing Ar->A to Ar->B
    if (m_left->m_right != nullptr) {
        m_left->m_right->m_parent = this;
    }
    //Changing B->A to B->Ar
    m_left = m_left->m_right;
    //Changing A->Ar to A->B
    m_parent->m_right = this;
    return tmpToReturn;
}


//Right-Right tree rotation, on the node with balance factor of -2
template <class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::rr_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmpToReturn = node;
    m_right->m_parent = m_parent;
    if (m_parent != nullptr) {
        if (m_parent->m_right == this) {
            m_parent->m_right = m_right;
        }
        else {
            m_parent->m_left = m_right;
        }
    }
    else {
        tmpToReturn = m_right;
    }
    m_parent = m_right;
    if(m_right->m_left != nullptr) {
        m_right->m_left->m_parent = this;
    }
    m_right = m_right->m_left;
    m_parent->m_left = this;
    return tmpToReturn;
}


//Right-Left tree rotation, on the node with balance factor of -2
template <class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::rl_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::lr_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}


//----------------------------------------------Node Stats---------------------------------------------------

//Update the balance factor of the specific node
template <class T>
void GenericNode<T>::update_bf()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    this->m_bf = heightLeft - heightRight;
}


//Update the subtree height of the specific node
template <class T>
void GenericNode<T>::update_height()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    if (heightLeft >= heightRight) {
        this->m_height = heightLeft;
    }
    else {
        this->m_height = heightRight;
    }
}

//------------------------------------Helper Function for streaming-------------------------------------------------

template <class T>
void GenericNode<T>::inorder_remove() {
    if (this != nullptr && this->m_data != nullptr) {
        m_left->inorder_remove();
        this->m_data->update_views();
        this->m_data->remove_group();
        m_right->inorder_remove();
    }
}

//-----------------------------------------------------------------------------------------------------------

#endif //GENERICNODE_H