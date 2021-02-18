#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono; 

class node {
    public:
        vector<vector<int>> boardState;
        int fCost;
        int depth;
        int zeroRow;
        int zeroColumn;
        node();
        node(vector<vector<int>>);
        node(vector<vector<int>>, int, int);
        // bool operator<(const node& n1, const node& n2) const {
        //     return n1.fCost > n2.fCost;
        // }
};

node::node() {
    fCost = 0;
    depth = 0;
    zeroRow = 0;
    zeroColumn = 0;
}

node::node(vector<vector<int>> currBoard) {
    boardState = currBoard;
    for(int i = 0; i < boardState.size(); i++) {
        for(int j = 0; j < boardState.size(); j++) {
            if(boardState[i][j] == 0) {
                zeroRow = i;
                zeroColumn = j;
                break;
            }
        }
    }
    fCost = 0;
    depth = 0;
}

node::node(vector<vector<int>> currBoard, int fCost, int depth) {
    boardState = currBoard;
    for(int i = 0; i < boardState.size(); i++) {
        for(int j = 0; j < boardState.size(); j++) {
            if(boardState[i][j] == 0) {
                zeroRow = i;
                zeroColumn = j;
                break;
            }
        }
    }
    this->fCost = fCost;
    this->depth = depth;
}

bool operator<(const node& n1, const node& n2) {
    return n1.fCost > n2.fCost;
}

class puzzleSolver {
    private:
        vector<vector<int>> gameBoard {
            {1, 3, 6},
            {5, 0, 2},
            {4, 7, 8}
        };
        int customBoard = 0;
        int algorithm = 0;
        int maxQueueSize = 0;
        int totalNodes = 0;
    public:
        void driverFunction();
        void promptBoardType();
        void initializeBoard();
        void initializeRowHelper(const string& input, int row);
        void algorithmChoice();
        bool generalSearch();
        int heuristic(const vector<vector<int>>&);
        int numMisplacedTiles(const vector<vector<int>>&);
        string matrixToString(const vector<vector<int>>&);
        bool isSolution(const vector<vector<int>>&);
        void showGameBoard(const vector<vector<int>>&);
};

void puzzleSolver::promptBoardType() {
    cout << "Welcome to Timothy Koo's 8-puzzle problem solver." << endl;
    cout << "Type \'1\' to use a default puzzle, or \'2\' to enter your own puzzle." << endl;
    cin >> customBoard;
}

void puzzleSolver::initializeBoard() {
    // Custom board
    if(customBoard == 2) {
        // Ask user for board elements (does not validate input)
        string input = "";
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        cout << "Enter the first row, use space or tabs between numbers" << endl;
        getline(cin, input);
        getline(cin, input);
        initializeRowHelper(input, 0);
        cout << "Enter the second row, use space or tabs between numbers" << endl;
        getline(cin, input);
        initializeRowHelper(input, 1);
        cout << "Enter the third row, use space or tabs between numbers" << endl;
        getline(cin, input);
        initializeRowHelper(input, 2);
    }
    // Otherwise, default board and do nothing
}

void puzzleSolver::initializeRowHelper(const string& input, int row) {
    stringstream rowElements(input);
    int num = 0;
    int i = 0;
    while(rowElements >> num) {
        gameBoard[row].at(i++) = num;
    }
}

void puzzleSolver::algorithmChoice() {
    cout << "Enter your choice of algorithm" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile heuristic" << endl;
    cout << "3. A* with the Manhattan distance heuristic" << endl;
    cin >> algorithm;
}

bool puzzleSolver::generalSearch() {
    auto start = high_resolution_clock::now();
    priority_queue<node> nodes;
    unordered_set<string> boardSet;
    node firstNode(gameBoard);
    int expandedNodes = 0;
    int maxQueueSize = 0;

    nodes.push(firstNode);

    while(!nodes.empty()) {
        node currNode = nodes.top();
        nodes.pop();

        cout << "Current matrix with cost " << currNode.fCost << endl;
        showGameBoard(currNode.boardState);
        
        if(isSolution(currNode.boardState)) {
            cout << "Goal!" << endl;
            cout << "To solve this problem the search algorithm expanded a total of " << expandedNodes << " nodes." << endl;
            cout << "The maximum number of nodes in the queue at any one time was " << maxQueueSize << endl;
            cout << "The depth of the goal node was " << currNode.depth << endl;
            return true;
        }

        // Move up
        if(currNode.zeroRow > 0) {
            node temp = currNode;
            int row = temp.zeroRow;
            int column = temp.zeroColumn;
            temp.boardState[row][column] = temp.boardState[row-1][column];
            temp.boardState[row-1][column] = 0;
            string matrixString = matrixToString(temp.boardState);
            if(boardSet.find(matrixString) == boardSet.end()) {
                boardSet.insert(matrixString);
                temp.zeroRow = row - 1;
                temp.depth += 1;
                temp.fCost += 1 + heuristic(temp.boardState);
                nodes.push(temp);
                expandedNodes++;
            }
        }
        // Move right
        if(currNode.zeroColumn < 2) {
            node temp = currNode;
            int row = temp.zeroRow;
            int column = temp.zeroColumn;
            temp.boardState[row][column] = temp.boardState[row][column+1];
            temp.boardState[row][column+1] = 0;
            string matrixString = matrixToString(temp.boardState);
            if(boardSet.find(matrixString) == boardSet.end()) {
                temp.zeroColumn = column + 1;
                temp.depth += 1;
                temp.fCost += 1 + heuristic(temp.boardState);
                nodes.push(temp);
                expandedNodes++;
            }
        }
        // Move down
        if(currNode.zeroRow < 2) {
            node temp = currNode;
            int row = temp.zeroRow;
            int column = temp.zeroColumn;
            temp.boardState[row][column] = temp.boardState[row+1][column];
            temp.boardState[row+1][column] = 0;
            string matrixString = matrixToString(temp.boardState);
            if(boardSet.find(matrixString) == boardSet.end()) {
                temp.zeroRow = row + 1;
                temp.depth += 1;
                temp.fCost += 1 + heuristic(temp.boardState);
                nodes.push(temp);
                expandedNodes++;
            }
        }
        // Move left
        if(currNode.zeroColumn > 0) {
            node temp = currNode;
            int row = temp.zeroRow;
            int column = temp.zeroColumn;
            temp.boardState[row][column] = temp.boardState[row][column-1];
            temp.boardState[row][column-1] = 0;
            string matrixString = matrixToString(temp.boardState);
            if(boardSet.find(matrixString) == boardSet.end()) {
                temp.zeroColumn = column - 1;
                temp.depth += 1;
                temp.fCost += 1 + heuristic(temp.boardState);
                nodes.push(temp);
                expandedNodes++;
            }
        }

        if(nodes.size() > maxQueueSize) {
            maxQueueSize = nodes.size();
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        if(duration.count() > 6e+7) {
            return false;
        }
    }

    return false;
}

int puzzleSolver::heuristic(const vector<vector<int>>& board) {
    if(algorithm == 2) {
        return numMisplacedTiles(board);
    }
    else if(algorithm == 3) {
        // calculate manhattan distance
    }
    // for uniform cost search
    return 0;
}

int puzzleSolver::numMisplacedTiles(const vector<vector<int>>& board) {
    int correctNum = 1;
    int tiles = 0;

    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            if(board[i][j] != correctNum) {
                tiles++;
            }
            correctNum++;
        }
    }

    return tiles;
}

string puzzleSolver::matrixToString(const vector<vector<int>>& board) {
    string nums = "";
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            nums.push_back('0' + board[i][j]);
        }
    }
    return nums;
}

bool puzzleSolver::isSolution(const vector<vector<int>>& currBoard) {
    const vector<vector<int>> solution {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };
    return (currBoard == solution ? true : false);
}

void puzzleSolver::showGameBoard(const vector<vector<int>>& board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void puzzleSolver::driverFunction() {
    promptBoardType();
    initializeBoard();
    algorithmChoice();
    if(!generalSearch()) {
        cout << "Search failed :(" << endl;
    }
}


int main() {
    puzzleSolver ps;
    ps.driverFunction();

    return 0;
}