#include <iostream>
#include <vector>
#include <algorithm>

// School method for addition
std::vector<int> additionVec(std::vector<int> one, std::vector<int> two, int base) {
    int n = std::max(one.size(), two.size());

    // Ensure same length vectors
    while (one.size() < n) one.insert(one.begin(), 0);
    while (two.size() < n) two.insert(two.begin(), 0);

    std::vector<int> result(n + 1, 0);
    int carry = 0;

    for (int i = n - 1; i >= 0; i--) {
        int sum = one[i] + two[i] + carry;
        result[i + 1] = sum % base;
        carry = sum / base;
    }
    result[0] = carry;

    // Remove leading 0
    while (result.size() > 1 && result[0] == 0) {
        result.erase(result.begin());
    }
    return result;
}

// Vector subtraction
std::vector<int> sub(std::vector<int> a, std::vector<int> b, int base) {
    // ensuring that the bigger one is a
    if (b.size() > a.size()){
        std::vector<int> c = a;
        a = b;
        b = c;
    }

    // Ensuring that they have the same length
    int n = a.size();
    while (b.size() < n) b.insert(b.begin(), 0);
    
    std::vector<int> res(n);
    int borrow = 0;
    for (int i = n - 1; i >= 0; i--) {
        int s = a[i] - b[i] - borrow;
        if (s < 0) {
            s += base;
            borrow = 1;
        } else borrow = 0;
        res[i] = s;
    }
    while (res.size() > 1 && res[0] == 0) res.erase(res.begin());
    return res;
}

std::vector<int> karatsuba(std::vector<int> v1, std::vector<int> v2, int base) {
    // Remove leading zeroes
    while (v1.size() > 1 && v1[0] == 0) v1.erase(v1.begin());
    while (v2.size() > 1 && v2[0] == 0) v2.erase(v2.begin());

    // Base case for length 1
    if (v1.size() == 1 && v2.size() == 1) {
        int prod = v1[0] * v2[0];
        if (prod == 0) return {0};
        std::vector<int> res;
        while (prod > 0) {
            res.insert(res.begin(), prod % base);
            prod /= base;
        }
        return res;
    }
    
    // Base case for length 0
    if (v1.size() == 0 || v2.size() == 0){
        return {0};
    }

    int n = std::max(v1.size(), v2.size());
    while (v1.size() < n) v1.insert(v1.begin(), 0);
    while (v2.size() < n) v2.insert(v2.begin(), 0);

    int m = n / 2;
    int highPart = n - m;

    std::vector<int> xH(v1.begin(), v1.begin() + highPart);
    std::vector<int> xL(v1.begin() + highPart, v1.end());
    std::vector<int> yH(v2.begin(), v2.begin() + highPart);
    std::vector<int> yL(v2.begin() + highPart, v2.end());

    std::vector<int> z0 = karatsuba(xL, yL, base);
    std::vector<int> z2 = karatsuba(xH, yH, base);
    
    // Gauss Trick
    std::vector<int> s1 = additionVec(xH, xL, base);
    std::vector<int> s2 = additionVec(yH, yL, base);
    std::vector<int> z1 = karatsuba(s1, s2, base);

    std::vector<int> middle = sub(z1, additionVec(z2, z0, base), base);

    // Shift
    for (int i = 0; i < 2 * m; i++) z2.push_back(0);
    for (int i = 0; i < m; i++) middle.push_back(0);

    return additionVec(z2, additionVec(middle, z0, base), base);
}

int main() {
    long long i1, i2;
    int b;
    if (!(std::cin >> i1 >> i2 >> b)) return 0;

    std::vector<int> v1, v2;
    if (i1 == 0) v1.push_back(0);
    while (i1 > 0) {
        v1.insert(v1.begin(), i1 % 10);
        i1 /= 10;
    }
    if (i2 == 0) v2.push_back(0);
    while (i2 > 0) {
        v2.insert(v2.begin(), i2 % 10);
        i2 /= 10;
    }

    std::vector<int> res = additionVec(v1, v2, b);
    for (int i = 0; i < res.size(); i++){
        std::cout << res[i];
    }
    std::cout << " ";

    res = karatsuba(v1, v2, b);
    for (int i = 0; i < res.size(); i++){
        std::cout << res[i];
    }
    std::cout << std::endl;

    return 0;
}
