#ifndef AVLMULTIVARIABLE_H
#define AVLMULTIVARIABLE_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"
#include "Exception.h"

/*
* Class MultiTree : Tree
* This class is used to create a tree based on ComplexNode that is sorted by three keys:
*       the movie's rating, the number of views it received, and its movie ID
*/
template <class T>
class MultiTree : public Tree<ComplexNode<T>, T> {
public:

    /*
    * Constructor of MultiTree class
    * @param - none
    * @return - A new instance of MultiTree
    */
    MultiTree();

    /*
    * Copy Constructor and Assignment Operator of MultiTree class
    * streaming does not allow two of the same user or movie (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    */
    MultiTree(const MultiTree& other) = delete;
    MultiTree& operator=(const MultiTree& other) = delete;

    /*
    * Destructor of MultiTree class
    * @param - none
    * @return - void
    */
    ~MultiTree() = default;

    /*
     * Insert new node with data, according to the id, views, and rating given
     * @param - New data to insert and the ID, views, and rating of the new node
     * @return - void
     */
    void insert(T data, const int id, const int views, const double rating);

    /*
     * Remove node according to the id, views, and rating given
     * @param - The ID, views, and rating of the node that needs to be removed
     * @return - void
     */
    void remove(const int id, const int views, const double rating);

    /*
     * Search for max node by going down the right side of the tree
     * @param - none
     * @return - the data of the max node
     */
    ComplexNode<T>* search_and_return_max();
    
    /*
     * Search for a specific node, according to the id, views, and rating given
     * @param - The ID, views, and rating of the requested node
     * @return - the data of the node
     */
    T& search_and_return_data(const int id, const int views, const double rating);

    /*
     * Search for node with a specific id, according to the id, views, and rating given
     * @param - The ID, views, and rating of the requested node
     * @return - the requested node
     */
    ComplexNode<T>& search_specific_id(const int id, const int views, const double rating);

    /*
     * Recursively search the tree, according to the id, views, and rating given, starting with the root
     * @param - The ID, views, and rating of the requested node, and the root of its tree
     * @return - the requested node
     */
    ComplexNode<T>& search_recursively(const int id, const int views, const double rating, ComplexNode<T>* currentNode);

    /*
     * Helper function for get_all_movies in streaming:
     * Recursively inserts the movie ID's of the data of the tree into a given array
     * @param - an array
     * @return - void
     */
    void get_all_data(int* const array) const;

    /*
     * Gets the number of nodes in the tree
     * @param - none
     * @return - int, the number of nodes in the tree
     */
    int get_num_nodes() const;

    /*
     * Gets the largest node in the tree
     * @param - none
     * @return - int, the largest node's id
     */
    int get_max() const;

private:
    /*
     * Internal fields for MultiTree:
     *    The number of nodes in the tree
     *    The largest node in the tree
    */
    int m_numOfNodes;
    ComplexNode<T>* m_max;
};


//-----------------------------------------Constructor--------------------------

template<class T>
MultiTree<T>::MultiTree() :
    Tree<ComplexNode<T>, T>(),
    m_numOfNodes(0),
    m_max(nullptr)
{}

//----------------------------------Insert and Remove---------------------------------

template<class T>
void MultiTree<T>::insert(T data, const int id, const int views, const double rating) 
{
    m_numOfNodes++;
    //If this is the first node in the tree:
    if (this->m_node->m_height == -1) {
        this->m_node->m_data = data;
        this->m_node->m_id = id;
        this->m_node->m_height++;
        this->m_node->m_rating = rating;
        this->m_node->m_views = views;
        m_max = this->m_node;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    ComplexNode<T>* x = this->m_node;
    ComplexNode<T>* parent = nullptr;
    while (x != nullptr) {
        parent = x;
        if (x->m_id == id) {
            //A node with that id already exists - invalid operation
            m_numOfNodes--;
            throw InvalidID(); 
        }
        if (rating < x->m_rating) {
            x = x->m_left;
        }
        else if (rating == x->m_rating) {
            if (views < x->m_views) {
                x = x->m_left;
            }
            else if (views == x->m_views) {
                if (id > x->m_id) {
                    x = x->m_left;
                }
                else {
                    x = x->m_right;
                }
            }
            else {
                x = x->m_right;
            }
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    ComplexNode<T>* node = nullptr;
    try{
        node = new ComplexNode<T>();
    }
    catch(const std::bad_alloc& e)
    {
        m_numOfNodes--;
        delete node;
        throw e;
    }
    node->m_parent = parent;
    node->m_left = nullptr;
    node->m_right = nullptr;
    node->m_data = data;
    node->m_rating = rating;
    node->m_views = views;
    node->m_id = id;
    node->m_height = 0;
    if (node->m_rating < parent->m_rating) {
        parent->m_left = node;
    }
    else if (node->m_rating == parent->m_rating) {
        if (node->m_views < parent->m_views) {
            parent->m_left = node;
        }
        else if (node->m_views == parent->m_views) {
            if (node->m_id > parent->m_id) {
                parent->m_left = node;
            }
            else {
                parent->m_right = node;
            }
        }
        else {
            parent->m_right = node;
        }
    }
    else {
        parent->m_right = node;
    }
    this->rebalance_tree(node->m_parent);
    m_max = search_and_return_max();
}


template<class T>
void MultiTree<T>::remove(const int id, const int views, const double rating) 
{
    if (this->m_node->m_id == id && this->m_node->m_right == nullptr && this->m_node->m_left == nullptr 
                                                                                && this->m_node->m_parent == nullptr) {
        this->m_node->m_data = nullptr;
        this->m_node->m_height = -1;
        this->m_node->m_bf = 0;
        this->m_node->m_id = 0;
        this->m_node->m_rating = 0;
        this->m_node->m_views = 0;
        m_numOfNodes--;
        return;
    }
    ComplexNode<T>* toRemove = &(search_specific_id(id, views, rating));
    ComplexNode<T>* nodeToFix = Tree<ComplexNode<T>, T>::make_node_leaf(toRemove);
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    Tree<ComplexNode<T>, T>::rebalance_tree(nodeToFix);
    m_numOfNodes--;
    m_max = search_and_return_max();
}


//-----------------------------------------Search Functions-----------------------------------------

template<class T>
ComplexNode<T>* MultiTree<T>::search_and_return_max() 
{
    ComplexNode<T>* node = this->m_node;
    while(node->m_right != nullptr) {
        node = node->m_right;
    }
    return node;
}


template<class T>
T& MultiTree<T>::search_and_return_data(const int id, const int views, const double rating) 
{
    return search_recursively(id, views, rating, this->m_node).m_data;
}


template<class T>
ComplexNode<T>& MultiTree<T>::search_specific_id(const int id, const int views, const double rating) 
{
    return search_recursively(id, views, rating, this->m_node);
}


template<class T>
ComplexNode<T>& MultiTree<T>::search_recursively(const int id, const int views, const double rating,
             ComplexNode<T>* currentNode) 
{
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return *currentNode;
    }
    if (currentNode->m_rating < rating) {
        return search_recursively(id, views, rating, currentNode->m_right);
    }
    if (currentNode->m_views < views && currentNode->m_rating == rating) {
        return search_recursively(id, views, rating, currentNode->m_right);
    }
    if (currentNode->m_id > id && currentNode->m_rating == rating && currentNode->m_views == views) {
        return search_recursively(id, views, rating, currentNode->m_right);
    }
    return search_recursively(id, views, rating, currentNode->m_left);
}


//-----------------------------------------Helper Functions for streaming-----------------------------------------

template <class T>
void MultiTree<T>::get_all_data(int* const array) const
{
    if (m_numOfNodes <= 0) {
        throw NodeNotFound();
    }
    if (this != nullptr) {
        this->m_node->get_data_inorder(array, 0);
    }
}


template<class T>
int MultiTree<T>::get_num_nodes() const 
{
    return m_numOfNodes;
}


template <class T>
int MultiTree<T>::get_max() const
{
    if (m_numOfNodes <= 0) {
        return -1;
    }
    return m_max->getID();
}

//----------------------------------------------------------------------------------------------

#endif //AVLMULTIVARIABLE_H