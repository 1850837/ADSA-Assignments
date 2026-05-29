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

// Part 2 Helper Functions
std::vector<double> dijkstra(const std::vector<std::vector<double>>& M) {
    std::vector<double> probabilities(M.size(), -1.0);
    std::vector<bool> visited(M.size(), false);
    std::vector<int> predecessors(M.size(), -1);

    probabilities[0] = 1.0;

    while (true) {
        int current = -1;
        double max_prob = -1.0;

        // Choosing an unvisited vertex with the highest probability
        for (int i = 0; i < visited.size(); i++) {
            if (!visited[i] && probabilities[i] > max_prob) {
                max_prob = probabilities[i];
                current = i;
            }
        }

        // Case where no unvisited nodes are reachable
        if (current == -1 || max_prob <= 0) {
            break;
        }

        // Check all neighboring vertices
        for (int i = 0; i < visited.size(); i++) {
            if (i != current && !visited[i]) {
                double prob = probabilities[current] * M[current][i];
                if (prob > probabilities[i]) {
                    probabilities[i] = prob;
                    predecessors[i] = current;
                }
            }
        }

        visited[current] = true;
    }

    int last = visited.size() - 1;
    std::vector<double> path;

    // Edge case: if the destination is completely unreachable (Had to be added as the map changes here)
    if (probabilities[last] <= 0) {
        path.push_back(last);
        path.push_back(0.0);
        return path;
    }

    // Reconstruct path
    int temp = last;
    while (temp != -1) {
        path.push_back(temp);
        temp = predecessors[temp];
    }

    reverse(path.begin(), path.end());
    path.push_back(probabilities[last]);

    return path;
}

std::vector<std::pair<int,int>> become_pairs(std::vector<int> nodes){
    
    std::vector<std::pair<int,int>> pairs = {};

    for (int i = 0; i < nodes.size() - 1; i++){
        pairs.push_back({nodes[i], nodes[i+1]});
    }

    return pairs;

}

// Global vars
double current_prob = 2;
std::vector<std::pair<int,int>> pairs = {};

// Part 2 recursion
void cut_edge(int level, std::vector<std::vector<double>> M, std::vector<std::pair<int,int>> current_pairs) {

    // Base cases
    if (level == 3) {
        std::vector<double> dij = dijkstra(M);
        double prob = dij.back();

        if (prob < current_prob) {
            current_prob = prob;
            pairs = current_pairs;
        }
        return;
    }

    if (level > 3){
        return;
    }

    std::vector<double> dij = dijkstra(M);
    double prob = dij.back();
    dij.pop_back();

    // Convert path nodes to integers
    std::vector<int> dij_int;
    for (int i = 0; i < dij.size(); i++) {
        dij_int.push_back(dij[i]);
    }
    std::vector<std::pair<int,int>> best_path = become_pairs(dij_int);

    // Finding a third edge even if the prob is 0
    if (prob <= 0 || best_path.empty()) {
        bool found_edge = false;
        // Find ANY remaining edge in the graph that isn't 0
        for (int i = 0; i < M.size() && !found_edge; i++) {
            for (int j = 0; j < M.size(); j++) {
                if (M[i][j] > 0) {
                    std::vector<std::vector<double>> M2 = M;
                    M2[i][j] = 0;
                    std::vector<std::pair<int, int>> current_pairs2 = current_pairs;
                    current_pairs2.push_back({i, j});
                    
                    cut_edge(level + 1, M2, current_pairs2);
                    found_edge = true;
                    break;
                }
            }
        }
    }

    // Recursive case
    for (int i = 0; i < best_path.size(); i++) {
        std::vector<std::vector<double>> M2 = M;
        M2[best_path[i].first][best_path[i].second] = 0;
        
        std::vector<std::pair<int, int>> current_pairs2 = current_pairs;
        current_pairs2.push_back(best_path[i]);
        
        cut_edge(level + 1, M2, current_pairs2);
    }
}

// Writes exactly 3 chosen edges (1-indexed) into edges_out.
// Returns the new max probability after zeroing those edges.
double solve_part2(const std::vector<std::vector<double>>& M,std::vector<std::pair<int,int>>& edges_out){
    cut_edge(0, M, {});

    // converting from 0-indexed to 1-indexed
    std::vector<std::pair<int, int>> edges = pairs;
    for (int i = 0; i < pairs.size(); i++){
        pairs[i].first++;
        pairs[i].second++;
    }

    edges_out = pairs;

    return current_prob;
}

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

    double result1 = solve_part1(mat);

    std::cout << std::setprecision(6) << std::fixed;

    std::cout << "PART1: " << result1 << "\n";

    std::vector<std::pair<int,int>> edges_out1 = {};

    double result2 = solve_part2(mat, edges_out1);

    // printing part 2a
    std::cout << "PART2_EDGES: (";
    for (int i = 0; i < edges_out1.size(); i++){
        std::cout << edges_out1[i].first << "," << edges_out1[i].second << ")";
        if (i != edges_out1.size() - 1){
            std::cout << " (";
        }
    }
    std::cout << "\n";

    // printing part 2b
    std::cout << "PART2_PROB: " << result2 << "\n";

    // testing DON'T FORGET TO CHANGE THE OUTPUT DECIMAL PLACES

    return 0;
}