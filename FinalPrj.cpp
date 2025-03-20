/**
 * @author Austin Savoie <austin.savoie@uleth.ca>,
 * @author Sofiah Plarison <s.plarison@uleth.ca>
 * @date Spring 2025
 */

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

// Function prototypes
/**
 * @brief Compute the edit distance between two words A and B.
 * @param A The first word, as an array of characters.
 * @param B The second word, as an array of characters.
 * @return The edit distance between A and B.
 * @details O(m*n) time complexity and O(m*n) space complexity, where m is the
 * length of A and n is the length of B. However, in larger cases, the space is
 * incredibly inefficient.
 */
int nieveEdit(const std::vector<char>& A, const std::vector<char>& B);

/**
 * @brief Compute the edit distance between two words A and B.
 * @param A The first word, as an array of characters.
 * @param B The second word, as an array of characters.
 * @return The edit distance between A and B.
 * @details O(m*n) time complexity and O(min(m, n)) space complexity, where m is
 * the length of A and n is the length of B. This is possible by saving only the
 * current and previous rows of the dynamic programming table.
 */
int optimizedEdit(const std::vector<char>& A, const std::vector<char>& B);

/**
 * @brief Input two words from the user.
 * @param A The first word, as an array of characters.
 * @param B The second word, as an array of characters.
 * @details This function will prompt the user to enter two words, and store
 * them in A and B, respectively.
 */
void inputWords(std::vector<char>& A, std::vector<char>& B);

/**
 * @details This main function tests the EditDistance algorithm in worst case,
 * as in it will have to make a change in every character. It is also testing
 * the time and memory usage of the algorithm, of which is O(m*n), but in this
 * example, is O(n^2) as m and n are the same.
 */
int main() {
  std::vector<long long int> sizes = {
      10,   25,   50,    75,    100,   250,   500,    750,    1000,  2500,
      5000, 7500, 10000, 25000, 50000, 75000, 100000, 125000, 150000};
  for (long long int size : sizes) {
    std::vector<char> A(size, 'a');
    std::vector<char> B(size, 'b');

    auto start = std::chrono::high_resolution_clock::now();
    int distance = nieveEdit(A, B);
    auto end = std::chrono::high_resolution_clock::now();

    auto optimizedStart = std::chrono::high_resolution_clock::now();
    int optimizedDistance = optimizedEdit(A, B);
    auto optimizedEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::chrono::duration<double> optimizedElapsed =
        optimizedEnd - optimizedStart;

    long long int naiveSpace = size * size * sizeof(char);
    long long int optimizedSpace = size * sizeof(char);

    std::cout << "Naive: " << std::endl;
    std::cout << "Edit Distance for size " << size << ": " << distance
              << std::endl;
    std::cout << "Time for size " << size << ": " << elapsed.count()
              << " seconds." << std::endl;
    std::cout << "Memory used for size " << size << ": " << naiveSpace
              << " bytes." << std::endl;
    std::cout << std::endl;

    std::cout << "Optimized: " << std::endl;
    std::cout << "Edit Distance for size " << size << ": " << optimizedDistance
              << std::endl;
    std::cout << "Time for size " << size << ": " << optimizedElapsed.count()
              << " seconds." << std::endl;
    std::cout << "Memory used for size " << size << ": " << optimizedSpace
              << " bytes." << std::endl;
    std::cout << std::endl;
  }
  return 0;
}

// Function definitions
int nieveEdit(const std::vector<char>& A, const std::vector<char>& B) {
  int m = A.size();
  int n = B.size();

  // Create a 2D table to store the edit distances
  std::vector<std::vector<int>> Edit(m + 1, std::vector<int>(n + 1));

  // Initialize the first row and first column
  for (int j = 0; j <= n; ++j) {
    Edit[0][j] = j;  // Base case for transforming A[0..0] to B[0..j]
  }

  for (int i = 0; i <= m; ++i) {
    Edit[i][0] = i;  // Base case for transforming A[0..i] to B[0..0]
  }

  // Compute the rest of the table
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      int insert = Edit[i][j - 1] + 1;                            // Insertion
      int del = Edit[i - 1][j] + 1;                               // Deletion
      int replace = Edit[i - 1][j - 1] + (A[i - 1] != B[j - 1]);  // Replacement

      // Take the minimum of insert, delete, and replace
      Edit[i][j] = std::min(std::min(insert, del), replace);
    }
  }

  // Return the edit distance between A[0..m] and B[0..n]
  return Edit[m][n];
}

int optimizedEdit(const std::vector<char>& A, const std::vector<char>& B) {
  int m = A.size();
  int n = B.size();
  std::vector<int> prev(n + 1, 0);  // Initialize the previous row
  std::vector<int> curr(n + 1, 0);  // Initialize the current row

  for (int i = 0; i <= m; ++i) {
    for (int j = 0; j <= n; ++j) {
      if (i == 0) {
        curr[j] = j;  // Base case for transforming A[0..0] to B[0..j]
      } else if (j == 0) {
        curr[j] = i;  // Base case for transforming A[0..i] to B[0..0]
      } else if (A[i - 1] == B[j - 1]) {
        curr[j] = prev[j - 1];  // No operation needed
      } else {
        int insert = curr[j - 1] + 1;                        // Insertion
        int del = prev[j] + 1;                               // Deletion
        int replace = prev[j - 1] + (A[i - 1] != B[j - 1]);  // Replacement

        // Take the minimum of insert, delete, and replace
        curr[j] = std::min(std::min(insert, del), replace);
      }
    }
    prev = curr;  // Update the previous row
  }
  return curr[n];  // Return the edit distance between A[0..m] and B[0..n]
}

void inputWords(std::vector<char>& A, std::vector<char>& B) {
  std::string word1;
  std::string word2;
  std::cout << "Enter the first word: ";
  std::cin >> word1;
  std::cout << "Enter the second word: ";
  std::cin >> word2;
  std::cout << '\n';
  for (int i = 0; i < word1.size(); i++) {
    word1[i] = tolower(word1[i]);
    A.push_back(word1[i]);
  }

  for (int i = 0; i < word2.size(); i++) {
    word2[i] = tolower(word2[i]);
    B.push_back(word2[i]);
  }
}
