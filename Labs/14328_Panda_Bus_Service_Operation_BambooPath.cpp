#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>
//#include <algorithm>
using namespace std;

const long long INF = numeric_limits<long long>::max();

vector<long long> dijkstra(const vector<vector<pair<int, int>>>& graph, int start) {
    
    int n = graph.size();
    
    vector<long long> dist(n, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    //At start
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        //auto [current_distance, current_node] = pq.top();
        //Find current node, distance
        int current_node = pq.top().second;
        int current_distance = pq.top().first;

        pq.pop();

        //check if to prevent processing already finalized nodes
        if (current_distance > dist[current_node])
            continue;

        //Each vertex v
        for (const auto& edge : graph[current_node]) {
            int neighbor = edge.first;
            int weight = edge.second;
            long long new_distance = current_distance + weight;
            
            //Check for shorter path
            if (new_distance < dist[neighbor]) {
                dist[neighbor] = new_distance;
                pq.push({new_distance, neighbor});
            }
        }
    }
    /*
    for(int i =0;i<n;i++){
        cout<<dist[i]<<endl;
    }*/

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;//vertex,edges
    
    vector<vector<pair<int, int>>> graph(n);
    int from, to, weight;

    //from, to, weight
    for (int i = 0; i < m; i++) {
        cin >> from >> to >> weight;
        graph[from].push_back({to, weight});
    }
    
    int t;
    cin >> t;
    int A, B, dest;
    
    if (t == 1) {// A to Big City
        cin >> A >> dest;
    } else if (t == 2) {// A,B to Big City
        cin >> A >> B >> dest;
    }
    
    long long result = 0;
    //cout<<"The infinity is:"<<INF<<endl;
    //result might be 1 <= result <= 100000
    if (t == 1) {
        vector<long long> dist_from_A = dijkstra(graph, A);
        result = dist_from_A[dest];
    } else if (t == 2) {
        //cout<<"A result are:"<<endl;
        vector<long long> dist_from_A = dijkstra(graph, A);
        //cout<<"B result are:"<<endl;
        vector<long long> dist_from_B = dijkstra(graph, B);

        //cout<<"The dist from A is: " <<dist_from_A<<endl;
        //cout<<"The dist from B is: " <<dist_from_B<<endl;
        if (dist_from_A[dest] < INF && dist_from_B[dest] < INF) {
            //cout<< "The A destination is: "<<dist_from_A[dest]<<endl;
            //cout<< "The A destination is: "<<dist_from_A[dest-1]<<endl;
            //cout<< "The B destination is: "<<dist_from_B[dest]<<endl;
            result = dist_from_A[dest] + dist_from_B[dest];
        } else {
            result = INF;
        }
    }
    if (result == INF) {
        cout << "-1\n";
    } else {
        cout << result << "\n";
    }
    
    return 0;
}