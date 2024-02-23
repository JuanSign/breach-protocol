#include <bits/stdc++.h>

using namespace std;

int buffer_size, matrix_width, matrix_height, n_seq;
vector<vector<string>> matrix, sequence;
vector<vector<int>> prefix;
vector<vector<bool>> visited;
vector<int> reward, seq_length, point;

int cur_length = 0, fix_length = 0;
int cur_score = 0, fix_score = INT_MIN;
vector<pair<int, int>> cur_seq, fix_seq;

void save_answer()
{
    fix_length = cur_length;
    fix_score = cur_score;
    for(int i = 0; i < fix_length; i++)
    {
        fix_seq[i] = cur_seq[i];
    }
}

void load()
{
    ifstream input ("input.txt", ios::in);
    input >> buffer_size >> matrix_width >> matrix_height;
    matrix.resize(matrix_height, vector<string>(matrix_width));
    for(int i = 0; i < matrix_height; i++)
    {
        for(int j = 0; j < matrix_width; j++)
        {
            input >> matrix[i][j];
        }
    }
    input >> n_seq;
    sequence.resize(n_seq);
    reward.resize(n_seq);
    seq_length.resize(n_seq, 0);
    string s;
    for(int i = 0; i < n_seq; i++)
    {
        getline(input, s);
        while(input.peek() != 10)
        {
            input >> s;
            sequence[i].push_back(s);
            seq_length[i]++;
        }
        input >> reward[i];
    }
}

void setup()
{
    cur_seq.resize(buffer_size);
    fix_seq.resize(buffer_size);
    prefix.resize(n_seq);
    point.resize(n_seq, 0);
    visited.resize(matrix_height, vector<bool>(matrix_width, 0));
    for(int i = 0; i < n_seq; i++)
    {
        prefix[i].resize(seq_length[i], 0);
        prefix[i][0] = 0;
        int p = 0;
        for(int j = 1; j < seq_length[i]; j++)
        {
            prefix[i][j] = p;
            if(sequence[i][j] == sequence[i][p]) p++;
            else p = 0;
        }
    }
}

void traverse(int row, int col)
{
    if(visited[row][col]) return;

    visited[row][col] = 1;
    cur_seq[cur_length++] = make_pair(row, col);

    int score_buffer = cur_score;
    int point_buffer[n_seq];
    for(int i = 0; i < n_seq; i++) point_buffer[i] = point[i];

    for(int i = 0; i < n_seq; i++)
    {
        if(matrix[row][col] == sequence[i][point[i]])
        {
            if(point[i] == seq_length[i]-1) 
            {
                cur_score += reward[i];
                point[i] = 0;
            }
            else point[i]++;
        }
        else
        {
            do
            {
                point[i] = prefix[i][point[i]];
            } while (matrix[row][col] != sequence[i][point[i]] && point[i]);
            if(matrix[row][col] == sequence[i][point[i]]) point[i]++;
        }
    }

    if(cur_score > fix_score) save_answer();

    bool checker = false;
    for(int i = 0; i < n_seq; i++)
    {
        checker |= ((buffer_size-cur_length) >= (seq_length[i]-point[i]));
    }

    if(cur_length != buffer_size && checker)
    {
        if(cur_length%2)
        {
            for(int i = 1; i <= row; i++) traverse(row-i, col);
            for(int i = 1; i <= matrix_height-row-1; i++) traverse(row+i, col); 
        }
        else
        {
            for(int i = 1; i <= col; i++) traverse(row, col-i);
            for(int i = 1; i <= matrix_width-col-1; i++) traverse(row, col+i);
        }
    }

    visited[row][col] = 0;
    cur_length--;
    cur_score = score_buffer;
    for(int i = 0; i < n_seq; i++) point[i] = point_buffer[i];
}

int main()
{
    load();
    setup();
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0; i < matrix_width; i++) traverse(0,i);
    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double, milli>(end-start).count();

    cout << "DONE IN " << time << " MILLISECONDS" << '\n';
    cout << "MAXIMUM SCORE: " << fix_score << '\n';
    cout << "COORDINATE (ROW,COL) : ";
    for(int i = 0; i < fix_length; i++) cout << "(" << fix_seq[i].first << "," << fix_seq[i].second << ")" << " ";
    cout << '\n';
    cout << "TOKENS : ";
    for(int i = 0; i < fix_length; i++) cout << matrix[fix_seq[i].first][fix_seq[i].second] << " ";
    cout << '\n';
}