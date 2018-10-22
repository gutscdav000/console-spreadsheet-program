#include "cell.h"


int main() {

  // test regex_match
  const char str1[] = "A5";
  const char str2[] = "A500";  
  const char str3[] = "Z0";
  const char str4[] = "hello";
  const char str5[] = "1520";
  
  const char pattern[] = "[A-Z][0-9]+";
  const char pattern1[] = "[A-Za-z]+";
  const char pattern2[] = "[0-9]+";
  
  printf("regex test 1 %s\n", regex_match(str1, pattern) == 1? "PASSED": "FAILED");
  printf("regex test 2 %s\n", regex_match(str2, pattern) == 1? "PASSED": "FAILED");
  printf("regex test 3 %s\n", regex_match(str3, pattern) == 1? "PASSED": "FAILED");
  // no match
  printf("regex test 4 %s\n", regex_match(str4, pattern) == 0? "PASSED": "FAILED");
  printf("regex test 5 %s\n", regex_match(str4, pattern1) == 1? "PASSED": "FAILED");
  // no match
  printf("regex test 6 %s\n", regex_match(str2, pattern1) == 0? "PASSED": "FAILED");
  printf("regex test 7 %s\n", regex_match(str5, pattern2) == 1? "PASSED": "FAILED");
  // no match
  printf("regex test 8 %s\n", regex_match(str4, pattern2) == 0? "PASSED": "FAILED");
  printf("regex test 9 %s\n", regex_match("1", pattern2) == 1? "PASSED" : "FAILED");
  
  return 0;
}
