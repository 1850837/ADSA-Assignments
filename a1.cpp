#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>

// School method for integer addition

unsigned int addition(std::vector<int>* one, std::vector<int>* two, int base){
    std::vector<int> carry(one->size() + 1, 0);
    std::vector<int> result(one->size() + 1, 0);

    // every element addressed by i except result and carry which are i+1 always
    // address the final part of result and carry after the loop
    for (int i = one->size() - 1; i >= 0; i--){
        int temp = one->at(i) + two->at(i) + carry[i+1];

        if (temp >= base){
            temp = temp - base;
            carry[i]++;
        }
        result[i+1] = temp;
    }
    result[0] = carry[0];

    unsigned int final = 0;
    for (int i = 0; i < result.size(); i++){
        final = final + result[i]*(pow(10, result.size() - 1 - i));
    }

    return final;
}

// Main function for input and output

int main(){
    // Take in inputs
    unsigned int i1, i2;
    int b;
    std::cin >> i1 >> i2 >> b;

    // Turn the values into vectors of each digit, v1:
    std::vector<int> v1 = {};
    std::vector<int>::iterator it;

    int val;
    while (i1 > 9){
        it = v1.begin();
        val = i1 % 10;
        v1.insert(it, val);
        i1 = i1 / 10;
    }
    it = v1.begin();
    val = i1;
    v1.insert(it, val);

    // v2
    std::vector<int> v2 = {};

    while (i2 > 9){
        it = v2.begin();
        val = i2 % 10;
        v2.insert(it, val);
        i2 = i2 / 10;
    }
    it = v2.begin();
    val = i2;
    v2.insert(it, val);

    // Making vectors the same length
    int diff = v1.size() - v2.size();
    diff = abs(diff);

    if (v1.size() < v2.size()){
        it = v1.begin();
        v1.insert(it, diff, 0);
    }
    else if (v1.size() > v2.size()){
        it = v2.begin();
        v2.insert(it, diff, 0);
    }

    // calling for addition
    std::vector<int>* one = &v1;
    std::vector<int>* two = &v2;

    std::cout << addition(one, two, b);
    std::cout << "\n";

    return 0;
}