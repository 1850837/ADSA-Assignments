#include <iostream>
#include <vector>

using namespace std;

// === AVL NODE ===

class Node{
    private:
        int key;
        Node* left;
        Node* right;
        int height;

    public:

        // initialisation
        Node(int newKey){
            key = newKey;
            left = nullptr;
            right = nullptr;
            height = 1;
        }

        Node(){
            key = -1;
            left = nullptr;
            right = nullptr;
            height = 1;
        }

        // getters
        int getKey(){
            return key;
        }
        Node* getLeft(){
            return left;
        }
        Node* getRight(){
            return right;
        }
        int getHeight(){
            return height;
        }
        int getBalance(){
            if (getLeft() == nullptr && getRight() == nullptr){
                return 0;
            }
            if (getLeft() == nullptr){
                return (0 - getRight()->getHeight());
            }
            if (getRight() == nullptr) {
                return (getLeft()->getHeight());
            }
            return getLeft()->getHeight() - getRight()->getHeight();
        }
        
        // setters
        void setKey(int newKey){
            key = newKey;
            return;
        }
        void setLeft(Node* newLeft){
            left = newLeft;
            return;
        }
        void setRight(Node* newRight){
            right = newRight;
            return;
        }
        void setHeight(int newHeight){
            height = newHeight;
            return;
        }
};

// === INPUT ===

void input(vector<string>* values){
    string input = "";
    while (cin >> input){
        values->push_back(input);
        if ((input == "PRE" || input == "IN") || input == "POST"){
            break;
        }
    }

    return;
}

// === ROTATIONS ===

// helper function to handle height with care for nullptrs
int heightOf(Node* node){
    if (node == nullptr){
        return 0;
    }
    return node->getHeight();
}

// right rotate
Node* rightRotate(Node* y){
    Node* x = y->getLeft();
    Node* z = x->getRight();

    x->setRight(y);
    y->setLeft(z);

    y->setHeight(max(heightOf(y->getLeft()), heightOf(y->getRight())) + 1);
    x->setHeight(max(heightOf(x->getLeft()), heightOf(x->getRight())) + 1);

    return x;
}

// left rotate
Node* leftRotate(Node* x){
    Node* y = x->getRight();
    Node* z = y->getLeft();

    y->setLeft(x);
    x->setRight(z);

    x->setHeight(max(heightOf(x->getLeft()), heightOf(x->getRight())) + 1);
    y->setHeight(max(heightOf(y->getLeft()), heightOf(y->getRight())) + 1);

    return y;
}

// === INSERT ===

// always returns the root node
Node* insert(Node* current, int key){
    
    // base case
    if (current == nullptr){
        return new Node(key);
    }
    if (current->getKey() == -1){
        current->setKey(key);
        return current;
    }

    // recursive step: left
    if (key < current->getKey()){
        current->setLeft(insert(current->getLeft(), key));
    }
    // recursive step: right
    else if (key > current->getKey()){
        current->setRight(insert(current->getRight(), key));
    }
    // recursive step: key already inserted
    else {
        return current;
    }

    // updating heights while travelling back up
    int newHeight;
    if (current->getLeft() == nullptr && current->getRight() == nullptr){
        newHeight = 1;
    }
    else if (current->getLeft() == nullptr){
        newHeight = 1 + current->getRight()->getHeight();
    }
    else if (current->getRight() == nullptr){
        newHeight = 1 + current->getLeft()->getHeight();
    }
    else{
        newHeight = 1 + max(current->getLeft()->getHeight(), current->getRight()->getHeight());
    }
    current->setHeight(newHeight);

    // rebalancing
    int balance = current->getBalance();

    // left-left case
    if (balance > 1 && key < current->getLeft()->getKey()){
        return rightRotate(current);
    }

    // right-right case
    if (balance < -1 && key > current->getRight()->getKey()){
        return leftRotate(current);
    }

    // left-right case
    if (balance > 1 && key > current->getLeft()->getKey()){
        current->setLeft(leftRotate(current->getLeft()));
        return rightRotate(current);
    }

    // right-left case
    if (balance < -1 && key < current->getRight()->getKey()){
        current->setRight(rightRotate(current->getRight()));
        return leftRotate(current);
    }

    // no rotation needed
    return current;

}

// === TRAVERSALS ===
void traversalPre(Node* current){
    if (current == nullptr){
        return;
    }
    cout << current->getKey() << " ";
    traversalPre(current->getLeft());
    traversalPre(current->getRight());
}

// === MAIN ===

int main(){
    Node* root = new Node();

    // collecting input
    vector<string> values = {};
    input(&values);

    // testing DELETE LATER
    // for (int i = 0; i < values.size(); i++){
    //     cout << values[i] << " - ";
    // }
    // cout << "\n";

    // inserting the insert vals (NOT FINAL don't do it this order)
    for (int i = 0; i < values.size(); i++){
        if (values[i][0] == 'A'){
            root = insert(root, stoi(values[i].substr(1)));
        }
    }

    traversalPre(root);

    return 0;
}