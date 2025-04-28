#include <bits/stdc++.h>
#include <chrono>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

// ------------------------ Flow Network (Dinic) ------------------------

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

    ll dfs(int u, int t, ll pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int& cid = ptr[u]; cid < (int)adj[u].size(); cid++) {
            int id = adj[u][cid];
            Edge& e = edges[id];
            if (e.cap - e.flow > 0 && level[e.v] == level[u] + 1) {
                ll tr = dfs(e.v, t, min(pushed, e.cap - e.flow));
                if (tr == 0) continue;
                e.flow += tr;
                edges[id ^ 1].flow -= tr;
                return tr;
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

        vector<int> cut;
        for (int i = 0; i < n; i++) {
            if (visited[i]) cut.push_back(i);
        }
        return cut;
    }
};

// ------------------------ Graph Utilities ------------------------

vector<vector<int>> read_graph(const string& filename, int& n) {
    ifstream file(filename);
    int u, v;
    vector<pair<int, int>> edges;
    int max_node = 0;
    while (file >> u >> v) {
        edges.push_back({u, v});
        max_node = max(max_node, max(u, v));
    }
    n = max_node + 1;
    vector<vector<int>> adj(n);
    for (auto& e : edges) {
        if (e.first != e.second) {
            adj[e.first].push_back(e.second);
            adj[e.second].push_back(e.first);
        }
    }
    return adj;
}

vector<int> read_core(const string& filename) {
    ifstream file(filename);
    int node, core;
    vector<pair<int, int>> entries;
    while (file >> node >> core) {
        entries.push_back({node, core});
    }
    int max_node = 0;
    for (auto& e : entries) max_node = max(max_node, e.first);
    vector<int> core_number(max_node + 1, 0);
    for (auto& e : entries) {
        core_number[e.first] = e.second;
    }
    return core_number;
}

vector<set<int>> find_cliques(int h, const vector<vector<int>>& adj) {
    vector<set<int>> cliques;
    vector<int> temp;
    function<void(int, int)> dfs = [&](int start, int depth) {
        if (depth == h) {
            cliques.push_back(set<int>(temp.begin(), temp.end()));
            return;
        }
        for (int i = start; i < (int)adj.size(); i++) {
            bool ok = true;
            for (int v : temp) {
                if (!binary_search(adj[v].begin(), adj[v].end(), i)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                temp.push_back(i);
                dfs(i + 1, depth + 1);
                temp.pop_back();
            }
        }
    };
    dfs(0, 0);
    return cliques;
}

// ------------------------ Main CoreExact Algorithm ------------------------

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int h = 3; // default
    if (argc > 1) {
        h = atoi(argv[1]);
    }

    int n;
    auto adj = read_graph("as733_edges.txt", n);
    auto core = read_core("core_as733.txt");

    int kmax = *max_element(core.begin(), core.end());
    cout << "kmax = " << kmax << endl;

    set<int> core_vertices;
    for (int i = 0; i < n; i++) {
        if (core[i] == kmax) core_vertices.insert(i);
    }

    if (core_vertices.empty()) {
        cout << "No vertices with core number kmax" << endl;
        return 0;
    }
    auto start = chrono::high_resolution_clock::now();
    // Induce subgraph
    vector<int> mapping(n, -1);
    vector<int> reverse_map;
    int id = 0;
    for (int v : core_vertices) {
        mapping[v] = id++;
        reverse_map.push_back(v);
    }
    vector<vector<int>> induced(id);
    for (int u : core_vertices) {
        for (int v : adj[u]) {
            if (core_vertices.count(v)) {
                induced[mapping[u]].push_back(mapping[v]);
            }
        }
    }
    for (auto& vec : induced) {
        sort(vec.begin(), vec.end());
    }

    // Find all h-cliques
    auto h_cliques = find_cliques(h, induced);

    if (h_cliques.empty()) {
        cout << "No h-cliques found" << endl;
        return 0;
    }

    vector<int> degree(id, 0);
    for (auto& clique : h_cliques) {
        for (int v : clique) degree[v]++;
    }

    double l = 0, u = *max_element(degree.begin(), degree.end());
    double eps = 1.0 / (id * id);

    vector<int> best_subgraph;

    while (u - l > eps) {
        double alpha = (l + u) / 2;

        Dinic dinic(id + 2);
        int s = id, t = id + 1;

        for (int v = 0; v < id; v++) {
            dinic.add_edge(s, v, degree[v]);
            dinic.add_edge(v, t, alpha * h);
        }

        for (auto& clique : h_cliques) {
            for (int v : clique) {
                for (int u : clique) {
                    if (v != u) {
                        dinic.add_edge(v, u, INF);
                    }
                }
            }
        }

        ll flow = dinic.max_flow(s, t);

        auto cut = dinic.min_cut(s);
        if (cut.size() == 1) {
            u = alpha;
        } else {
            l = alpha;
            best_subgraph.clear();
            for (int v : cut) {
                if (v < id) best_subgraph.push_back(v);
            }
        }
    }

    cout << "\nBest subgraph size: " << best_subgraph.size() << endl;
    for (int v : best_subgraph) {
        cout << reverse_map[v] << " ";
    }
    cout << endl;

    set<int> subgraph_set(best_subgraph.begin(), best_subgraph.end());

    int final_cliques = 0;
    for (auto& clique : h_cliques) {
        bool all_inside = true;
        for (int v : clique) {
            if (!subgraph_set.count(v)) {
                all_inside = false;
                break;
            }
        }
        if (all_inside) final_cliques++;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    double density = 1.0 * final_cliques / best_subgraph.size();
    cout << "Number of " << h << "-cliques in the densest subgraph: " << final_cliques << endl;
    cout << "Maximum h-clique density: " << fixed << setprecision(6) << density << endl;
    cout << "Time taken: " << duration.count() << " seconds." << endl;

    return 0;
}

