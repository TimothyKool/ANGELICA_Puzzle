#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <cmath>

using namespace std;
using namespace std::chrono; 

class node {
    public:
        vector<vector<char>> boardState;
        int fCost;
        int depth;
        int zeroRow;
        int zeroColumn;
        node();
        node(vector<vector<char>>);
        node(vector<vector<char>>, int, int);
};

node::node() {
    fCost = 0;
    depth = 0;
    zeroRow = 0;
    zeroColumn = 0;
}

node::node(vector<vector<char>> currBoard) {
    boardState = currBoard;
    for(int i = 0; i < boardState.size(); i++) {
        for(int j = 0; j < boardState.size(); j++) {
            if(boardState[i][j] == '0') {
                zeroRow = i;
                zeroColumn = j;
                break;
            }
        }
    }
    fCost = 0;
    depth = 0;
}

node::node(vector<vector<char>> currBoard, int fCost, int depth) {
    boardState = currBoard;
    for(int i = 0; i < boardState.size(); i++) {
        for(int j = 0; j < boardState.size(); j++) {
            if(boardState[i][j] == '0') {
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
        vector<vector<char>> gameBoard {
            {'E', 'A', 'G'},
            {'N', '0', 'L'},
            {'C', 'Z', 'I'}
        };
        int customBoard = 0;
        int algorithm = 0;
        int maxQueueSize = 0;
        int totalNodes = 0;
    public:
        void driverFunction();
        void promptBoardType();
        void initializeBoard();
        void initializeRowHelper(const string& input1, const string& input2, const string& input3);
        void algorithmChoice();
        bool generalSearch();
        int heuristic(const vector<vector<char>>&);
        int numMisplacedTiles(const vector<vector<char>>&);
        int manhattanDistance(const vector<vector<char>>&);
        string matrixToString(const vector<vector<char>>&);
        bool isSolution(const vector<vector<char>>&);
        void showGameBoard(const vector<vector<char>>&);
};

void puzzleSolver::promptBoardType() {
    cout << "Welcome to Timothy Koo's Angelica-puzzle problem solver." << endl;
    cout << "Type \'1\' to use a default puzzle, or \'2\' to enter your own puzzle." << endl;
    cin >> customBoard;
}

void puzzleSolver::initializeBoard() {
    // Custom board
    if(customBoard == 2) {
        // Ask user for board elements (does not validate input)
        string row1 = "";
        string row2 = "";
        string row3 = "";
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        cout << "Enter the first row, use space or tabs between letters" << endl;
        getline(cin, row1);
        getline(cin, row1);
        cout << "Enter the second row, use space or tabs between letters" << endl;
        getline(cin, row2);
        cout << "Enter the third row, use space or tabs between letters" << endl;
        getline(cin, row3);
        initializeRowHelper(row1, row2, row3);
    }
    // Otherwise, default board and do nothing
}

void puzzleSolver::initializeRowHelper(const string& input1, const string& input2, const string& input3) {
    stringstream rowElements1(input1);
    stringstream rowElements2(input2);
    stringstream rowElements3(input3);

    char letter;
    bool seenA = false;
    int i = 0;
    while(rowElements1 >> letter) {
        if(seenA && letter == 'A') {
            gameBoard[0].at(i++) = 'Z';
        }
        else {
            if(letter == 'A') {
                seenA = true;
            }
            gameBoard[0].at(i++) = letter;
        }
    }
    i = 0;
    while(rowElements2 >> letter) {
        if(seenA && letter == 'A') {
            gameBoard[1].at(i++) = 'Z';
        }
        else {
            if(letter == 'A') {
                seenA = true;
            }
            gameBoard[1].at(i++) = letter;
        }
    }
    i = 0;
    while(rowElements3 >> letter) {
        if(seenA && letter == 'A') {
            gameBoard[2].at(i++) = 'Z';
        }
        else {
            if(letter == 'A') {
                seenA = true;
            }
            gameBoard[2].at(i++) = letter;
        }
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
            temp.boardState[row-1][column] = '0';
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
            temp.boardState[row][column+1] = '0';
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
            temp.boardState[row+1][column] = '0';
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
            temp.boardState[row][column-1] = '0';
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
        if(duration.count() > 3e+8) {
            return false;
        }
    }

    return false;
}

int puzzleSolver::heuristic(const vector<vector<char>>& board) {
    if(algorithm == 2) {
        return numMisplacedTiles(board);
    }
    else if(algorithm == 3) {
        return manhattanDistance(board);
    }
    // for uniform cost search
    return 0;
}

int puzzleSolver::numMisplacedTiles(const vector<vector<char>>& board) {
    int tiles = 0;
    const vector<vector<char>> solution {
        {'A', 'N', 'G'},
        {'E', 'L', 'I'},
        {'C', 'Z', '0'}
    };

    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            if(board[i][j] != '0' && board[i][j] != solution[i][j]) {
                tiles++;
            }
        }
    }
    return tiles;
}

int puzzleSolver::manhattanDistance(const vector<vector<char>>& board) {
    int distance = 0;
    char letter;

    unordered_map<char, pair<int,int>> correctCoord;
    pair<int,int> A(0, 0);
    correctCoord['A'] = A;
    pair<int,int> N(0, 1);
    correctCoord['N'] = N;
    pair<int,int> G(0, 2);
    correctCoord['G'] = G;
    pair<int,int> E(1, 0);
    correctCoord['E'] = E;
    pair<int,int> L(1, 1);
    correctCoord['L'] = L;
    pair<int,int> I(1, 2);
    correctCoord['I'] = I;
    pair<int,int> C(2, 0);
    correctCoord['C'] = C;
    pair<int,int> Z(2, 1);
    correctCoord['Z'] = Z;

    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            letter = board[i][j];
            if(letter != '0') {
                distance += abs(i - correctCoord[letter].first) + abs(j - correctCoord[letter].second);
            }
        }
    }
    return distance;
}

string puzzleSolver::matrixToString(const vector<vector<char>>& board) {
    string nums = "";
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            nums.push_back(board[i][j]);
        }
    }
    return nums;
}

bool puzzleSolver::isSolution(const vector<vector<char>>& currBoard) {
    const vector<vector<char>> solution {
        {'A', 'N', 'G'},
        {'E', 'L', 'I'},
        {'C', 'Z', '0'}
    };
    return (currBoard == solution ? true : false);
}

void puzzleSolver::showGameBoard(const vector<vector<char>>& board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            if(board[i][j] == 'Z') {
                cout << 'A' << " ";
            }
            else {
                cout << board[i][j] << " ";
            }
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