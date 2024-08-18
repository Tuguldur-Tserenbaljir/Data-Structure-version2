#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include <limits>
#include <sstream>
#include <tuple>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int destination, distance, trafficCapacity;
};

struct DeliveryMan {
    int location;
    //bool available;
    //int totalDistance = 0;
};

struct VertexDeliveryInfo {
    vector<DeliveryMan> deliveryMen;
    int availableCount = 0;  // Tracks the number of available drivers at this vertex
};

// Structure to hold order details
struct OrderDetails {
    int pickupLocation;
    list<int> pathToRestaurant;  // Path taken from driver to restaurant
    list<int> pathToUser;        // Path taken from restaurant to user
    int trafficSpace;
    int totalTravelDistance; //This is driver to resturaunt distance
    bool isWaiting; // Indicates if the order is waiting for a path to clear
    int destination;
};

// Map from vertex ID to a list of DeliveryMen
map<int, VertexDeliveryInfo> deliveryMenAtVertex;
map<int, OrderDetails> activeOrders;  // Map from order ID to OrderDetails
map<int, vector<Edge>> graph;
map<int, vector<Edge>> initial_state_of_graph;
//map<int, DeliveryMan> deliveryMen;

void addEdge(int s, int d, int dis, int t) {
    graph[s].push_back({d, dis, t});
    graph[d].push_back({s, dis, t});
}

void placeDeliveryMan(int vertex, int count) {
    // Access or create the vertex info
    VertexDeliveryInfo& vertexDeliveryInfo = deliveryMenAtVertex[vertex];

    // Add new delivery men to the vertex
    for (int i = 0; i < count; ++i) {
        vertexDeliveryInfo.deliveryMen.push_back(DeliveryMan{vertex});
        vertexDeliveryInfo.availableCount++;  // Increment the count for each new available delivery man
    }
}

int calculatePathDistance(const list<int>& path) {
    if (path.size() < 2) return 0;  // If the path has less than two nodes, the distance is zero.

    int distance = 0;
    auto it = path.begin();
    int prev = *it;
    ++it;
    while (it != path.end()) {
        int current = *it;
        // Find the edge between prev and current to get the distance
        for (const Edge& edge : graph[prev]) {
            if (edge.destination == current) {
                distance += edge.distance;
                break;
            }
        }
        prev = current;
        ++it;
    }
    return distance;
}

void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void insertionSortCandidates(vector<pair<int, pair<int, list<int>>>>& candidates) {
    for (int i = 1; i < candidates.size(); i++) {
        auto key = candidates[i];
        int j = i - 1;

        // Compare first by distance, and if they are the same, by the starting location of the driver
        while (j >= 0 && (candidates[j].second.first > key.second.first ||
                          (candidates[j].second.first == key.second.first && candidates[j].first > key.first))) {
            candidates[j + 1] = candidates[j];
            j--;
        }
        candidates[j + 1] = key;
    }
}

void updateTrafficCapacity(const list<int>& path, int ts, bool increase = false) {
    auto it = path.begin();
    int u = *it;
    ++it;
    while (it != path.end()) {
        int v = *it;
        ++it;
        // Update the traffic capacity for edge u -> v
        for (auto& edge : graph[u]) {
            if (edge.destination == v) {
                if (increase) {
                    edge.trafficCapacity += ts;
                } else {
                    edge.trafficCapacity -= ts;
                    //cout<< "The global traffic capacity is:"<<edge.trafficCapacity<<endl;
                }
                // No break here; we want to find the reverse path as well
            }
        }
        // Update the traffic capacity for edge v -> u (the reverse direction)
        for (auto& edge : graph[v]) {
            if (edge.destination == u) {
                if (increase) {
                    edge.trafficCapacity += ts;
                } else {
                    edge.trafficCapacity -= ts;
                }
                // No break here; we might still need to continue updating if there are parallel edges
            }
        }
        u = v;
    }
    //ts = edge.trafficCapacity;
}

bool canAccommodateTraffic(const list<int>& path, int ts) {
    auto it = path.begin();
    int u = *it;
    ++it;
    while (it != path.end()) {
        int v = *it;
        ++it;
        for (const auto& edge : graph[u]) { //current graph
            if (edge.destination == v && edge.trafficCapacity < ts) {
                return false;
            }
        }
        u = v;
    }
    return true;
}

void print_current_GraphState() {
    cout << "Current Graph State:" << endl;
    for (const auto& node : graph) {
        int from = node.first;
        const auto& edges = node.second;
        for (const auto& edge : edges) {
            cout << "Edge from " << from << " to " << edge.destination
                 << " - Distance: " << edge.distance
                 << ", Traffic Capacity: " << edge.trafficCapacity << endl;
        }
    }
}

void print_initial_GraphState() {
    cout << "Initial Graph State:" << endl;
    for (const auto& node : initial_state_of_graph) {
        int from = node.first;
        const auto& edges = node.second;
        for (const auto& edge : edges) {
            cout << "Edge from " << from << " to " << edge.destination
                 << " - Distance: " << edge.distance
                 << ", Traffic Capacity: " << edge.trafficCapacity << endl;
        }
    }
}

void printPathToUser(int id) {
    if (activeOrders.find(id) == activeOrders.end()) {
        cout << "Order ID not found." << endl;
        return;
    }

    const auto& orderDetails = activeOrders[id];
    cout << "Path to User for Order " << id << ": ";
    for (int node : orderDetails.pathToUser) {
        cout << node << " ";
    }
    cout << endl;
}

void printPathToRestaurant(int id) {
    if (activeOrders.find(id) == activeOrders.end()) {
        cout << "Order ID not found." << endl;
        return;
    }

    const auto& orderDetails = activeOrders[id];
    cout << "Path to Restaurant for Order " << id << ": ";
    for (int node : orderDetails.pathToRestaurant) {
        cout << node << " ";
    }
    cout << endl;
}

void printDeliveryMenCount() {
    cout << "Delivery Men Count by Location:" << endl;
    for (const auto& pair : deliveryMenAtVertex) {
        cout << "Location " << pair.first << ": " << pair.second.availableCount << " available drivers." << endl;
    }
}

void processOrder(int id, int src, int ts) {
    int bestDriver = -1;
    int shortestDist = INF;
    list<int> bestPath;

    map<int, tuple<int, list<int>>> visited;  // Maps each node to its shortest known distance and the path taken to reach it
     priority_queue<pair<pair<int, int>, pair<int, list<int>>>, vector<pair<pair<int, int>, pair<int, list<int>>>>, greater<>> pq;

    // Initialize with path length = 0, distance = 0, starts with the source node
    pq.push({{0, 0}, {src, {src}}});
    visited[src] = make_tuple(0, list<int>{src});
    //cout << "Initialized starting node " << src << " with path length 0 and distance 0." << endl;

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();
        int currentPathLength = current.first.first;
        int currentDist = current.first.second;
        int currentNode = current.second.first;
        list<int> currentPath = current.second.second;

        // Debug output
        //cout << "Current node: " << currentNode << ", Path length: " << currentPathLength << ", Distance: " << currentDist << ", Path: ";
        /*
        for (auto node : currentPath) {
            cout << node << " ";
        }
        cout << endl;
*/
        // Check if the current node has available drivers
        if (deliveryMenAtVertex[currentNode].availableCount > 0 && (bestDriver == -1 || currentDist < shortestDist)) {
            bestDriver = currentNode;
            shortestDist = currentDist;
            bestPath = currentPath;
            deliveryMenAtVertex[currentNode].availableCount--;
            //cout << "Driver at " << currentNode << " set as unavailable. Path found!" << endl;
            break;  // Stop the loop as soon as a valid driver is found
        }

        // Expand to adjacent nodes
        for (const Edge& edge : graph[currentNode]) {
            if (edge.trafficCapacity < ts) continue;  // Check traffic capacity
            int nextNode = edge.destination;
            int nextDist = currentDist + edge.distance;
            int nextPathLength = currentPathLength + 1;

            // Update or add paths if it leads to a better path
            if (!visited.count(nextNode) || nextDist < get<0>(visited[nextNode])) {
                currentPath.push_back(nextNode);
                visited[nextNode] = make_tuple(nextDist, list<int>(currentPath.begin(), currentPath.end()));
                pq.push({{nextPathLength, nextDist}, {nextNode, get<1>(visited[nextNode])}});
                //cout << "Updating path to node " << nextNode << " with path length " << nextPathLength << " and distance " << nextDist << "." << endl;
                currentPath.pop_back();  // Remove the last node to maintain correct path for subsequent iterations
            }
        }
    }
    //print_current_GraphState();
    //printPathToRestaurant(id);
    if (bestDriver != -1) {
        updateTrafficCapacity(bestPath, ts);
        list<int> existingPathToUser;
        if (activeOrders.find(id) != activeOrders.end()) {
            existingPathToUser = activeOrders[id].pathToUser;
        }
        activeOrders[id] = {src, bestPath, existingPathToUser, ts, shortestDist, false, activeOrders[id].destination};
        //cout << "Order " << id << " from: " << bestDriver << ", Distance: " << shortestDist << endl;
        cout << "Order " << id << " from: " << bestDriver <<endl;
        //printPathToRestaurant(id);
        //printDeliveryMenCount();
    } else {
        activeOrders.erase(id);
        cout << "Just walk. T-T" << endl;

    }
    //print_current_GraphState();
}

bool attemptPathfinding(int id, int dst, const map<int, vector<Edge>>& graphToUse, list<int>& pathToUse) {
    map<int, pair<int, list<int>>> visited;
    priority_queue<pair<int, pair<int, list<int>>>, vector<pair<int, pair<int, list<int>>>>, greater<>> pq;
    int pickupLocation = activeOrders[id].pickupLocation;
    pq.push({0, {pickupLocation, {pickupLocation}}});
    visited[pickupLocation] = {0, {pickupLocation}};

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();
        int currentDist = current.first;
        int currentNode = current.second.first;
        list<int> currentPath = current.second.second;

        for (const Edge& edge : graphToUse.at(currentNode)) {
            if (edge.trafficCapacity < activeOrders[id].trafficSpace) continue;
            int nextNode = edge.destination;
            int nextDist = currentDist + edge.distance;

            if (!visited.count(nextNode) || visited[nextNode].first > nextDist) {
                list<int> newPath = currentPath;
                newPath.push_back(nextNode);  // Correct method to add to the path

                visited[nextNode] = {nextDist, newPath};
                pq.push({nextDist, {nextNode, newPath}});

                if (nextNode == dst) {
                    pathToUse = newPath; // Ensure the pathToUse is updated with the complete path
                    return true; // Return true as a path has been found
                }
            }
        }
    }

    return false; // Return false if no path is found
}



void processDrop(int id, int dst, bool from_complete_function = false) {
    activeOrders[id].destination = dst; // Set the new destination for the order.
    if(!from_complete_function){
        updateTrafficCapacity(activeOrders[id].pathToRestaurant, activeOrders[id].trafficSpace, true);
    }
    
    bool pathFound = false;
    list<int> potentialPath;
    pathFound = attemptPathfinding(id, dst, graph, potentialPath);

    if (pathFound) {
        updateTrafficCapacity(potentialPath, activeOrders[id].trafficSpace);
        
        int totalTravelDistance = calculatePathDistance(potentialPath);
        totalTravelDistance += activeOrders[id].totalTravelDistance;
        cout << "Order " << id << " distance: " << totalTravelDistance << endl;
        //print_current_GraphState();
        activeOrders[id].pathToUser = potentialPath; // Update the path to user
    } else {
        // No path in the current state, try initial state graph
        //pathFound = attemptPathfinding(id, dst, initial_state_of_graph, potentialPath);
        cout << "No Way Home" << endl;
        //print_current_GraphState();
        activeOrders[id].isWaiting = true;
    }
    //printPathToUser(id);
    //print_current_GraphState();
}


void processComplete(int id) {
    /*
    // Check if the order exists
    if (activeOrders.find(id) == activeOrders.end()) {
        cout << "Order ID " << id << " not found." << endl;
        return;
    }
*/
    OrderDetails& order = activeOrders[id];

    // Restore traffic capacities for both paths used by the order
    //updateTrafficCapacity(order.pathToRestaurant, order.trafficSpace, true);
    updateTrafficCapacity(order.pathToUser, order.trafficSpace, true);

    // Logically place the delivery man back at the destination
    placeDeliveryMan(activeOrders[id].destination, 1);
    //cout<<"The destination is: "<<order.destination<<endl;
    //cout<<"Adding back the driver"<<endl;
    //printDeliveryMenCount();
    //cout << "Order " << id << " completed. Delivery man is now available at location " << order.destination << "." << endl;

    // Remove the completed order from the active orders
    activeOrders.erase(id);

    // Sort and process any waiting orders
    vector<int> toReprocess;
    for (const auto& entry : activeOrders) {
        if (entry.second.isWaiting) {
            toReprocess.push_back(entry.first);
        }
    }

    // Sort the IDs in ascending order to process from the smallest to largest
    insertionSort(toReprocess);

    // Attempt to process each waiting order
    for (int waitingId : toReprocess) {
        auto& details = activeOrders[waitingId];
        if (details.isWaiting) {
            processDrop(waitingId, details.destination,true);
        }
    }
    
    //cout<<"The order is not working lol bro"<<endl;
    //print_current_GraphState();
}


int main() {
    int V, E, D;
    cin >> V >> E >> D; //D = driver count

    // Initialize all vertices in the graph
    for (int i = 1; i <= V; ++i) {
        deliveryMenAtVertex[i] = VertexDeliveryInfo();
    }

    for (int i = 0; i < D; i++) {
        string cmd;
        int v, c;
        std::cin >> cmd >> v >> c;
        placeDeliveryMan(v, c);
    }
    //printDeliveryMenCount();
    for (int i = 0; i < E; ++i) {
        string cmd;
        int s, d, dis, t;
        std::cin >> cmd >> s >> d >> dis >> t;
        addEdge(s, d, dis, t);
    }

    //Make copy of graph
    initial_state_of_graph = graph;


    int C;
    std::cin >> C; // Read the number of commands
    std::cin.ignore(); // Ignore newline before reading commands

    vector<string> commands(C);
    for (int i = 0; i < C; ++i) {
        std::getline(cin, commands[i]); // Read each command into the vector
    }

    // Process each command from the vector
    for (const string& command : commands) {
        stringstream ss(command);
        string cmdType;
        ss >> cmdType;
        //printDeliveryMenCount();
        if (cmdType == "Order") {
            int id, src, ts;
            ss >> id >> src >> ts;
            processOrder(id, src, ts);
        } else if (cmdType == "Drop") {
            int id, dst;
            ss >> id >> dst;
            processDrop(id, dst);
        } else if (cmdType == "Complete") {
            int id;
            ss >> id;
            processComplete(id);
        }
    }
    //cout<<endl;
    //printGraphState();
    //printDeliveryMenCount();
}
