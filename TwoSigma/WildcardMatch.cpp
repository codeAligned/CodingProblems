#include <string>
#include <vector>
#include <cassert>

namespace RecursiveSolution {
  
  bool RecursiveCheck (const std::string & str,
                       const std::string & pattern,
                       std::size_t i,
                       std::size_t j) {
    if (i == str.size() && j == pattern.size()) {
      return true;
    } else if (j == pattern.size()) {
      return false;
    } else if (i == str.size()) {
      // skip all * left
      while(j<pattern.size() && pattern[j]=='*') {
        ++ j;
      }
      return j == pattern.size();
    } else {
      if (pattern[j] != '*') {
        return ( (str[i] == pattern[j] || pattern[j]=='?') )
          && RecursiveCheck(str,pattern,i+1,j+1);
      } else {
        return RecursiveCheck(str,pattern,i,j+1)
          || RecursiveCheck(str,pattern,i+1,j);
      }
      
    }
  }
  
  bool WildcardMatch(const std::string & str, const std::string & pattern) {
    return RecursiveCheck(str,pattern,0,0);
  }

  
}


namespace DynamicProgramming{
  bool WildcardMatch(const std::string & str, const std::string & pattern) {
    const std::size_t str_size(str.size()), pattern_size(pattern.size());
    std::vector< std::vector<bool> > is_match(str_size+1,std::vector<bool>(pattern_size+1,false));
    is_match[0][0] = true;
    for (std::size_t i=1;i<=pattern_size && pattern[i-1]=='*';++i) {
      is_match[0][i] = true;
    }

    for (std::size_t i=1;i<=str_size;++i) {
      for (std::size_t j=1;j<=pattern_size;++j) {
        if (pattern[j-1]=='*') {
          is_match[i][j] = is_match[i][j-1] // do nothing
            || is_match[i-1][j]; // match current char
        } else {
          is_match[i][j] = is_match[i-1][j-1]
            && (str[i-1]==pattern[j-1] || pattern[j-1]=='?');
        }
      }
    }
    
    return is_match[str_size][pattern_size];
  }
}

namespace TwoPointerSolution {
  bool WildcardMatch(const std::string & str, const std::string & pattern) {
    const std::size_t str_size(str.size()), pattern_size(pattern.size());
    std::size_t str_index(0),pattern_index(0);
    std::size_t str_marker(-1),pattern_marker(-1);

    while (str_index < str_size) {
      if (pattern[pattern_index]=='*') {
        // mark where * appears
        pattern_marker = pattern_index;
        str_marker = str_index;

        // at first let * match nothing
        ++ pattern_index; 
      } else {
        if ( str[str_index] == pattern[pattern_index]
             || pattern[pattern_index] == '?') {
          ++ pattern_index;
          ++ str_index;
        } else {
          if (pattern_marker == -1) {
            // no *
            return false;
          } else {
            str_index = ++ str_marker; // need increase str_marker here!
            pattern_index = pattern_marker + 1; 
          }
        }
      }
    }
    // Right now str_index == str_size,
    // we need make sure pattern_index can reach pattern_size
    while (pattern_index < pattern_size && pattern[pattern_index]=='*') ++ pattern_index;

    return pattern_index == pattern_size;
  }
}


void UnitTest() {
  using namespace RecursiveSolution;
  assert(WildcardMatch("aaa","a*")==true);
  assert(WildcardMatch("","*")==true);
  assert(WildcardMatch("abcadca","a*ca")==true);
  assert(WildcardMatch("dbbca","db*cae")==false);
  assert(WildcardMatch("dbbca","db?ca")==true);
  assert(WildcardMatch("","")==true);
  assert(WildcardMatch("ab","?*")==true);
  assert(WildcardMatch("aab","c*a*b")==false);
  assert(WildcardMatch("aa","aa")==true);
  assert(WildcardMatch("aaaaaaa","a*a")==true);
  assert(WildcardMatch("aaa","aa")==false);
  assert(WildcardMatch("aa","?")==false);
}

int main() {
  UnitTest();
  return 0;
}