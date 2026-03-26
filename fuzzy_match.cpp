#include <string_view>
#include <vector>
#include <algorithm>

int leveshtein_distance(std::string_view s1, std::string_view s2) {
  if(s2.size() > s1.size()) return leveshtein_distance(s2, s1);

  int m = s1.size();
  int n = s2.size();
  
  std::vector<int> prev_row(n+1);
  std::vector<int> curr_row(n+1);

  for(int i {0}; i < n + 1; i++ ) {
    prev_row[i] = i;
  }

  for(int i {1}; i < m + 1; i++) {
    
    curr_row[0] = i;

    for(int j {1}; j < n + 1; j++) {
      if(s1[i - 1] == s2[j - 1]) {
        curr_row[j] = prev_row[j - 1];
      } else {
        curr_row[j] = std::min({curr_row[j-1], prev_row[j], prev_row[j-1]}) + 1;
      }
    }
    
    std::swap(curr_row, prev_row);
  }
  return prev_row.back();
}

