
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <functional>
#include <iomanip>
#include <chrono>

using namespace std;

typedef long long ll;
const ll INF = 1e18;

struct Edge {
    int u, v;
    ll cap, flow;
    Edge(int u, int v, ll cap) : u(u), v(v), cap(cap), flow(0) {}
};

struct Dinic {
    int n;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> level, ptr;
    
    Dinic(int n) : n(n), adj(n) {}
    
    void add_edge(int u, int v, ll cap) {
        if (u >= n || v >= n) {
            cerr << "Error: Edge nodes out of range: " << u << " -> " << v << endl;
            return;
        }
        adj[u].push_back(edges.size());
        edges.emplace_back(u, v, cap);
        adj[v].push_back(edges.size());
        edges.emplace_back(v, u, 0);
    }
    
    bool bfs(int s, int t) {
        level.assign(n, -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                Edge& e = edges[id];
                if (e.cap - e.flow > 0 && level[e.v] == -1) {
                    level[e.v] = level[u] + 1;
                    q.push(e.v);
                }
            }
        }
        
        return level[t] != -1;
    }
    
    ll dfs(int u, int t, ll flow) {
        if (u == t || flow == 0) return flow;
        
        for (int& i = ptr[u]; i < (int)adj[u].size(); ++i) {
            int id = adj[u][i];
            Edge& e = edges[id];
            
            if (e.cap - e.flow > 0 && level[e.v] == level[u] + 1) {
                ll pushed = dfs(e.v, t, min(flow, e.cap - e.flow));
                if (pushed > 0) {
                    e.flow += pushed;
                    edges[id ^ 1].flow -= pushed;
                    return pushed;
                }
            }
        }
        
        return 0;
    }
    
    ll max_flow(int s, int t) {
        ll flow = 0;
        
        while (bfs(s, t)) {
            ptr.assign(n, 0);
            while (ll pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        
        return flow;
    }
    
    vector<int> min_cut(int s) {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                Edge& e = edges[id];
                if (e.cap - e.flow > 0 && !visited[e.v]) {
                    visited[e.v] = true;
                    q.push(e.v);
                }
            }
        }
        
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (visited[i]) result.push_back(i);
        }
        
        return result;
    }
};

// Find all k-cliques in the graph (improved implementation)
vector<set<int>> find_k_cliques(int k, const vector<vector<int>>& adj_list, int num_vertices) {
    vector<set<int>> cliques;
    vector<int> current;
    
    function<void(int, int)> backtrack = [&](int start, int depth) {
        if (depth == k) {
            set<int> clique(current.begin(), current.end());
            cliques.push_back(clique);
            return;
        }
        
        for (int u = start; u < num_vertices; ++u) {
            // Check if u can be added to the current clique
            bool can_add = true;
            for (int v : current) {
                // Check if u is connected to all vertices in the current clique
                bool connected = false;
                for (int neighbor : adj_list[v]) {
                    if (neighbor == u) {
                        connected = true;
                        break;
                    }
                }
                if (!connected) {
                    can_add = false;
                    break;
                }
            }
            
            if (can_add) {
                current.push_back(u);
                backtrack(u + 1, depth + 1);
                current.pop_back();
            }
        }
    };
    
    backtrack(0, 0);
    return cliques;
}

// Compute clique degrees
vector<int> compute_clique_degrees(const vector<set<int>>& h_cliques, int n_vertices) {
    vector<int> deg(n_vertices, 0);
    for (const auto& clique : h_cliques) {
        for (int u : clique) {
            if (u < n_vertices) {
                deg[u]++;
            }
        }
    }
    return deg;
}

// Read graph from file
pair<vector<vector<int>>, int> read_graph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return {{}, 0};
    }
    
    int u, v, max_vertex = -1;
    vector<pair<int, int>> edges;
    
    while (file >> u >> v) {
        edges.emplace_back(u, v);
        max_vertex = max({max_vertex, u, v});
    }
    
    int n_vertices = max_vertex + 1;
    vector<vector<int>> adj_list(n_vertices);
    
    for (const auto& [u, v] : edges) {
        if (u != v) { // Skip self-loops
            adj_list[u].push_back(v);
            adj_list[v].push_back(u);
        }
    }
    
    // Remove duplicates
    for (auto& neighbors : adj_list) {
        sort(neighbors.begin(), neighbors.end());
        neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }
    
    return {adj_list, n_vertices};
}

int main(int argc, char* argv[]) {
    int h = 5; // Default h value
    
    if (argc > 1) {
        h = atoi(argv[1]);
    }
    
    if (h < 2) {
        cerr << "Error: h must be at least 2" << endl;
        return 1;
    }
    
    cout << "Running with h = " << h << endl;
    
    auto start_time = chrono::high_resolution_clock::now();
    
    // Read graph
    auto [adj_list, n_vertices] = read_graph("CA-HepTh.txt");
    
    if (n_vertices == 0) {
        cerr << "Error: Empty graph" << endl;
        return 1;
    }
    
    cout << "Graph has " << n_vertices << " vertices" << endl;
    
    // Find h-cliques
    cout << "Finding " << h << "-cliques..." << endl;
    auto h_cliques = find_k_cliques(h, adj_list, n_vertices);
    cout << "Found " << h_cliques.size() << " " << h << "-cliques" << endl;
    
    if (h_cliques.empty()) {
        cout << "No " << h << "-cliques found in the graph" << endl;
        return 0;
    }
    
    // Compute clique degrees
    auto clique_degrees = compute_clique_degrees(h_cliques, n_vertices);
    
    // Find (h-1)-cliques
    cout << "Finding " << (h-1) << "-cliques..." << endl;
    auto h_minus_1_cliques = find_k_cliques(h-1, adj_list, n_vertices);
    cout << "Found " << h_minus_1_cliques.size() << " " << (h-1) << "-cliques" << endl;
    
    if (h_minus_1_cliques.empty()) {
        cout << "No " << (h-1) << "-cliques found in the graph" << endl;
        return 0;
    }
    
    // Binary search for densest subgraph
    double l = 0, u = *max_element(clique_degrees.begin(), clique_degrees.end());
    vector<int> best_subgraph;
    // Correct epsilon according to the paper
    double epsilon = 1.0 / (n_vertices * (n_vertices - 1));
    
    cout << "Starting binary search with bounds [" << l << ", " << u << "] and epsilon = " << epsilon << endl;
    
    int iteration = 0;
    while (u - l >= epsilon) {
        iteration++;
        double alpha = (l + u) / 2;
        cout << "Iteration " << iteration << ": alpha = " << alpha << endl;
        
        // Build flow network
        int node_id = 0;
        int s = node_id++; // Source
        
        // Vertices
        vector<int> vertex_nodes;
        for (int i = 0; i < n_vertices; ++i) {
            vertex_nodes.push_back(node_id++);
        }
        
        // (h-1)-cliques
        vector<int> clique_nodes;
        for (size_t i = 0; i < h_minus_1_cliques.size(); ++i) {
            clique_nodes.push_back(node_id++);
        }
        
        int t = node_id++; // Sink
        
        Dinic dinic(node_id);
        
        // Add edges from source to vertices
        for (int i = 0; i < n_vertices; ++i) {
            dinic.add_edge(s, vertex_nodes[i], clique_degrees[i]);
        }
        
        // Add edges from vertices to sink
        for (int i = 0; i < n_vertices; ++i) {
            dinic.add_edge(vertex_nodes[i], t, alpha * h);
        }
        
        // Add edges from (h-1)-cliques to vertices
        for (size_t i = 0; i < h_minus_1_cliques.size(); ++i) {
            int cn = clique_nodes[i];
            for (int v : h_minus_1_cliques[i]) {
                if (v < n_vertices) {
                    dinic.add_edge(cn, vertex_nodes[v], INF);
                }
            }
        }
        
        // Add edges from vertices to (h-1)-cliques - FIXED implementation
        for (size_t i = 0; i < h_minus_1_cliques.size(); ++i) {
            const auto& clique = h_minus_1_cliques[i];
            int cn = clique_nodes[i];
            
            // Find common neighbors of all vertices in the (h-1)-clique
            set<int> common_neighbors;
            bool first = true;
            
            for (int u : clique) {
                if (first) {
                    for (int v : adj_list[u]) {
                        common_neighbors.insert(v);
                    }
                    first = false;
                } else {
                    set<int> temp;
                    for (int v : adj_list[u]) {
                        if (common_neighbors.count(v)) {
                            temp.insert(v);
                        }
                    }
                    common_neighbors = move(temp);
                }
                
                if (common_neighbors.empty()) break;
            }
            
            // Remove vertices that are already in the clique
            for (int u : clique) {
                common_neighbors.erase(u);
            }
            
            // Add edges from vertices to (h-1)-clique
            for (int v : common_neighbors) {
                if (v < n_vertices) {
                    dinic.add_edge(vertex_nodes[v], cn, 1);
                }
            }
        }
        
        // Compute max flow and min cut
        ll flow = dinic.max_flow(s, t);
        cout << "Max flow: " << flow << endl;
        
        vector<int> cut = dinic.min_cut(s);
        
        if (cut.size() == 1 && cut[0] == s) {
            u = alpha;
            cout << "Empty cut, updating upper bound to " << u << endl;
        } else {
            l = alpha;
            
            // Extract vertices from cut
            best_subgraph.clear();
            for (int node : cut) {
                auto it = find(vertex_nodes.begin(), vertex_nodes.end(), node);
                if (it != vertex_nodes.end()) {
                    int vertex_idx = it - vertex_nodes.begin();
                    best_subgraph.push_back(vertex_idx);
                }
            }
            
            cout << "Non-empty cut with " << best_subgraph.size() << " vertices, updating lower bound to " << l << endl;
        }
    }
    
    cout << "\nDensest subgraph vertices (" << best_subgraph.size() << " vertices):" << endl;
    for (int v : best_subgraph) {
        cout << v << " ";
    }
    cout << endl;
    
    // Calculate final density
    if (best_subgraph.empty()) {
        cout << "No densest subgraph found" << endl;
        return 0;
    }
    
    // Build induced subgraph
    vector<vector<int>> induced_adj_list(n_vertices);
    set<int> in_subgraph(best_subgraph.begin(), best_subgraph.end());
    
    for (int u : best_subgraph) {
        for (int v : adj_list[u]) {
            if (in_subgraph.count(v)) {
                induced_adj_list[u].push_back(v);
            }
        }
    }
    
    // Count h-cliques in the densest subgraph
    vector<set<int>> final_cliques = find_k_cliques(h, induced_adj_list, n_vertices);
    double final_density = (double)final_cliques.size() / best_subgraph.size();
    
    cout << "Number of " << h << "-cliques in the densest subgraph: " << final_cliques.size() << endl;
    cout << "Maximum h-clique density: " << fixed << setprecision(6) << final_density << endl;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "\nExecution time: " << duration << " ms" << endl;
    
    return 0;
}
