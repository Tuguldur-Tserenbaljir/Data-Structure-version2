#include <iostream>
#include <vector>
#include <map>
//#include <list>
#include <tuple>
//#include <queue>

#include <climits>
#include <algorithm>

using namespace std;

struct Node {
    int value;
    int distance_to_parent;
    Node(int val, int dist) : value(val), distance_to_parent(dist) {}
};

//GLOBAL VARIABLES
int maximumDistance = INT_MIN;
//vector<int> currentPath, bestPath;
//vector<int> maximumPath;
//map<int, vector<int>> pathTracker; 
int rootOfMaximumPath = -1;
map<int, vector<Node>> tree;

void printTree(const map<int, vector<Node>>& tree) {
    for (const auto& pair : tree) {
        int parent = pair.first;
        const vector<Node>& children = pair.second;
        cout << "Parent " << parent << " has children: " << endl;
        for (const Node& child : children) {
            cout << "  Child " << child.value << " at distance " << child.distance_to_parent << endl;
        }
    }
}

// Function to print the root node
void printRoot(const map<int, vector<Node>>& tree) {
    //root parent saved at -1
    auto it = tree.find(-1); 
    if (it != tree.end() && !it->second.empty()) {
        // Assuming the first entry for -1 is always the root
        const Node& root = it->second.front();
        //cout << "Root Node: Value = " << root.value;
        cout<<root.distance_to_parent<<endl;
    }
}
void printCurrentPath(const vector<int>& path) {
    for (int node : path) {
        cout << node << " ";
    }
    cout << endl;
}
void deleteNode(map<int, vector<Node>>& tree, int B) {
    int parentOfB = -1;
    vector<Node>* childrenOfB = nullptr;

    // Find parent of B and B's children
    for (auto& entry : tree) {
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it) {
            if (it->value == B) {
                parentOfB = entry.first;
                childrenOfB = &entry.second;
                break;
            }
        }
        if (parentOfB != -1) break;
    }

    if (parentOfB == -1) {
        //cout << "No node B to delete " << endl;
        return; // B does not exist
    }

    // Transfer B's children to B's parent
    vector<Node> newChildren;
    for (auto& child : tree[B]) {
        newChildren.push_back(child);
    }

    // Remove B from its parent's child list
    vector<Node>& siblings = tree[parentOfB];
    for (auto it = siblings.begin(); it != siblings.end();) {
        if (it->value == B) {
            it = siblings.erase(it);
        } else {
            ++it;
        }
    }

    // Add B's children to B's parent's child list
    siblings.insert(siblings.end(), newChildren.begin(), newChildren.end());

    // Remove B's entry from the tree
    tree.erase(B);

    //cout << "Node " << B << deleted. 
}

pair<int, int> calculateMaxPath(int node, int rootDistance, vector<pair<int, int>>& childPaths) {
    int maxPropagatePath = rootDistance;  // Best path to propagate upwards.
    int globalMaxPath = rootDistance;  // Local maximum path.
    int pathRoot = node;  // Assume node is the path root initially.

    vector<int> pathsIncludingRoot;  // Store paths that include root's contribution.

    // Collect paths from children including root contribution.
    for (const auto& child : childPaths) {
        int pathIncludingRoot = child.first + rootDistance;
        pathsIncludingRoot.push_back(pathIncludingRoot);
        if (pathIncludingRoot > maxPropagatePath) {
            maxPropagatePath = pathIncludingRoot;  // Update the path to propagate.
            pathRoot = child.second;  // Root of this path is the child's root.
        }
    }

    // Handle local maximum calculations.
    if (childPaths.size() == 1) {
        // Only one child - consider root + child.
        globalMaxPath = pathsIncludingRoot[0];
        pathRoot = node;
    } else if (childPaths.size() > 1) {
        std::sort(pathsIncludingRoot.begin(), pathsIncludingRoot.end(), greater<int>());
        // Consider root + two best children if more than one child exists.
        if (pathsIncludingRoot.size() > 1) {
            int topTwoCombined = rootDistance + pathsIncludingRoot[0] + pathsIncludingRoot[1] - 2 * rootDistance;
            if (topTwoCombined > globalMaxPath) {
                globalMaxPath = topTwoCombined;
                pathRoot = node;  // Node is crucial as it links these top paths.
            }
        }
    }

    // Check if this path is the highest found so far.
    if (globalMaxPath > maximumDistance) {
        maximumDistance = globalMaxPath;
        rootOfMaximumPath = pathRoot;  // Update global root if this node has the highest path.
    }
    //cout << "The global max path for many children is: " << globalMaxPath << endl;
    //cout << "The maximumDistance is: " << maximumDistance << endl;
    //cout << endl;
    //cout << "Node: " << node << " - MaxPath: " << globalMaxPath << ", PropagatePath: " << maxPropagatePath << endl;

    return {maxPropagatePath, pathRoot};  // Return the best path for upward propagation and the root.
}








//Just more children same logic
pair<int, int> calculateMaxPathForManyChildren(int node, int rootDistance, vector<pair<int, int>>& childPaths) {
    int bestUpwardPath = rootDistance;  // This will be used for upward propagation.
    int globalMaxPath = rootDistance;  // Highest path found locally at this node.
    int pathRoot = node;  // Start with the current node as the root.

    vector<int> childPathsWithRoot;  // Paths including the root's contribution.

    // Evaluate each child's contribution, considering it for local and upward decisions.
    for (const auto& child : childPaths) {
        int pathWithRoot = child.first + rootDistance;
        childPathsWithRoot.push_back(pathWithRoot);
        if (pathWithRoot > bestUpwardPath) {
            bestUpwardPath = pathWithRoot;  // Update the best path for upward propagation.
            pathRoot = child.second;  // Update the root of the propagated path.
        }
    }

    // Handle more than two children for local maximum calculation.
    if (childPathsWithRoot.size() > 2) {
        std::sort(childPathsWithRoot.rbegin(), childPathsWithRoot.rend());  // Sort to get the largest values.
        int combinedTopTwoPaths = childPathsWithRoot[0] + childPathsWithRoot[1] - rootDistance;
        if (combinedTopTwoPaths > globalMaxPath) {
            globalMaxPath = combinedTopTwoPaths;  // Update the local maximum if this is better.
            pathRoot = node;  // Node is crucial as it links these top paths.
        }
    }

    //cout << "The global max path for many children is: " << globalMaxPath << endl;
    //cout << "The maximumDistance is: " << maximumDistance << endl;
    //cout << endl;

    // Update global maximum if necessary.
    if (globalMaxPath > maximumDistance) {
        maximumDistance = globalMaxPath;
        rootOfMaximumPath = node;  // Update global root if this node has the highest path.
    }

    return {bestUpwardPath, pathRoot};  // Return the best upward path and the root of that path.
}




pair<int, int> traverseAndComputeMaxPath(int node, int parentDistance) {
    vector<pair<int, int>> childPaths;
    for (auto& child : tree[node]) {
        auto result = traverseAndComputeMaxPath(child.value, child.distance_to_parent);
        childPaths.push_back({result.first, child.value});
    }

    //cout<<childPaths.size()<<endl; Correctly recursing children

    if(childPaths.size() > 2){
        return calculateMaxPathForManyChildren(node, parentDistance, childPaths); 
    }else{ 
        return calculateMaxPath(node, parentDistance, childPaths);
    }
}


int main(){
    // N = total nodes, O = number of operations
    int N, O;  
    cin >> N >> O;

    // Vector map for tree
    //map<int, vector<Node>> tree;
    
    int rootValue, rootDistance;
    cin >> rootValue >> rootDistance;

    // Make the actual root a child of the dummy root
    tree[-1].push_back(Node(rootValue, rootDistance));
    
    for (int i = 0; i < N; i++) {
        int P, C, D;
        cin >> P >> C >> D;  // Parent node, Child node, Distance to child
        tree[P].push_back(Node(C, D));
    }

    vector<tuple<string, int, int, int>> operations;
    string operation;

    for (int i = 0; i < O; i++) {
        cin >> operation;
        if (operation == "Delete") {
            int nodeToDelete;
            cin >> nodeToDelete;
            operations.push_back(make_tuple(operation, nodeToDelete, -1, -1));
        } else if (operation == "Check") {
            operations.push_back(make_tuple(operation, -1, -1, -1));
        } else if (operation == "Add") {
            int parent, value, distance;
            cin >> parent >> value >> distance;
            operations.push_back(make_tuple(operation, parent, value, distance));
        }
    }

    //printTree(tree);

    for (const auto& op : operations) {
        operation = get<0>(op);
        //cout<< "The operation is: "<<operation<<endl;
        if (operation == "Delete") {
            deleteNode(tree, get<1>(op));
            //Deleting correctly,Check later cause description say all children become
            //cout << "Delete function: Node " << nodeToDelete << endl;
            //printTree(tree);
        } else if (operation == "Check") {          
            //maximumDistance = INT_MIN;
            maximumDistance = INT_MIN;
            rootOfMaximumPath = -1;
            traverseAndComputeMaxPath(tree[-1].front().value,tree[-1].front().distance_to_parent);
            //cout<<result.first<<endl;
            //cout<<result.second<<endl;
            cout << "Maximum Value: " << maximumDistance << endl;
            cout << "Root of the Path: " << rootOfMaximumPath << endl;
            //cout << "Maximum Value: " << maximumDistance << endl;
            //cout << "Root of the Path: " << rootOfMaximumPath << endl;
        } else if (operation == "Add") {
            //cout << "Add function: Adding Node " << value << " under Parent " << parent << " with Distance " << distance << endl;
            tree[get<1>(op)].push_back(Node(get<2>(op), get<3>(op)));  // Adding a new node
            //printTree(tree);
        }
    }

    //print final root
    //dft(tree[-1].front().value);    

    
    maximumDistance = INT_MIN;
    rootOfMaximumPath = -1;
    traverseAndComputeMaxPath(tree[-1].front().value,tree[-1].front().distance_to_parent);
    //cout << "Maximum Value: " << maximumDistance << endl;
    cout<<"Final Root: "<<rootOfMaximumPath<<endl;
    

    //printRoot(tree);
    //Debuggin
    //printTree(tree);
    return 0;
}