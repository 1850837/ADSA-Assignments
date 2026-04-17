#include <iostream>

using namespace std;

// === AVL NODE ===
// untested

class Node{
    private:
        int key;
        Node* left;
        Node* right;
        int height;

    public:

        // initialisation
        Node(int key){
            key = key;
            left = nullptr;
            right = nullptr;
            height = 0;
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


// === MAIN ===

int main(){



    return 0;
}