#ifndef BKU_TREE
#define BKU_TREE

#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
using namespace std;

template <class K, class V>
class BKUTree 
{
public:
    class AVLTree;
    class SplayTree;

    class Entry 
    {
    public:
        K           key;
        V           value;

        Entry(K key, V value) 
            : key(key), value(value) 
        {}
    };

private:
    AVLTree*        avl;
    SplayTree*      splay;
    queue<K>        keys;
    int             maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) { 
        avl = new AVLTree();
        splay = new SplayTree();
        this->maxNumOfKeys = maxNumOfKeys; 
    }
    ~BKUTree() { 
        this->clear(); 
        if (avl != nullptr) delete avl;
        if (splay != nullptr) delete splay;
    }

    void    add(K key, V value);
    void    remove(K key);
    V       search(K key, vector<K>& traversedList);
    void    traverseNLROnAVL(void (*func)(K key, V value));
    void    traverseNLROnSplay(void (*func)(K key, V value));
    void    clear();

    class SplayTree 
    {
    public:
        class Node 
        {
        public:
            Entry*                      entry;
            Node*                       left;
            Node*                       right;
            typename AVLTree::Node*     corr;
        
            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) 
                : entry(entry), left(left), right(right), corr(nullptr)
            {}
            ~Node() { 
                if (entry != nullptr) { delete entry; }
                if (corr != nullptr) { corr->entry = nullptr; corr->corr = nullptr; } 
            }
        };

        void    rotLeft(Node* & root);
        void    rotRight(Node* & root);
        void    splay(Node* & root, K key);
        void    halfsplay(Node* & root, K key);
        V       splayOnce(K key, vector<K>& traversedList, bool record = false);
        bool    add(Node* & root, Entry* entry);
    
        void    traverseNLR(Node* & root, void (*func)(K key, V value));
        void    clear(Node* & root);

    public:
        Node*           root;

        SplayTree() : root(NULL) {};
        ~SplayTree() { this->clear(); };
        
        void    add(K key, V value);
        void    add(Entry* entry);
        void    remove(K key);
        V       search(K key);
        void    traverseNLR(void (*func)(K key, V value));
        void    clear();

    };

    class AVLTree 
    {
    public:
        class Node 
        {
        public:
            Entry*                          entry;
            Node*                           left;
            Node*                           right;
            int                             balance;
            typename SplayTree::Node*       corr;
        
            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) 
                : entry(entry), left(left), right(right), balance(0), corr(nullptr)
            {}
            ~Node() { 
                if (entry != nullptr) { delete entry; } 
                if (corr != nullptr) { corr->entry = nullptr; corr->corr = nullptr; } 
            }
        };
    
        void    rotLeft(Node* & root);
        void    rotRight(Node* & root);
        void    rotRL(Node* & root);
        void    rotLR(Node* & root);        
        bool    balanceLeft(Node* & root);
        bool    balanceRight(Node* & root);
        bool    add(Node* & root, Entry* entry);
        bool    remove(Node* & root, K key);
        V       search(Node* root, K key);

        void    traverseNLR(Node* & root, void (*func)(K key, V value));
        void    clear(Node* & root);

    public:
        Node*           root;

        AVLTree() : root(NULL) {}
        ~AVLTree() { this->clear(); }
        
        void    add(K key, V value);
        void    add(Entry* entry);
        void    remove(K key);
        V       search(K key);
        void    traverseNLR(void (*func)(K key, V value));
        void    clear();
    };
};



/********************************
 *                              *
 *      BKU TREE METHODS        *
 *                              *
 ********************************/

template <class K, class V>
void    BKUTree<K, V>::add(K key, V value)
{
    Entry* ePtr = new Entry(key, value);
    avl->add(ePtr);
    splay->add(ePtr);

    typename AVLTree::Node*         newAVLNode = avl->root;
    typename SplayTree::Node*       newSplayNode = splay->root;

    while (newAVLNode->entry->key != key) {
        if (newAVLNode->entry->key < key) 
            newAVLNode = newAVLNode->right;
        else newAVLNode = newAVLNode->left;
    }
    while (newSplayNode->entry->key != key) {
        if (newSplayNode->entry->key < key) 
            newSplayNode = newSplayNode->right;
        else newSplayNode = newSplayNode->left;
    }
    newAVLNode->corr = newSplayNode;
    newSplayNode->corr = newAVLNode;
    
    if (keys.size() == (long unsigned int)maxNumOfKeys) {
        keys.pop();
    }
    keys.push(key);
}
template <class K, class V>
void    BKUTree<K, V>::remove(K key)
{
    avl->remove(key);
    splay->remove(key);
    queue<K> shift;
    bool addRoot = false;
    while (!keys.empty()) {
        if (keys.front() == key) {
            keys.pop();
            if (!addRoot && splay->root != nullptr) {
                keys.push(splay->root->entry->key);
                addRoot = true;
            }
        }
        else {
            shift.push(keys.front());
            keys.pop();
        }
    }
    while (!shift.empty()) {
        keys.push(shift.front());
        shift.pop();
    }
}
template <class K, class V>
V       BKUTree<K, V>::search(K key, vector<K>& traversedList)
{
    if (key == splay->root->entry->key) {
        return splay->root->entry->value;
    }
    queue<K> shift;
    bool found = false;
    while (!keys.empty()) {
        if (keys.front() == key) found = true;
        shift.push(keys.front());
        keys.pop();
    }
    while (!shift.empty()) {
        keys.push(shift.front());
        shift.pop();
    }

    if (found) {
        return splay->splayOnce(key, traversedList, true);
    }
    else {
        // Search on AVL from r
        // Update traverseList while you're at it
        typename AVLTree::Node* r = splay->root->corr;
        while (r != nullptr)
        {
            if (key != r->entry->key) {
                traversedList.push_back(r->entry->key);
                if (r->entry->key < key) {
                    r = r->right;
                }
                else {
                    r = r->left;
                }
            }
            else {
                if (keys.size() == (long unsigned int)maxNumOfKeys) {
                    keys.pop();
                }
                keys.push(key);
                return splay->splayOnce(key, traversedList, true);
                // return r->entry->value;
            } 
        }

        // If not found from r, search on AVL from root, if cross r then end
        r = avl->root;
        while (r != nullptr) 
        {
            if (key != r->entry->key) {
                if (r == splay->root->corr) 
                    throw "Not found";

                traversedList.push_back(r->entry->key);
                if (r->entry->key < key) {
                    r = r->right;
                }
                else {
                    r = r->left;
                }
            }
            else {
                if (keys.size() == (long unsigned int)maxNumOfKeys) {
                    keys.pop();
                }
                keys.push(key);
                return splay->splayOnce(key, traversedList);
                // return r->entry->value;
            }
        }
        throw "Not found";
    }
}
template <class K, class V>
void    BKUTree<K, V>::traverseNLROnAVL(void (*func)(K key, V value))
{
    avl->traverseNLR(func);
}
template <class K, class V>
void    BKUTree<K, V>::traverseNLROnSplay(void (*func)(K key, V value))
{
    splay->traverseNLR(func);
}
template <class K, class V>
void    BKUTree<K, V>::clear()
{
    avl->clear();
    splay->clear();
    while (!keys.empty()) keys.pop();
}



/********************************
 *                              *
 *      AVL TREE METHODS        *
 *                              *
 ********************************/

template <class K, class V>
void    BKUTree<K, V>::AVLTree::add(K key, V value)
{
    add(new Entry(key, value));
}
template <class K, class V>
void    BKUTree<K, V>::AVLTree::add(Entry* entry)
{
    Node* p = root;
    while (p != nullptr) {
        if (p->entry->key < entry->key) {
            p = p->right;
        }
        else if (p->entry->key > entry->key) {
            p = p->left;
        }
        else throw "Duplicate key";
    }
    add(root, entry);
}
template <class K, class V>
void    BKUTree<K, V>::AVLTree::remove(K key)
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
    throw "Not found";
}
template <class K, class V>
V       BKUTree<K, V>::AVLTree::search(K key)
{
    return search(root, key);
}
template <class K, class V>
void    BKUTree<K, V>::AVLTree::traverseNLR(void (*func)(K key, V value))
{
    traverseNLR(root, func);
}
template <class K, class V>
void    BKUTree<K, V>::AVLTree::clear()
{
    clear(this->root);
}



template <class K, class V> 
void    BKUTree<K, V>::AVLTree::rotLeft(Node* & root) 
{
    Node* p = root;
    root = root->right;
    p->right = root->left;
    root->left = p;
}
template <class K, class V> 
void    BKUTree<K, V>::AVLTree::rotRight(Node* & root) 
{
    Node* p = root;
    root = root->left;
    p->left = root->right;
    root->right = p;       
}
template <class K, class V> 
void    BKUTree<K, V>::AVLTree::rotRL(Node* & root) 
{
    rotRight(root->right);
    rotLeft(root);
}
template <class K, class V> 
void    BKUTree<K, V>::AVLTree::rotLR(Node* & root)
{
    rotLeft(root->left);
    rotRight(root);
}
template <class K, class V> 
bool    BKUTree<K, V>::AVLTree::balanceLeft(Node* & root) 
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
bool    BKUTree<K, V>::AVLTree::balanceRight(Node* & root) 
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
bool    BKUTree<K, V>::AVLTree::add(Node* & root, Entry* entry) 
{
    if (root == nullptr) {
        root = new Node(entry);
        return true;
    }
    if (entry->key < root->entry->key) {
        return add(root->left, entry) ? balanceLeft(root) : false;
    }
    else {
        return add(root->right, entry) ? balanceRight(root) : false;
    }
    return false;
}
template <class K, class V>
bool    BKUTree<K, V>::AVLTree::remove(Node* & root, K key)
{
    if (root == nullptr) return false;

    if (key < root->entry->key) 
        return remove(root->left, key) ? !balanceRight(root) : false;
    else if (root->entry->key < key)
        return remove(root->right, key) ? !balanceLeft(root) : false;
    else
    {
        // Ensure removal in splay tree
        if (root->corr != nullptr) {
            root->corr->entry = new Entry(root->entry->key, root->entry->value);
            root->corr->corr = nullptr;
            root->corr = nullptr;
        }
        
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

            Entry* pE = root->entry;
            // Stealing entry
            if (succ->entry != nullptr) {
                root->entry = succ->entry;            
                succ->entry = pE;
            }
            if (succ->corr != nullptr) {
                root->corr = succ->corr;
                root->corr->corr = root;
                succ->corr = nullptr;
            }
            return remove(root->left, succ->entry->key) ? !balanceRight(root) : false;
        }
        return true;
    }
}



template <class K, class V>
void    BKUTree<K, V>::AVLTree::traverseNLR(Node* & root, void (*func)(K key, V value))
{
    if (root != nullptr) {
        func(root->entry->key, root->entry->value);
        traverseNLR(root->left, func);
        traverseNLR(root->right, func);
    }
}
template <class K, class V>
void    BKUTree<K, V>::AVLTree::clear(Node* & root)
{
    if (root != nullptr) {
        clear(root->left);
        clear(root->right);
        Node* p = root;
        if (root->entry != nullptr) { 
            delete root->entry;
            root->entry = nullptr;
            root->corr->entry = nullptr;
        }
        if (root->corr != nullptr) {
            root->corr->corr = nullptr;     // Delete connection
        }
        root = nullptr;
        delete p;
    }
}



/********************************
 *                              *
 *      SPLAY TREE METHODS      *                              
 *                              *
 ********************************/



template <class K, class V>
void    BKUTree<K, V>::SplayTree::add(K key, V value)
{
    add(new Entry(key, value));
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::add(Entry* entry)
{
    Node* p = root;
    while (p != nullptr) {
        if (p->entry->key < entry->key) {
            p = p->right;
        }
        else if (p->entry->key > entry->key) {
            p = p->left;
        }
        else throw  "Duplicate key";
    }
    if (add(root, entry))       // Odd
    {
        splay(root, entry->key);
    }
    else                        // Even
    {
        halfsplay(root, entry->key);
    }
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::remove(K key)
{
    search(key);

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
        Node* p = root->left;
        bool odd = true;
        while (p->right != nullptr) {
            p = p->right;
            odd = !odd;
        }
        if (odd) {
            splay(root->left, key);
        }
        else {
            halfsplay(root->left, key);
        }
        p = root;
        root = root->left;
        root->right = p->right;
        delete p;
    }
}
template <class K, class V>
V       BKUTree<K, V>::SplayTree::search(K key)
{
    Node* p = root;
    bool odd = true;
    while (p != nullptr) {
        if (p->entry->key < key) {
            p = p->right;
            odd = !odd;
        }
        else if (p->entry->key > key) {
            p = p->left;
            odd = !odd;
        }
        else {
            if (odd) {
                splay(root, key);
            }
            else {
                halfsplay(root, key);
            }
            return p->entry->value;
        }
    }
    throw "Not found";
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::traverseNLR(void (*func)(K key, V value))
{
    traverseNLR(root, func);
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::clear()
{
    clear(this->root);
}



template <class K, class V> 
void    BKUTree<K, V>::SplayTree::rotLeft(Node* & root) 
{
    if (root->right != nullptr) {
        Node* p = root;
        root = root->right;
        p->right = root->left;
        root->left = p;    
    }
}
template <class K, class V> 
void    BKUTree<K, V>::SplayTree::rotRight(Node* & root) 
{
    if (root->left != nullptr) {
        Node* p = root;
        root = root->left;
        p->left = root->right;
        root->right = p;
    }
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::splay(Node* & root, K key) 
{
    // If our node is on the root already
    if (root != nullptr && root->entry->key != key) 
    {
        if (key < root->entry->key) 
        {
            if (root->left == nullptr) return;
            if (key < root->left->entry->key) {
                splay(root->left->left, key);
                rotRight(root);
            }
            else if (root->left->entry->key < key) {
                splay(root->left->right, key);
                if (root->left->right != nullptr) {
                    rotLeft(root->left);
                }
            }
            rotRight(root);
        }
        else if (root->entry->key < key)
        {
            if (root->right == nullptr) return;
            if (root->right->entry->key < key) {
                splay(root->right->right, key);
                rotLeft(root);
            }
            else if (key < root->right->entry->key) {
                splay(root->right->left, key);
                if (root->right->left != nullptr) {
                    rotRight(root->right);
                }
            }
            rotLeft(root);
        }
    }
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::halfsplay(Node* & root, K key) 
{
    if (key < root->entry->key) {
        splay(root->left, key);
        rotRight(root);
    }
    else {
        splay(root->right, key);
        rotLeft(root);
    }
}
template <class K, class V>
V       BKUTree<K, V>::SplayTree::splayOnce(K key, vector<K>& traversedList, bool record) 
{
    // Note: node containing the key above will not be the root
    // in this implementation
    Node** p = &root;
    while (*p != nullptr) 
    {
        if (key < (*p)->entry->key)
        {
            if (record) traversedList.push_back((*p)->entry->key);
            if (key == (*p)->left->entry->key) {
                rotRight(*p);
                return (*p)->entry->value;
            }
            else if ((*p)->left->left != nullptr && key == (*p)->left->left->entry->key) {      // Left - Left
                if (record) traversedList.push_back((*p)->left->entry->key);
                rotRight(*p);
                rotRight(*p);
                return (*p)->entry->value;
            }
            else if ((*p)->left->right != nullptr && key == (*p)->left->right->entry->key) {    // Left - Right
                if (record) traversedList.push_back((*p)->left->entry->key);
                rotLeft(((*p)->left));
                rotRight(*p);
                return (*p)->entry->value;
            }
            else {
                p = &(*p)->left;
            }
        }
        else if ((*p)->entry->key < key)
        {
            if (record) traversedList.push_back((*p)->entry->key);
            if (key == (*p)->right->entry->key) {
                rotLeft(*p);
                return (*p)->entry->value;
            }
            else if ((*p)->right->right != nullptr && key == (*p)->right->right->entry->key) {    // Right - Right
                if (record) traversedList.push_back((*p)->right->entry->key);
                rotLeft(*p);
                rotLeft(*p);
                return (*p)->entry->value;
            }
            else if ((*p)->right->left != nullptr && key == (*p)->right->left->entry->key) {      // Left - Right
                if (record) traversedList.push_back((*p)->right->entry->key);
                rotRight((*p)->right);
                rotLeft(*p);
                return (*p)->entry->value;
            }
            else {
                p = &(*p)->right;
            }
        }
    }
    throw "Not found";
}
template <class K, class V>
bool    BKUTree<K, V>::SplayTree::add(Node* & root, Entry* entry)
{
    // Tree is empty
    if (root == nullptr) {
        root = new Node(entry);
        return true;
    }
    if (entry->key < root->entry->key) 
        return !add(root->left, entry);     // Provide binary information about the depth
    else  
        return !add(root->right, entry);
}



template <class K, class V>
void    BKUTree<K, V>::SplayTree::traverseNLR(Node* & root, void (*func)(K key, V value))
{
    if (root != nullptr) {
        func(root->entry->key, root->entry->value);
        traverseNLR(root->left, func);
        traverseNLR(root->right, func);
    }
}
template <class K, class V>
void    BKUTree<K, V>::SplayTree::clear(Node* & root)
{
    if (root != nullptr) {
        clear(root->left);
        clear(root->right);
        Node* p = root;
        if (root->entry != nullptr) { 
            delete root->entry;
            root->entry = nullptr;
            root->corr->entry = nullptr;
        }
        if (root->corr != nullptr) {
            root->corr->corr = nullptr;     // Delete connection
        }
        root = nullptr;
        delete p;
    }
}


#endif