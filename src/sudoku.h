#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <unordered_map>
#include <unordered_set>

// should add notes feature, check if generated board has multiple solutions
class Sudoku
{
private:
    // data members
    std::vector<std::vector<int>> board_;
    std::vector<std::vector<int>> solvedBoard_;
    std::unordered_map<int, std::unordered_set<int>> notes_;
    int numEmpty_;
    bool playable_;
    int fails;

    // internal functions
    bool checkPlacement(int r, int c, int n) const;
    bool validPlacement(int r, int c, int n) const;
    bool helper(int index);
    int helper2(int index); // could use better names, huh

public:
    // constructor
    Sudoku();

    // game management
    bool generate();
    void reset();
    void solve();
    void print() const;
    void place(int r, int c, int n);
    void addNote(int r, int c, int note);
    void removeNote(int r, int c, int note);
    void printNotes(int r, int c);
    bool done() const;
};

#endif