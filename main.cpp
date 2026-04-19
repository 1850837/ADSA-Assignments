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
Node* rotateRight(Node* a){
    Node* b = a->getLeft();
    Node* c = b->getRight();

    b->setRight(a);
    a->setLeft(c);

    a->setHeight(max(heightOf(a->getLeft()), heightOf(a->getRight())) + 1);
    b->setHeight(max(heightOf(b->getLeft()), heightOf(b->getRight())) + 1);

    return b;
}

// left rotate
Node* rotateLeft(Node* a){
    Node* b = a->getRight();
    Node* c = b->getLeft();

    b->setLeft(a);
    a->setRight(c);

    a->setHeight(max(heightOf(a->getLeft()), heightOf(a->getRight())) + 1);
    b->setHeight(max(heightOf(b->getLeft()), heightOf(b->getRight())) + 1);

    return b;
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
        return rotateRight(current);
    }

    // right-right case
    if (balance < -1 && key > current->getRight()->getKey()){
        return rotateLeft(current);
    }

    // left-right case
    if (balance > 1 && key > current->getLeft()->getKey()){
        current->setLeft(rotateLeft(current->getLeft()));
        return rotateRight(current);
    }

    // right-left case
    if (balance < -1 && key < current->getRight()->getKey()){
        current->setRight(rotateRight(current->getRight()));
        return rotateLeft(current);
    }

    // no rotation needed
    return current;

}

// === DELETE ===

// always returns root node
Node* deleteNode(Node* root, int key){

    // traversing to find value recursively
    // base case
    if (root == nullptr){
        return root;
    }

    // traversing
    if (key < root->getKey()){
        root->setLeft(deleteNode(root->getLeft(), key));
    }
    else if (key > root->getKey()){
        root->setRight(deleteNode(root->getRight(), key));
    }

    // found value
    else {
        if ((root->getLeft() == nullptr) || root->getRight() == nullptr){
            Node* temp = nullptr;
            if (root->getLeft() != nullptr){
                temp = root->getLeft();
            }
            else if (root->getRight() != nullptr){
                temp = root->getRight();
            }

            // case where the node has no children
            if (temp == nullptr){
                temp = root;
                root = nullptr;
            }

            // case where the node has a child
            else {
                delete root;
                return temp;
            }
        }

        // case where node has two children
        else {
            Node* temp = root->getRight();

            while (temp->getLeft() != nullptr) {
                temp = temp->getLeft();
            }

            root->setKey(temp->getKey());

            root->setRight(deleteNode(root->getRight(), temp->getKey()));
        }
    }

    // case where the tree only had one node
    if (root == nullptr){
        return root;
    }

    // update the height of the current node
    if (root->getLeft() == nullptr){

        // case where no children
        if (root->getRight() == nullptr){
            root->setHeight(1);
        }

        // case for right child only
        else {
            root->setHeight(1 + root->getRight()->getHeight());
        }
    }
    else {

        // case for left child only
        if (root->getRight() == nullptr){
            root->setHeight(1 + root->getLeft()->getHeight());
        }

        // case for two children
        else {
            root->setHeight(1 + max(root->getLeft()->getHeight(), root->getRight()->getHeight()));
        }
    }

    // update the balance and rotate accordingly
    int balance = root->getBalance();

    // left left case
    if (balance > 1 && (root->getLeft()->getBalance()) >= 0){
        return rotateRight(root);
    }

    // left right case
    if (balance > 1 && (root->getLeft()->getBalance()) < 0){
        root->setLeft(rotateLeft(root->getLeft()));
        return rotateRight(root);
    }

    // right right case
    if (balance < -1 && (root->getRight()->getBalance()) <= 0){
        return rotateLeft(root);
    }

    // right left case
    if (balance < -1 && (root->getRight()->getBalance()) > 0){
        root->setRight(rotateRight(root->getRight()));
        return rotateLeft(root);
    }

    // no rotation needed
    return root;
}

// === TRAVERSALS ===

vector<int> preOrder(Node* current, vector<int> trav){
    if (current == nullptr){
        return trav;
    }
    trav.push_back(current->getKey());
    trav = preOrder(current->getLeft(), trav);
    trav = preOrder(current->getRight(), trav);

    return trav;
}

vector<int> inOrder(Node* current, vector<int> trav){
    if (current == nullptr){
        return trav;
    }
    trav = inOrder(current->getLeft(), trav);
    trav.push_back(current->getKey());
    trav = inOrder(current->getRight(), trav);

    return trav;
}

vector<int> postOrder(Node* current, vector<int> trav){
    if (current == nullptr){
        return trav;
    }
    trav = postOrder(current->getLeft(), trav);
    trav = postOrder(current->getRight(), trav);
    trav.push_back(current->getKey());

    return trav;
}

// === MAIN ===

int main(){
    Node* root = nullptr;

    // collecting input

    vector<string> values = {};
    input(&values);

    // inserting and deleting vals

    for (int i = 0; i < values.size(); i++){

        // insertion
        if (values[i][0] == 'A'){
            root = insert(root, stoi(values[i].substr(1)));
        }

        // deletion
        if (values[i][0] == 'D'){
            root = deleteNode(root, stoi(values[i].substr(1)));
        }
    }

    // traversing

    vector<int> traversal;
    if (values[values.size()-1] == "PRE"){
        traversal = preOrder(root, traversal);
    }
    else if (values[values.size()-1] == "IN"){
        traversal = inOrder(root, traversal);
    }
    else if (values[values.size()-1] == "POST"){
        traversal = postOrder(root, traversal);
    }

    // printing output

    if (traversal.empty()){
        cout << "EMPTY\n";
    }
    else {
        for (int i = 0; i < traversal.size() - 1; i++){
            cout << traversal[i] << " ";
        }
        cout << traversal[traversal.size()-1] << "\n";
    }

    return 0;
}