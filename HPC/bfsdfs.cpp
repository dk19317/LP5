/*1.Design and implement Parallel Breadth First Search and Depth First Search based on existing
algorithms using OpenMP. Use a Tree or an undirected graph for BFS and DFS .

compile: g++ -fopenmp filename.cpp -o filename
Run: //run: ./filename

eg.  nodes: 6 and edges 5 (eg. edge 1 is 0 1)
    0
    |
    1
   / \
  2   3
 / \
4   5

*/


#include<iostream>
#include<omp.h>
#include<bits/stdc++.h>
#include<chrono>

using namespace std;

class Graph {
public:
    vector<vector<int>> graph;
    vector<bool> visited;
    int vertices = 0;
    int edges = 0;

    Graph() {
        cout << "Enter number of nodes: ";
        cin >> vertices;
        cout << "Enter number of edges: ";
        cin >> edges;
        graph.assign(vertices, vector<int>());
        for (int i = 0; i < edges; i++) {
            int a, b;
            cout << "Enter adjacent nodes: ";
            cin >> a >> b;
            addEdge(a, b);
        }
    }

    void addEdge(int a, int b) {
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    void printGraph() {
        for (int i = 0; i < vertices; i++) {
            cout << i << " -> ";
            for (auto j = graph[i].begin(); j != graph[i].end(); j++) {
                cout << *j << " ";
            }
            cout << endl;
        }
    }

    void initialize_visited() {
        visited.assign(vertices, false);
    }

    void dfs(int i) {
        stack<int> s;
        s.push(i);
        visited[i] = true;

        while (!s.empty()) {
            int current = s.top();
            cout << current << " ";
            s.pop();

            // Reverse the order of neighbors for left-to-right traversal
            for (auto j = graph[current].rbegin(); j != graph[current].rend(); j++) {
                if (!visited[*j]) {
                    s.push(*j);
                    visited[*j] = true;
                }
            }
        }
    }

    void parallel_dfs(int i) {
        stack<int> s;
        s.push(i);
        visited[i] = true;

        while (!s.empty()) {
            int current = s.top();
            cout << current << " ";
            #pragma omp critical
            s.pop();

            #pragma omp parallel for
            for (auto j = graph[current].begin(); j != graph[current].end(); j++) {
                if (!visited[*j]) {
                    #pragma omp critical
                    {
                        s.push(*j);
                        visited[*j] = true;
                    }
                }
            }
        }
    }

    void bfs(int i) {
        queue<int> q;
        q.push(i);
        visited[i] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << current << " ";
            for (auto j = graph[current].begin(); j != graph[current].end(); j++) {
                if (!visited[*j]) {
                    q.push(*j);
                    visited[*j] = true;
                }
            }
        }
    }

    void parallel_bfs(int i) {
        queue<int> q;
        q.push(i);
        visited[i] = true;

        while (!q.empty()) {
            int current = q.front();
            cout << current << " ";
            #pragma omp critical
            q.pop();

            #pragma omp parallel for
            for (auto j = graph[current].begin(); j != graph[current].end(); j++) {
                if (!visited[*j]) {
                    #pragma omp critical
                    {
                        q.push(*j);
                        visited[*j] = true;
                    }
                }
            }
        }
    }
};

int main() {
    Graph g;
    cout << "Adjacency List:\n";
    g.printGraph();

    // Depth First Search (DFS)
    g.initialize_visited();
    cout << "Depth First Search: \n";
    auto start = chrono::high_resolution_clock::now();
    g.dfs(0);
    cout << endl;
    auto end = chrono::high_resolution_clock::now();
    cout << "Time taken: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;

    // Parallel Depth First Search (Parallel DFS)
    g.initialize_visited();
    cout << "Parallel Depth First Search: \n";
    start = chrono::high_resolution_clock::now();
    g.parallel_dfs(0);
    cout << endl;
    end = chrono::high_resolution_clock::now();
    cout << "Time taken: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;

    // Breadth First Search (BFS)
    g.initialize_visited();
    cout << "Breadth First Search: \n";
    start = chrono::high_resolution_clock::now();
    g.bfs(0);
    cout << endl;
    end = chrono::high_resolution_clock::now();
    cout << "Time taken: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;

    // Parallel Breadth First Search (Parallel BFS)
    g.initialize_visited();
    cout << "Parallel Breadth First Search: \n";
    start = chrono::high_resolution_clock::now();
    g.parallel_bfs(0);
    cout << endl;
    end = chrono::high_resolution_clock::now();
    cout << "Time taken: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;

    return 0;
}
