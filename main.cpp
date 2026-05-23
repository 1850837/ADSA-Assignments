#include <vector>
#include <fstream>
#include <iostream>

// M is 0-indexed internally: M[i][j] = probability of hop from node (i+1) to node (j+1).
double solve_part1(const std::vector<std::vector<double>>& M);

// Writes exactly 3 chosen edges (1-indexed) into edges_out.
// Returns the new max probability after zeroing those edges.
double solve_part2(const std::vector<std::vector<double>>& M,std::vector<std::pair<int,int>>& edges_out);

int main() {

    std::ifstream matrix("matrix.txt");

    // checking if file opened successfully
    if (!matrix) {
        std::cout << "Error opening file\n";
        return -1;
    }

    int val;
    matrix >> val;

    std::vector<std::vector<double>> mat = {};

    std::vector<double> current = {};
    double temp;

    for (int i = 0; i < val; i++){
        for (int j = 0; j < val; j++){
            matrix >> temp;
            current.push_back(temp);
        }
        mat.push_back(current);
        current = {};
    }

    matrix.close();

    // testing
    for (int i = 0; i < val; i++){
        for (int j = 0; j < val; j++){
            std::cout << mat[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}