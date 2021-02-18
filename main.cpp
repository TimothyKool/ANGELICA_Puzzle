#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class node {
    vector<vector<int>> boardState;
    int fCost;
    int depth;

};

class puzzleSolver {
    private:
        vector<vector<int>> gameBoard {
            {1, 2, 3},
            {4, 8, 0},
            {7, 6, 5}
        };
        int customBoard = 0;
        int maxQueueSize = 0;
        int totalNodes = 0;
    public:
        void driverFunction();
        void promptBoardType() {
            cout << "Welcome to Timothy Koo's 8-puzzle problem solver." << endl;
            cout << "Type \'1\' to use a default puzzle, or \'2\' to enter your own puzzle." << endl;
            cin >> customBoard;
        }
        void initializeBoard() {
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
        void initializeRowHelper(const string& input, int row) {
            stringstream rowElements(input);
            int num = 0;
            int i = 0;
            while(rowElements >> num) {
                gameBoard[row].at(i++) = num;
            }
        }
        void showGameBoard() {
            for(int i = 0; i < gameBoard.size(); i++) {
                for(int j = 0; j < gameBoard[0].size(); j++) {
                    cout << gameBoard[i][j] << " ";
                }
                cout << endl;
            }
        }
};

void puzzleSolver::driverFunction() {
    promptBoardType();
    initializeBoard();
    showGameBoard();
}


int main() {
    puzzleSolver ps;
    ps.driverFunction();

    return 0;
}