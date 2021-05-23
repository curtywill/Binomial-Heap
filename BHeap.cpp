#include <iostream>

using namespace std;

template <typename keytype>
class Node {
    public:
        //members
        keytype key;
        int degree;
        Node<keytype> *sister;
        Node<keytype> *child;
        
        //methods
        Node() {
            degree = 0;
            sister = nullptr;
            child = nullptr;
        }

        Node(keytype k) {
            key = k;
            degree = 0;
            sister = nullptr;
            child = nullptr;
        }

        Node(Node<keytype> *ptr) { // ONLY FOR JOIN
            key = ptr->key;
            degree = ptr->degree;
            child = ptr->child;
            sister = nullptr;
        }

        Node(const Node<keytype> &other) {
            key = other.key;
            degree = other.degree;
            if (other.child != nullptr) child = new Node(*other.child);
            else child = nullptr;

            if (other.sister != nullptr) sister = new Node(*other.sister);
            else sister = nullptr;
        }

        friend void swap(Node<keytype> &first, Node<keytype> &other){
            std::swap(first.key, other.key);
            std::swap(first.degree, other.degree);
            std::swap(first.child, other.child);
            std::swap(first.sister, other.sister);
        }

        Node<keytype>& operator=(const Node<keytype> other){ // copy swap idiom
            swap(*this, other);
            return *this;
        }
};

template <typename keytype>
class BHeap {
    public:
        BHeap();
        BHeap(keytype keys[], int s);
        BHeap(Node<keytype> *x);
        BHeap(const BHeap<keytype> &other);
        friend void swap(BHeap<keytype> &first, BHeap<keytype> &other){
            std::swap(first.root, other.root);
            std::swap(first.min, other.min);
        }
        BHeap<keytype>& operator=(const BHeap<keytype> other);
        ~BHeap();
        keytype peekKey();
        keytype extractMin();
        void insert(keytype key);
        void merge(BHeap<keytype> &H2); 
        void printKey();
        
    private:
        void preorder(Node<keytype> *x);
        void link(Node<keytype> *x, Node<keytype> *y);
        void join(BHeap<keytype> &H2);
        void resetMin();
        void destroy(Node<keytype> *x);
        Node<keytype> *reverse(Node<keytype> *x);
        Node<keytype> *root;
        Node<keytype> *min;
};

//HELPER FUNCTIONS
template <typename keytype>
void BHeap<keytype>::preorder(Node<keytype> *x)
{
    if (x == nullptr) return;
    cout << x->key << " ";
    preorder(x->child);
    preorder(x->sister);
}

template <typename keytype>
void BHeap<keytype>::link(Node<keytype> *x, Node<keytype> *y)
{
    y->sister = x->child;
    x->child = y;
    x->degree++;
}

template <typename keytype>
void BHeap<keytype>::join(BHeap<keytype> &H2)
{
    Node<keytype> *temp = root;
    Node<keytype> *other = H2.root;
    Node<keytype> *x;

    if (temp->degree <= other->degree){ 
        x = new Node<keytype>(temp);
        temp = temp->sister;
    }
    else{
        x = new Node<keytype>(other);
        other = other->sister;
    }

    Node<keytype> *y = x;
    while (temp != nullptr && other != nullptr){
        if (temp->degree == other->degree){
            y->sister = new Node<keytype>(temp);
            y = y->sister;
            y->sister = new Node<keytype>(other);
            y = y->sister;
            temp = temp->sister;
            other = other->sister;
        }
        else if (temp->degree < other->degree){
            y->sister = new Node<keytype>(temp);
            y = y->sister;
            temp = temp->sister;
        }
        else if (other->degree < temp->degree){
            y->sister = new Node<keytype>(other);
            y = y->sister;
            other = other->sister;
        }
    }

    if (temp != nullptr)
        y->sister = temp;

    if (other != nullptr)
        y->sister = other;

    H2.root = nullptr;
    root = nullptr;
    root = x;
    resetMin();
}

template <typename keytype>
void BHeap<keytype>::resetMin()
{
    min = root;
    for (Node<keytype> *ptr = root; ptr != nullptr; ptr = ptr->sister)
        if (ptr->key < min->key) min = ptr;
}

template <typename keytype>
void BHeap<keytype>::destroy(Node<keytype> *x)
{
    if (x == nullptr) return;

    destroy(x->child);
    destroy(x->sister);
    delete x;
}

template <typename keytype>
Node<keytype> *BHeap<keytype>::reverse(Node<keytype> *x)
{
    Node<keytype> *prev = nullptr;
    Node<keytype> *y = x;
    Node<keytype> *next = nullptr;

    while (y != nullptr){
        next = y->sister;
        y->sister = prev;
        prev = y;
        y = next;
    }

    return prev;
}

//MAIN FUNCTIONS
template <typename keytype>
BHeap<keytype>::BHeap()
{
    root = nullptr;
    min = nullptr;
}

template <typename keytype>
BHeap<keytype>::BHeap(keytype keys[], int s)
{
    root = nullptr;
    min = nullptr;
    for (int i = 0; i < s; i++)
        insert(keys[i]);
}

template <typename keytype>
BHeap<keytype>::BHeap(Node<keytype> *x)
{
    root = x;
    if (root->sister == nullptr) min = root;
    else resetMin();
}

template <typename keytype>
BHeap<keytype>::BHeap(const BHeap<keytype> &other)
{
    root = new Node<keytype>(*other.root);
    resetMin();
}

template <typename keytype>
BHeap<keytype>& BHeap<keytype>::operator=(const BHeap<keytype> other)
{
    swap(*this, other);
    return *this;
}

template <typename keytype>
BHeap<keytype>::~BHeap()
{
    destroy(root);
}

template <typename keytype>
keytype BHeap<keytype>::peekKey()
{
    return min->key;
}

template <typename keytype>
keytype BHeap<keytype>::extractMin()
{
    Node<keytype> *x = min->child;
    keytype k = min->key;
    if (root == min) root = min->sister;
    else {
        for (Node<keytype> *ptr = root; ptr != nullptr; ptr = ptr->sister){
            if (ptr->sister == min){
                ptr->sister = min->sister;
                break;
            }
        }
    }

    if (x == nullptr){
        resetMin();
        return k;
    }
    
    BHeap<keytype> *prime = new BHeap<keytype>(reverse(x));
    merge(*prime);

    return k;
}

template <typename keytype>
void BHeap<keytype>::insert(keytype k)
{
    Node<keytype> *x = new Node<keytype>(k);
    if (root == nullptr){
        root = x;
        min = root;
        return;
    }
    BHeap<keytype> *prime = new BHeap<keytype>(x);
    merge(*prime);
}  

template <typename keytype>
void BHeap<keytype>::merge(BHeap<keytype> &H2)
{
    if (root == nullptr){
        root = new Node<keytype>(*H2.root);
        resetMin();
        return;
    }
    
    join(H2);
    Node<keytype> *prev = nullptr;
    Node<keytype> *x = root;
    Node<keytype> *next = x->sister;
    while (next != nullptr){
        if ((x->degree != next->degree) || (next->sister != nullptr && next->sister->degree == x->degree)){
            prev = x;
            x = next;
        }
        else{
            if (x->key <= next->key){
                x->sister = next->sister;
                link(x, next);
            }
            else{
                if (prev == nullptr) root = next;
                else prev->sister = next;
                link(next, x);
                x = next;
            }
        }
        next = x->sister;
    }
}

template <typename keytype>
void BHeap<keytype>::printKey()
{
    for (Node<keytype> *cur = root; cur != nullptr; cur = cur->sister){
        cout << "B" << cur->degree << endl;
        cout << cur->key << " ";
        preorder(cur->child);
        cout << endl << endl;
    }
}
