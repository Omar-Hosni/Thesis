#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;



struct Node {
    int val;
    bool isLeaf;
    Node* topLeft;
    Node* topRight;
    Node* bottomLeft;
    Node* bottomRight;

    Node() : val(0), isLeaf(false), topLeft(nullptr), topRight(nullptr), bottomLeft(nullptr), bottomRight(nullptr) {}
    Node(int _val, bool _isLeaf) : val(_val), isLeaf(_isLeaf), topLeft(nullptr), topRight(nullptr), bottomLeft(nullptr), bottomRight(nullptr) {}
};

class QuadTree {
public:
    Node* constructSequential(vector<vector<int>>& grid) {
        const auto n = grid.size();
        Node* root = new Node();


        rec(root, grid, 0, 0, n);

        

        return root;
    }

private:
    void rec(Node* root, vector<vector<int>>& grid, int x, int y, int size) {
        if (sameVal(grid, x, y, size)) {
            root->isLeaf = true;
            root->val = grid[x][y];
            return;
        }

        root->topLeft = new Node();
        root->topRight = new Node();
        root->bottomLeft = new Node();
        root->bottomRight = new Node();

        int new_size = size / 2;

        rec(root->topLeft, grid, x, y, new_size);
        rec(root->topRight, grid, x, y + new_size, new_size);
        rec(root->bottomLeft, grid, x + new_size, y, new_size);
        rec(root->bottomRight, grid, x + new_size, y + new_size, new_size);
    }

    bool sameVal(vector<vector<int>>& grid, int x, int y, int size) {
        int val = grid[x][y];
        for (int i = x; i < x + size; ++i) {
            for (int j = y; j < y + size; ++j) {
                if (grid[i][j] != val) {
                    return false;
                }
            }
        }
        return true;
    }
};

void runSequentialConstruction(vector<vector<int>>& grid, QuadTree quadTree) {
    quadTree.constructSequential(grid);
}

int main() {

    cout << "4 Threads Executing Construction in Parallel" << endl;
    vector<vector<int>> grid1(50 * 1024, vector<int>(50 * 1024, 1));
    vector<vector<int>> grid2(50 * 1024, vector<int>(50 * 1024, 1));
    vector<vector<int>> grid3(50 * 1024, vector<int>(50 * 1024, 1));
    vector<vector<int>> grid4(50 * 1024, vector<int>(50 * 1024, 1));

    QuadTree quadTree;

    thread t1(runSequentialConstruction, ref(grid1), ref(quadTree));
    thread t2(runSequentialConstruction, ref(grid2), ref(quadTree));
    thread t3(runSequentialConstruction, ref(grid3), ref(quadTree));
    thread t4(runSequentialConstruction, ref(grid4), ref(quadTree));

    auto start_time = chrono::high_resolution_clock::now();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    cout << "Time taken for parallel construction of 4 trees: " << duration.count() << " microseconds" << endl;

    return 0;
}
