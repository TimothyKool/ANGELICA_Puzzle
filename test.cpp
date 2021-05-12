#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    unordered_map<char, pair<int,int>> coordinates;
    pair<int,int> temp(10,6);
    coordinates['A'] = temp;

    cout << coordinates['A'].second << endl;

    return 0;
}