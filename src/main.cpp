/*
 * IST - ASA 25/26 - Project 1
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Maximum expected size N + safety margin.
const int MAXN = 2005;

// DP Table: dp[i][j] stores max energy for the interval (i, j).
unsigned long long dp[MAXN][MAXN];

// Roots Table: root[i][j] stores the index 'k' that provided the max energy.
unsigned long long root[MAXN][MAXN];

// Input data: Potentials (p) and Biochemical Classes (c)
unsigned long long p[MAXN];
unsigned long long c[MAXN];

// Affinity Matrix
int AF[5][5] = {
    {1, 3, 1, 3, 1}, // P
    {5, 1, 0, 1, 1}, // N
    {0, 1, 0, 4, 1}, // A
    {1, 3, 2, 3, 1}, // B
    {1, 1, 1, 1, 1}  // T
};

int n;
vector<int> result_sequence;

inline int get_class_index(char ch)
{
  switch (ch)
  {
  case 'P':
    return 0;
  case 'N':
    return 1;
  case 'A':
    return 2;
  case 'B':
    return 3;
  default:
    return 4; // T
  }
}

// Recursive function to reconstruct the removal order.
void build_sequence(int i, int j)
{
  if (i + 1 >= j)
    return;

  int k = root[i][j];

  // Process sub-intervals
  build_sequence(i, k);
  build_sequence(k, j);

  // Add current split point
  result_sequence.push_back(k);
}

int main()
{
  // I/O Optimization for faster execution times
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  if (!(cin >> n))
    return 0;

  // 1. Input Reading & Sentinel Setup
  // Sentinels at indices 0 and N+1 represent the fixed boundary amino acids.
  p[0] = 1;
  p[n + 1] = 1;
  for (int i = 1; i <= n; i++)
    cin >> p[i];

  c[0] = 4;     // Class T
  c[n + 1] = 4; // Class T
  string s;
  cin >> s;
  for (int i = 0; i < n; i++)
    c[i + 1] = get_class_index(s[i]);

  // 2. Dynamic Programming Core
  // Iterate through all chain lengths, starting from 2 (minimum gap).
  for (int len = 2; len <= n + 1; len++)
  {
    // Iterate through all starting positions
    for (int i = 0; i <= n + 1 - len; i++)
    {
      int j = i + len;

      unsigned long long max_energy = 0;
      int best_k = -1;

      // Cache values to minimize array lookups inside the inner loop
      int pi = p[i];
      int pj = p[j];
      int ci = c[i];
      int cj = c[j];

      for (int k = i + 1; k < j; k++)
      {

        // Calculate interaction energy for removing k with factorized formula
        unsigned long long interaction = (unsigned long long)p[k] * (pi * AF[ci][c[k]] + AF[c[k]][cj] * pj);
        unsigned long long current_energy = dp[i][k] + dp[k][j] + interaction;

        // Tie-breaking rule for Lexicographical Order
        if (current_energy >= max_energy)
        {
          max_energy = current_energy;
          best_k = k;
        }
      }

      dp[i][j] = max_energy;
      root[i][j] = best_k;
    }
  }

  // 3. Output Results
  cout << dp[0][n + 1] << endl;

  // Reconstruct solution
  result_sequence.reserve(n);
  build_sequence(0, n + 1);

  // Print sequence formatted with spaces
  for (size_t i = 0; i < result_sequence.size(); i++)
  {
    cout << result_sequence[i] << (i == result_sequence.size() - 1 ? "" : " ");
  }
  cout << endl;

  return 0;
}