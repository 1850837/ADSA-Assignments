#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

// M is 0-indexed internally: M[i][j] = probability of hop from node (i+1) to node (j+1).
double solve_part1(const std::vector<std::vector<double>>& M){

    // Creating a vector of probs for all vertices
    // Creating a vector of visited for all vertices (True = visited, False = not visited)
    std::vector<double> probabilities = {1};
    std::vector<bool> visited = {false};
    for (int i = 1; i < M.size(); i++){
        probabilities.push_back(-1);
        visited.push_back(false);
    }

    // Create a while loop that continues while there are unvisited nodes
    auto it = find(visited.begin(), visited.end(), false);
    double current_prob = 0;
    int current = 0;
    while (it != visited.end()){

        // Choosing a current vertex
        // This vertex will have the highest probability while not having been visited
        // It will always begin at the first node due to the setup
        for (int i = 0; i < visited.size(); i++){
            if (visited[i] == false){
                if (probabilities[i] > current_prob){
                    current_prob = probabilities[i];
                    current = i;
                }
            }
        }

        // Check all the neighbouring verticies, calculating the new prob and updating it if it's greater
        for (int i = 0; i < visited.size(); i++){

            // not current vertext
            if (i != current){
                // not visited
                if (visited[i] == false){
                 
                    // calculating new prob (will be 0 if not a neighbour)
                    double prob = probabilities[current]*M[current][i];
                    if (prob > probabilities[i]){
                        probabilities[i] = prob;
                    }

                }
            }
        }

        // Updating current vertex as visited
        visited[current] = true;

        it = find(visited.begin(), visited.end(), false);

        // Resetting current and current_prob
        current = 0;
        current_prob = 0;
    }

    return probabilities.back();

}

// Writes exactly 3 chosen edges (1-indexed) into edges_out.
// Returns the new max probability after zeroing those edges.
double solve_part2(const std::vector<std::vector<double>>& M,std::vector<std::pair<int,int>>& edges_out);

int main() {

    // reading in file
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

    double result1 = solve_part1(mat);

    std::cout << "PART1: " << result1 << "\n";

    return 0;
}