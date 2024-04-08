#include "sudoku.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    // Creates instance of Sudoku game.
    Sudoku game = Sudoku();
    bool error = game.generate();
    if (error)
    {
        cout << "Failed to generate board.\n";
        return 1;
    }

    // Game loop (keeps track of whether player used solve() to make fun of them later).
    bool usedSolver = false;
    while (!game.status())
    {
        game.print();

        cout << "Enter command: ";
        string com;
        cin >> com;

        if (cin.fail())
            break;

        if (com == "help" || com == "h")
        {
            cout << "\n";

            cout << "Commands are:\n";
            cout << "> \'help (h)\': Prints commands.\n";
            cout << "> \'quit (q)\': Quits game.\n";
            cout << "> \'place (p) <row> <col> <val>\': Places value at specified row and column.\n";
            cout << "> \'addnote (an) <row> <col> <...notes>\': Inserts notes at specified row and column.\n";
            cout << "> \'removenote (rn) <row> <col> <...notes>\': Removes notes at specified row and column.\n";
            cout << "> \'printnotes (pn) <row> <col>\': Prints notes at specified row and column.\n";
            cout << "> \'solve (s)\': Solves board.\n";
            // cout << "> \'new\': Generates new board.\n";

            cout << "\n";
        }
        else if (com == "place" || com == "p")
        {
            int r, c, n;
            cin >> r >> c >> n;
            cout << "\n";

            game.place(r - 1, c - 1, n);
        }
        else if (com == "addnote" || com == "an")
        {
            // Allows us to read until end of line, accepting variable number of values.
            string line;
            getline(cin, line);
            stringstream ss(line);

            int r, c;
            ss >> r >> c;

            cout << "\n";

            int n;
            while (ss >> n)
            {
                game.addNote(r - 1, c - 1, n);
            }

            cout << "\n";
        }
        else if (com == "removenote" || com == "rn")
        {
            // Allows us to read until end of line, accepting variable number of values.
            string line;
            getline(cin, line);
            stringstream ss(line);

            int r, c;
            ss >> r >> c;

            int n;
            while (ss >> n)
            {
                game.removeNote(r - 1, c - 1, n);
            }

            cout << "\n";
        }
        else if (com == "printnotes" || com == "pn")
        {
            int r, c;
            cin >> r >> c;
            cout << "\n";

            game.printNotes(r - 1, c - 1);

            cout << "\n";
        }
        else if (com == "solve" || com == "s")
        {
            cout << "\n";

            game.solve();
            usedSolver = true;
        }
        else // Fine, you don't have to type "quit" or "q". Just type fucking anything. I don't care.
        {
            cout << "\n";
            break;
        }
    }

    game.print();
    if (game.status() == 1)
    {
        if (usedSolver)
            cout << "used solver... fool...\n";
        else
            cout << "you won!! lmao\n";
    }
    else if (game.status() == -1)
    {
        cout << "you lose!!!!!!!!\n";
    }
    cout << "ok bye" << endl;
}