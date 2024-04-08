#include "sudoku.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>

using namespace std;

// private:
bool Sudoku::checkPlacement(int r, int c, int n) const
{
    // Checks column for conflicts.
    for (int i = 0; i < 9; i++)
    {
        if (i != r && board_[i][c] == n)
            return false;
    }

    // Checks row for conflicts.
    for (int i = 0; i < c; i++)
    {
        if (i != c && board_[r][i] == n)
            return false;
    }

    // Checks box for conflicts.
    int r3 = (r / 3) * 3;
    int c3 = (c / 3) * 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (r3 + i == r && c3 + j == c)
                continue;
            if (board_[r3 + i][c3 + j] == n)
                return false;
        }
    }

    return true;
}

bool Sudoku::validPlacement(int r, int c, int n) const
{
    if (status_ != 0)
    {
        cout << "The game has either finished or has not been started.\n";
        return false;
    }
    else if (r < 0 || r > 8 || c < 0 || c > 8)
    {
        cout << "This location is not on the board.\n";
        return false;
    }
    else if (n < 1 || n > 9)
    {
        cout << "Value must be between 1 and 9.\n";
        return false;
    }
    else if (board_[r][c] != 0)
    {
        cout << "This space is already occupied.\n";
        return false;
    }
    return true;
}

// helper() fills a board with valid placements and returns whether a solution exists or not.
bool Sudoku::helper(int index) // Consider including time so we can break the loop if it's taking too long.
{
    // All empty spots have been considered so we're done.
    if (index > 80)
        return true;

    int r = index / 9, c = index % 9;

    // We choose random values this way so the following for loop doesn't check the same value multiple times.
    vector<int> order = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(order.begin(), order.end(), default_random_engine(seed));

    // Note: This function can be used to generate AND solve the board, because generation is just solving an
    // empty board. As of right now, there's no need to call this in solve() though.
    for (int i = 0; i < 9; i++)
    {
        int n = order[i];
        if (checkPlacement(r, c, n))
        {
            board_[r][c] = n;

            int j;
            for (j = 0; j < 81 - index; j++)
            {
                int n_r = (index + j) / 9, n_c = (index + j) % 9;
                if (board_[n_r][n_c] == 0)
                    break;
            }

            bool works = helper(index + j);
            if (works)
            {
                return true;
            }
        }
    }
    board_[r][c] = 0;
    return false;
}

// helper2() differs from helper() in that it returns the number of solutions and reverts the board to its
// original state afterwards. Could probably use a better name, but there's no "we" in Shitty Sudoku App,
// so I make the decisions.
int Sudoku::helper2(int index)
{
    // All empty spots have been considered so we're done.
    if (index > 80)
        return 1;

    int r = index / 9, c = index % 9;
    int count = 0;

    for (int n = 1; n <= 9; n++)
    {
        if (checkPlacement(r, c, n))
        {
            board_[r][c] = n;

            int j;
            for (j = 0; j < 81 - index; j++)
            {
                int n_r = (index + j) / 9, n_c = (index + j) % 9;
                if (board_[n_r][n_c] == 0)
                    break;
            }

            count += helper2(index + j);
        }
    }
    board_[r][c] = 0;
    return count;
}

// public:

// constructor
Sudoku::Sudoku() : board_(9, vector<int>(9, 0)), solvedBoard_(9, vector<int>(9, 0))
{
    status_ = 0;
    numEmpty_ = 81;
    fails = 5;
}

// game management
bool Sudoku::generate()
{
    reset();
    helper(0);

    // helper() generates a full board, so we keep a copy before removing spots to check against player
    // moves (and also solve the board if prompted).
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            solvedBoard_[i][j] = board_[i][j];
        }
    }

    srand(time(0));
    numEmpty_ = rand() % 10 + 50; // Slates anywhere from 50 - 59 spots to be cleared.
    unordered_set<int> spots;

    // Actually removes those spots.
    int helper2Val = 1, minInd = 81, iters = 0;
    while (!(spots.size() == numEmpty_ && helper2Val == 1))
    {
        int ind = rand() % 81;
        int tmp = board_[ind / 9][ind % 9];
        board_[ind / 9][ind % 9] = 0;
        minInd = min(ind, minInd);

        helper2Val = helper2(minInd);
        if (helper2Val > 1)
            board_[ind / 9][ind % 9] = tmp;
        else
            spots.insert(ind);
        iters++;

        // will be changed
        if (iters == 500)
            return true;
    }

    status_ = 0;
    return false;
}

void Sudoku::reset()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board_[i][j] = 0;
        }
    }
}

void Sudoku::solve()
{
    if (status_ != 0)
    {
        cout << "The board is either completed or has not been generated.\n";
        return;
    }

    // This just replaces board with previously stored solved board. It's easy to use helper() for this
    // function, and we SHOULD use it when dealing with boards not generated by generate(). But for now,
    // there's no need.
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board_[i][j] = solvedBoard_[i][j];
        }
    }

    numEmpty_ = 0;
    status_ = 1;
}

void Sudoku::print() const
{
    cout << "+-------+-------+-------+\n";
    for (int i = 0; i < 9; i++)
    {
        cout << "|";
        for (int j = 0; j < 9; j++)
        {
            cout << " " << (board_[i][j] == 0 ? "." : to_string(board_[i][j])); // ternaries!! so fun!!
            if ((j + 1) % 3 == 0)
                cout << " |";
        }
        cout << "\n";
        if ((i + 1) % 3 == 0)
        {
            cout << "+-------+-------+-------+\n";
        }
    }
}

void Sudoku::place(int r, int c, int n)
{
    // If we can't place here, we leave.
    if (!validPlacement(r, c, n))
    {
        return;
    }
    else if (solvedBoard_[r][c] != n)
    {
        fails--;
        cout << "Incorrect placement. Fails left: " << fails << ".\n";
        if (fails == 0)
            status_ = -1;
        return;
    }

    // Gets rid of notes for current space.
    board_[r][c] = n;
    numEmpty_--;
    if (notes_.count(r * 9 + c))
        notes_.erase(r * 9 + c);

    // Checks row, column, and box to remove any notes containing n (quality of life thing).
    for (int i = 0; i < 9; i++)
    {
        if (i != r && notes_.count(i * 9 + c) && notes_[i * 9 + c].count(n))
            notes_[r * 9 + i].erase(n);
    }

    for (int i = 0; i < c; i++)
    {
        if (i != c && notes_.count(r * 9 + i) && notes_[r * 9 + i].count(n))
            notes_[r * 9 + i].erase(n);
    }

    int r3 = (r / 3) * 3;
    int c3 = (c / 3) * 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (r3 + i == r && c3 + j == c)
                continue;
            if (notes_.count((r3 + i) * 9 + (c3 + j)) && notes_[(r3 + i) * 9 + (c3 + j)].count(n))
                notes_[(r3 + i) * 9 + (c3 + j)].erase(n);
        }
    }

    if (numEmpty_ == 0)
    {
        status_ = 1;
    }
}

void Sudoku::addNote(int r, int c, int note)
{
    if (!validPlacement(r, c, note))
    {
        return;
    }

    int index = r * 9 + c;
    notes_[index].insert(note);
    // Consider checking surrounding conditions to disallow wrong notes.
}

void Sudoku::removeNote(int r, int c, int note)
{
    if (!validPlacement(r, c, note))
    {
        return;
    }

    int index = r * 9 + c;
    notes_[index].erase(note);
}

void Sudoku::printNotes(int r, int c)
{
    if (!validPlacement(r, c, 1))
    {
        return;
    }

    int index = r * 9 + c;
    cout << "Notes for (" << r + 1 << ", " << c + 1 << "): [";
    unordered_set<int> &curNotes = notes_[index];

    // We keep track of the size here because we want to stop iterating 1 before the last element. But
    // we're using an unordered_set, so we have to work around unindexed values.
    int size = 1, lastElem = -1;
    for (int i : curNotes)
    {
        if (size == curNotes.size())
        {
            lastElem = i;
            break;
        }
        cout << i << ", ";
        size++;
    }
    cout << (lastElem == -1 ? "" : to_string(lastElem)) << "]\n";
}

int Sudoku::status() const
{
    // -1 = player lost, 0 = not done, 1 = player won
    return status_;
}