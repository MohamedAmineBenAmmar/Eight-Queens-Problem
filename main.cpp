#include <iostream>
#include <map>
#include <bits/stdc++.h>
using namespace std;
struct Position
{
    int row;
    int col;
};

int **allocate_matrix(int size)
{
    int **rows = new int *[size];

    for (int i = 0; i < size; i++)
    {
        rows[i] = new int[size];
    }

    return rows;
}

void init_matrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void display(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << matrix[i][j] << '\t';
        }

        cout << '\n';
        cout << '\n';
    }
}

bool is_not_valid(int **matrix, int size, int row, int col, map<int, struct Position> &queens_positions_by_row, map<int, struct Position> &queens_positions_by_col)
{
    // Check if I can set a queen in the m[i, j]
    bool my_row, my_col, left_upper_diag, right_upper_diag, left_lower_diag, right_lower_diag;
    int prev_row, next_row, prev_col, next_col;

    left_upper_diag = false;
    left_lower_diag = false;
    right_upper_diag = false;
    right_lower_diag = false;

    // Inialization of the counters
    prev_row = row - 1;
    next_row = row + 1;
    prev_col = col - 1;
    next_col = col + 1;

    // Verify the diagonals
    while (prev_row > -1 || prev_col > -1 || next_row < size || next_col < size)
    {
        // cout << prev_row << " " << prev_col << " " << next_row << " " << next_col <<  endl;
        // int x;
        // cin >> x;
        // cout << '\n';

        if ((prev_row > -1) && (prev_col > -1) && (matrix[prev_row][prev_col] == 1))
        {
            left_upper_diag = true;
            break;
        }

        if ((prev_row > -1) && (next_col < size) && (matrix[prev_row][next_col] == 1))
        {
            right_upper_diag = true;
            break;
        }

        if ((next_row < size) && (prev_col > -1) && (matrix[next_row][prev_col] == 1))
        {
            left_lower_diag = true;
            break;
        }

        if ((next_row < size) && (next_col < size) && (matrix[next_row][next_col] == 1))
        {
            right_lower_diag = true;
            break;
        }

        // Increment counters
        prev_row--;
        next_row++;
        prev_col--;
        next_col++;
    }

    // Verify the row and the column
    my_row = false;
    my_col = false;

    if (queens_positions_by_row.count(row) > 0 && queens_positions_by_row[row].col != col)
    {
        my_row = true;
    }

    if (queens_positions_by_col.count(col) > 0 && queens_positions_by_col[col].row != row)
    {
        my_col = true;
    }

    return my_row || my_col || left_upper_diag || right_upper_diag || left_lower_diag || right_lower_diag;
}

void set_queens(int **matrix, int size, int row, map<int, struct Position> &queens_positions_by_row, map<int, struct Position> &queens_positions_by_col, char operation)
{
    // s: for set queen
    // r: for remove queen
    bool placed;
    bool is_not_valid_position;

    if (row < size)
    {
        if (operation == 's')
        {
            // Going to set a queen in the row of index equal to row
            placed = false;
            for (int j = 0; j < size; j++)
            {
                bool is_not_valid_position = is_not_valid(matrix, size, row, j, queens_positions_by_row, queens_positions_by_col);

                if (!is_not_valid_position)
                {

                    Position pos;
                    pos.row = row;
                    pos.col = j;

                    matrix[row][j] = 1;

                    // cout << "matrix state change" << endl;
                    // display(matrix, size);
                    // cout << "\n";

                    placed = true;

                    queens_positions_by_row.insert(pair<int, Position>(row, pos));
                    queens_positions_by_col.insert(pair<int, Position>(j, pos));
                    break;
                }
            }

            // If the this loop finished and the palced flag is still false means that there is no place on the current row
            if (!placed)
            {
                set_queens(matrix, size, row - 1, queens_positions_by_row, queens_positions_by_col, 'r');
            }
            else
            {
                set_queens(matrix, size, row + 1, queens_positions_by_row, queens_positions_by_col, 's');
            }
        }
        else
        {
            // Going to remove the queen from the row equal to row
            if (queens_positions_by_row.count(row) > 0)
            {
                // Updated portion
                int queen_old_col = queens_positions_by_row[row].col;
                matrix[row][queens_positions_by_row[row].col] = 0;

                // cout << "matrix state change" << endl;
                // display(matrix, size);
                // cout << "\n";

                queens_positions_by_col.erase(queens_positions_by_row[row].col);
                queens_positions_by_row.erase(row);

                placed = false;
                for (int j = queen_old_col + 1; j < size; j++)
                {
                    is_not_valid_position = is_not_valid(matrix, size, row, j, queens_positions_by_row, queens_positions_by_col);

                    if (!is_not_valid_position)
                    {

                        Position pos;
                        pos.row = row;
                        pos.col = j;

                        matrix[row][j] = 1;

                        // cout << "matrix state change" << endl;
                        // display(matrix, size);
                        // cout << "\n";

                        placed = true;

                        queens_positions_by_row.insert(pair<int, Position>(row, pos));
                        queens_positions_by_col.insert(pair<int, Position>(j, pos));
                        break;
                    }
                }

                // If the this loop finished and the palced flag is still false means that there is no place on the current row
                if (!placed)
                {
                    set_queens(matrix, size, row - 1, queens_positions_by_row, queens_positions_by_col, 'r');
                }
                else
                {
                    set_queens(matrix, size, row + 1, queens_positions_by_row, queens_positions_by_col, 's');
                }
                // end updated portion
            }
        }
    }
}

void eight_queens(int **matrix, int size)
{
    map<int, struct Position> queens_positions_by_row;
    map<int, struct Position> queens_positions_by_col;

    // Set queens
    set_queens(matrix, size, 0, queens_positions_by_row, queens_positions_by_col, 's');

    // Validate the queens positioning
    // Get an iterator pointing to the first element in the map
    map<int, struct Position>::iterator it = queens_positions_by_row.begin();

    // Iterate through the map and print the elements
    bool is_not_valid_position = false;
    while (it != queens_positions_by_row.end())
    {
        is_not_valid_position = is_not_valid(matrix, size, it->second.row, it->second.col, queens_positions_by_row, queens_positions_by_col);
        if (is_not_valid_position)
        {
            break;
        }

        ++it;
    }

    if (is_not_valid_position)
    {
        cout << "Error found in the queens positioning \n";
    }
    else
    {
        cout << "All queens are well positioned \n";
    }
}

int main()
{
    int size = 8;
    std::map<int, struct Position> queens_positions_by_col;
    int **matrix = allocate_matrix(size);
    init_matrix(matrix, size);
    eight_queens(matrix, size);
    display(matrix, size);

    return 0;
}