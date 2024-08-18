#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : value(x), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    TreeNode* root;

    void insertHelper(TreeNode*& node, int parent_value, int value, bool& inserted) {
        if (!node) return;
        if (node->value == parent_value) {
            if (!node->left) {
                node->left = new TreeNode(value);
                inserted = true;
            } else if (!node->right) {
                node->right = new TreeNode(value);
                inserted = true;
            }
            return;
        }
        if (!inserted) {
            insertHelper(node->left, parent_value, value, inserted);
            insertHelper(node->right, parent_value, value, inserted);
        }
    }

    //Printing Functions
    void preorder(TreeNode* node) {
        if (!node) return;
        cout << node->value << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void inorder(TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->value << " ";
        inorder(node->right);
    }

    void postorder(TreeNode* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->value << " ";
    }

    //Delete function in private
    TreeNode* detachSubtree(TreeNode*& node, int value) {
        if (!node) return nullptr;
        if (node->value == value) {
            TreeNode* detached = node;
            node = nullptr; // Detach the node
            return detached;
        }
        TreeNode* leftResult = detachSubtree(node->left, value);
        if (leftResult) return leftResult;
        TreeNode* rightResult = detachSubtree(node->right, value);
        return rightResult;
    }

    void clear(TreeNode*& node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }
    //Find max function in private
    int findMax(TreeNode* node) {
        if (!node) return INT_MIN;
        int leftMax = findMax(node->left);
        int rightMax = findMax(node->right);
        return max(node->value, max(leftMax, rightMax));
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        clear(root);
    }

    void insert(int parent_value, int value) {
        if (!root) {
            root = new TreeNode(value);
        } else {
            bool inserted = false;
            insertHelper(root, parent_value, value, inserted);
        }
    }

    void deleteValue(int value) {
        TreeNode* subtree = detachSubtree(root, value);
        clear(subtree);
    }

    void print(string mode) {
        if (mode == "pre") {
            preorder(root);
        } else if (mode == "in") {
            inorder(root);
        } else if (mode == "post") {
            postorder(root);
        }
        cout << endl;
    }

    void printMax() {
        int maxValue = findMax(root);
        if (maxValue != INT_MIN) { // Check if the tree is not empty
            cout << maxValue << endl;
        }
    }
};

int main() {
    int N, ops;
    cin >> N >> ops;
    vector<BinaryTree> trees(N);

    string command;
    while (ops > 0) {
        cin >> command;
        if (command == "Insert") {
            int index, parent_value, value;
            cin >> index >> parent_value >> value;
            trees[index].insert(parent_value, value);
        } else if (command == "Delete") {
            int index, value;
            cin >> index >> value;
            trees[index].deleteValue(value); //Correct
        } else if (command == "Print") {
            int index;
            string mode;
            cin >> index >> mode;
            trees[index].print(mode);
        } else if (command == "Max") {
            int index;
            cin >> index;
            trees[index].printMax();
        } else if (command == "Merge") {
            int destination,from,value;
            cin >> destination>>from>>value;
            //do later
        } else if (command == "Disjoint") {
            int index,value;
            cin >> index>>value;
            //do later
        }
        ops--;
    }

    // Print all trees in inorder traversal
    for (int i = 0; i < trees.size(); i++) {
        cout << "Tree " << i << endl;
        trees[i].print("in");
    }
}