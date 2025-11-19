/*
 * IST - ASA 25/26 - Project 1 - Solution
 * Algorithm: Dynamic Programming (Interval DP)
 * Time Complexity: O(N^3)
 * Space Complexity: O(N^2)
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Biochemical classes mapping
// P=0, N=1, A=2, B=3, T=4
int get_class_index(char c)
{
  switch (c)
  {
  case 'P':
    return 0;
  case 'N':
    return 1;
  case 'A':
    return 2;
  case 'B':
    return 3;
  case 'T':
    return 4; // Terminal
  default:
    return -1;
  }
}

// Affinity Matrix (4x4 base + T)
// Based on the problem statement:
//   P N A B
// P 1 3 1 3
// N 5 1 0 1
// A 0 1 0 4
// B 1 3 2 3
// T interacts with everything with value 1 (Row/Col 4)
int AF[5][5] = {
    {1, 3, 1, 3, 1}, // P
    {5, 1, 0, 1, 1}, // N
    {0, 1, 0, 4, 1}, // A
    {1, 3, 2, 3, 1}, // B
    {1, 1, 1, 1, 1}  // T (Terminal)
};

// Global variables
int n;
vector<int> p;                // Potentials
vector<int> c;                // Classes
vector<vector<long long>> dp; // DP Table (Max Energy)
vector<vector<int>> root;     // Roots table for solution reconstruction (index k)
vector<int> result_sequence;  // To store the final output order

// Function to recontruct the sequence from the root table
// Logic: Post-order traversal simulation (Left -> Right -> Root)
// beacause 'k' is the LAST element removed in the interval (i, j)
void build_sequence(int i, int j)
{
  if (i + 1 >= j)
    return;

  int k = root[i][j];

  // Process left sub-interval
  build_sequence(i, k);
  // Process right sub-interval
  build_sequence(k, j);

  // Add k to the sequence (k is removed last)
  result_sequence.push_back(k);
}

int main()
{
  // I/O Optimization for faster execution
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // 1. Read N
  if (!(cin >> n))
    return 0;

  // 2. Read Potentials
  // Indices 1 to N. 0 and N+1 are sentinels (guards).
  p.resize(n + 2);
  p[0] = 1;     // Left Sentinel (Terminal)
  p[n + 1] = 1; // Right Sentinel (Terminal)
  for (int i = 1; i <= n; i++)
  {
    cin >> p[i];
  }

  // 3. Read Classes
  c.resize(n + 2);
  c[0] = 4;     // T (Terminal)
  c[n + 1] = 4; // T (Terminal)

  string s;
  cin >> s;
  for (int i = 0; i < n; i++)
  {
    c[i + 1] = get_class_index(s[i]);
  }

  // 4. Initialize DP tables
  // Size (N+2) x (N+2)
  dp.assign(n + 2, vector<long long>(n + 2, 0));
  root.assign(n + 2, vector<int>(n + 2, 0));
}