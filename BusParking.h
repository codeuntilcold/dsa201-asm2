#ifndef _BUS_PARK_H_
#define _BUS_PARK_H_

// #include "D:\1_UNIVERSITY\HK201\4_DataStructure_n_Algorithms\ASM\ASM_2\BKUTree.h"
#include <vector>
#include <algorithm>

/*
 *      REQUIREMENT 1:
 *          Implement a function that find the minimum
 *          parking slot required
 *          Complexity:     O(N logN)
 */

class Interval 
{
public:
    int start;
    int end;
    
    Interval(int start = 0, int end = 0) {
        this->start = start;
        this->end = end;
    }
};
int minPark(vector<Interval> lInterval) 
{
    // Split into arrival time and departure time
    // Then sort the two (sorting using Quicksort - NlogN)
    // Then record the highest parking slot required
    vector<int> ls, lt;
    for (Interval i : lInterval) {
        ls.push_back(i.start);
        lt.push_back(i.end);
    }
    sort(ls.begin(), ls.end());
    sort(lt.begin(), lt.end());

    int maxOcp = 0;
    int curOcp = 0;
    vector<int>::iterator is, it;
    is = ls.begin();
    it = lt.begin();
    for (int time = ls.front(); time < lt.back(); time++) {
        while (*is == time) {
            curOcp++;
            is++;
        }
        while (*it == time) {
            curOcp--;
            it++;
        }
        maxOcp = (maxOcp > curOcp) ? maxOcp : curOcp;
    }
    return maxOcp;
}

/*
 *      REQUIREMENT 2:
 *          Implement a class that stores intervals
 *          (recommend using balanced tree)
 *          add() and remove():     O(logN)
 *          minPark():              O(1)
 */

template <class K, class V>
class AVLTree 
{
public:
    class Entry
    {
    public:
        K           key;
        V           value;
        int         multi;

        Entry(K key, V value, int multi = 1) 
            : key(key), value(value), multi(multi)
        {}
    };

    class Node 
    {
    public:
        Entry*                          entry;
        Node*                           left;
        Node*                           right;
        int                             balance;
    
        Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) 
            : entry(entry), left(left), right(right), balance(0)
        {}
        ~Node() {}
    };

    friend class BusParking;
protected:
    void    rotLeft(Node* & root);
    void    rotRight(Node* & root);
    void    rotRL(Node* & root);
    void    rotLR(Node* & root);        
    bool    balanceLeft(Node* & root);
    bool    balanceRight(Node* & root);
    bool    add(Node* & root, Entry* entry);
    bool    remove(Node* & root, K key);

    void    traverseNLR(Node* & root, void (*func)(K key, V value));
    void    clear(Node* & root);

public:
    Node*           root;
    Node*           corr;

    AVLTree() : root(NULL) {}
    ~AVLTree() { this->clear(); }
    
    void    add(K key, V value, int multi=1);
    void    add(Entry* entry);
    void    remove(K key);
    V       search(K key);
    void    traverseNLR(void (*func)(K key, V value));
    void    clear();

    void    setCorr(Node* corr) { this->corr = corr; }
};

/********************************
 *                              *
 *      AVL TREE METHODS        *
 *                              *
 ********************************/

template <class K, class V>
void    AVLTree<K, V>::add(K key, V value, int multi)
{
    add(new Entry(key, value, multi));
}
template <class K, class V>
void    AVLTree<K, V>::add(Entry* entry)
{
    add(root, entry);
}
template <class K, class V>
void    AVLTree<K, V>::remove(K key)
{
    Node* p = root;
    while (p != nullptr) {
        if (p->entry->key < key) {
            p = p->right;
        }
        else if (key < p->entry->key) {
            p = p->left;
        }
        else {
            remove(root, key);
            return;
        }
    }
}
template <class K, class V>
V       AVLTree<K, V>::search(K key)
{
    Node* p = root;
    while (p != nullptr) {
        if (p->entry->key < key) {
            p = p->right;
        }
        else if (key < p->entry->key) {
            p = p->left;
        }
        else {
            return p->entry->value;
        }
    }
}
template <class K, class V>
void    AVLTree<K, V>::traverseNLR(void (*func)(K key, V value))
{
    traverseNLR(root, func);
}
template <class K, class V>
void    AVLTree<K, V>::clear()
{
    clear(this->root);
}



template <class K, class V> 
void    AVLTree<K, V>::rotLeft(Node* & root) 
{
    Node* p = root;
    root = root->right;
    p->right = root->left;
    root->left = p;
}
template <class K, class V> 
void    AVLTree<K, V>::rotRight(Node* & root) 
{
    Node* p = root;
    root = root->left;
    p->left = root->right;
    root->right = p;       
}
template <class K, class V> 
void    AVLTree<K, V>::rotRL(Node* & root) 
{
    rotRight(root->right);
    rotLeft(root);
}
template <class K, class V> 
void    AVLTree<K, V>::rotLR(Node* & root)
{
    rotLeft(root->left);
    rotRight(root);
}
template <class K, class V> 
bool    AVLTree<K, V>::balanceLeft(Node* & root) 
{
    if (root->balance == 0) {
        root->balance = -1;
        return true;
    }
    else if (root->balance == 1) {
        root->balance = 0;
        return false;
    }
    else {
        if (root->left->balance == 0) {
            rotRight(root);
            root->right->balance = -1;
            root->balance = 1;
            return true;
        }
        else if (root->left->balance == -1) {
            rotRight(root);
            root->right->balance = 0;
            root->balance = 0;
            return false;
        }
        else {
            rotLR(root);
            if (root->balance == -1) {
                root->balance = 0;
                root->left->balance = 0;
                root->right->balance = 1;
            }
            else if (root->balance == 0) {
                root->left->balance = 0;
                root->right->balance = 0;
            }
            else {
                root->balance = 0;
                root->left->balance = -1;
                root->right->balance = 0;
            }
            return false;
        }
    }
}
template <class K, class V> 
bool    AVLTree<K, V>::balanceRight(Node* & root) 
{
    if (root->balance == 0) {
        root->balance = 1;
        return true;
    }
    else if (root->balance == -1) {
        root->balance = 0;
        return false;
    }
    else {
        if (root->right->balance == 0) {
            rotLeft(root);
            root->left->balance = 1;
            root->balance = -1;
            return true;
        }
        else if (root->right->balance == 1) {
            rotLeft(root);
            root->left->balance = 0;
            root->balance = 0;
            return false;
        }
        else {
            rotRL(root);
            if (root->balance == 1) {
                root->balance = 0;
                root->right->balance = 0;
                root->left->balance = -1;
            }
            else if (root->balance == 0) {
                root->left->balance = 0;
                root->right->balance = 0;
            }
            else {
                root->balance = 0;
                root->right->balance = 1;
                root->left->balance = 0;
            }
            return false;
        }
    }
}
template <class K, class V> 
bool    AVLTree<K, V>::add(Node* & root, Entry* entry) 
{
    if (root == nullptr) {
        root = new Node(entry);
        return true;
    }
    if (entry->key < root->entry->key) {
        return add(root->left, entry) ? balanceLeft(root) : false;
    }
    else if (root->entry->key < entry->key) {
        return add(root->right, entry) ? balanceRight(root) : false;
    }
    else {
        root->entry->multi++;               ////////////////////////////////////////////////
        return false;
    }
    return false;
}
template <class K, class V>
bool    AVLTree<K, V>::remove(Node* & root, K key)
{
    if (root == nullptr) return false;

    if (key < root->entry->key) 
        return remove(root->left, key) ? !balanceRight(root) : false;
    else if (root->entry->key < key)
        return remove(root->right, key) ? !balanceLeft(root) : false;
    else
    {
        if (root->entry->multi == 1)
        {
            if (root->left == nullptr) {
                Node* p = root;
                root = root->right;
                delete p;
            }
            else if (root->right == nullptr) {
                Node* p = root;
                root = root->left;
                delete p;
            }
            else
            {
                // Find successor
                Node* succ = root->left;
                while (succ->right != nullptr) {
                    succ = succ->right;
                }
                // Stealing entry
                Entry* pE = root->entry;
                if (succ->entry != nullptr) {
                    root->entry = succ->entry;            
                    succ->entry = pE;
                }
                return remove(root->left, succ->entry->key) ? !balanceRight(root) : false;
            }
            return true;
        }
        else {
            root->entry->multi--;       ///////////////////////////////////////////////////////////
            return false;
        }
    }
}

template <class K, class V>
void    AVLTree<K, V>::traverseNLR(Node* & root, void (*func)(K key, V value))
{
    if (root != nullptr) {
        func(root->entry->key, root->entry->value);
        traverseNLR(root->left, func);
        traverseNLR(root->right, func);
    }
}
template <class K, class V>
void    AVLTree<K, V>::clear(Node* & root)
{
    if (root != nullptr) {
        clear(root->left);
        clear(root->right);
        Node* p = root;
        if (root->entry != nullptr) { 
            delete root->entry;
            root->entry = nullptr;
        }
        root = nullptr;
        delete p;
    }
}

class BusParking {
protected:
    AVLTree<int, int>* ltree;
    AVLTree<int, int>* rtree;

public:
    BusParking() {
        ltree = new AVLTree<int, int>();
        rtree = new AVLTree<int, int>();
        ltree->corr = rtree->root;
        rtree->corr = ltree->root;
    }

    void add(int s, int t) 
    {
        ltree->add(s, 0);
        rtree->add(t, 0);
    }
    
    void remove(int s, int t) 
    {
        ltree->remove(s);
        rtree->remove(t);
    }
    
    int minPark() 
    {
        return 5;
    }
};


#endif