#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Coord {
   int molecule;
   int atom;
};

struct Atom {
   vector<int> target;
   // bool is_fast = false;
};

bool is_isomorfism(vector<string> A, vector<string> B)
{
   bool result = false;

   // If find two same certificates, molecules are same
   for (int i = 0; i < A.size(); ++i) {
      for (int j = 0; j < B.size(); ++j) {
         if (!A[i].compare(B[j]))
            return true;
      }
   }

   return result;
}

string generate_certificate(vector<Atom> &molecule, vector<bool> &visited,
                            int start_atom)
{
   string ret;
   if (visited[start_atom] == true)
      return "01";
   visited[start_atom] = true;
   for (auto &to : molecule[start_atom].target) {
      if (!visited[to] && molecule[to].target.size() == 1) {
         ret.append(generate_certificate(molecule, visited, to));
      }
   }
   for (auto &to : molecule[start_atom].target) {
      if (!visited[to] && molecule[to].target.size() == 2) {
         ret.append(generate_certificate(molecule, visited, to));
      }
   }
   for (auto &to : molecule[start_atom].target) {
      if (!visited[to] && molecule[to].target.size() == 3) {
         ret.append(generate_certificate(molecule, visited, to));
      }
   }
   return "0" + ret + "1";
}

int main(int argc, char const *argv[])
{
   // Load the metadata
   int count_molecules, count_atoms, count_bonds;
   if (scanf("%d %d %d\n", &count_molecules, &count_atoms, &count_bonds) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }
   // printf("M: %d, A: %d, B: %d\n", count_molecules, count_atoms,
   // count_bonds);

   // Load the data
   vector<vector<Atom>> molecules(count_molecules, vector<Atom>(count_atoms));
   for (auto &molecule : molecules) {
      for (int i = 0; i < count_bonds; ++i) {
         int a, b;
         if (scanf("%d %d\n", &a, &b) != 2) {
            fprintf(stderr, "ERROR - can't load the data!\n");
            exit(EXIT_FAILURE);
         }
         a -= 1;
         b -= 1;
         molecule[a].target.push_back(b);
         molecule[b].target.push_back(a);
      }
   }

   queue<Coord> task_to_generate;

   // Calcule token for each combination by targets count
   // This policy has not influence
   vector<long unsigned int> molecule_tokens(count_molecules, 0);
   for (int i = 0; i < count_molecules; ++i) {
      for (int j = 0; j < molecules[i].size(); ++j) {
         int size = molecules[i][j].target.size();
         sort(molecules[i][j].target.begin(), molecules[i][j].target.end(),
              [](const int &a, const int &b) { return a < b ? true : false; });
         // 1 - 1
         // 2 - 10000
         // 3 - 100000000
         if (size == 1) {
            molecule_tokens[i] += 1;
            task_to_generate.push({i, j});
         } else if (size == 2) {
            task_to_generate.push({i, j});
            molecule_tokens[i] += 10000;
         } else {
            task_to_generate.push({i, j});
            molecule_tokens[i] += 100000000;
         }
      }
   }

   vector<vector<string>> certifiactes(count_molecules);

   while (!task_to_generate.empty()) {
      Coord tmp = task_to_generate.front();
      vector<bool> visited(count_atoms, false);
      string certificate =
          generate_certificate(molecules[tmp.molecule], visited, tmp.atom);
      // cout << certificate << endl;
      certifiactes[tmp.molecule].push_back(certificate);
      task_to_generate.pop();
   }

   // Map ordered molecules
   vector<bool> is_ordered(count_molecules, false);
   vector<int> group_counts;

   for (int i = 0; i < count_molecules; ++i) {
      // If source molecule is orderd, skip it
      if (is_ordered[i])
         continue;

      // Create new class
      is_ordered[i] = true;
      group_counts.push_back(1);

      for (int j = i + 1; j < count_molecules; ++j) {
         // If molecule is orderd, skip it
         if (is_ordered[j])
            continue;

         // Valid isomorfism
         if (molecule_tokens[i] == molecule_tokens[j] &&
             is_isomorfism(certifiactes[i], certifiactes[j])) {
            is_ordered[j] = true;

            // incereas last index
            group_counts.back() += 1;
         }
      }
   }

   sort(group_counts.begin(), group_counts.end(),
        [](const int &a, const int &b) { return a > b ? false : true; });

   // Print results
   for (long unsigned int i = 0; i < group_counts.size(); ++i) {
      if (i == 0)
         cout << group_counts[i];
      else
         cout << " " << group_counts[i];
   }
   cout << endl;

   return 0;
}
