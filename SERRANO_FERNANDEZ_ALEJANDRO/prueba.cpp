#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector

int main () {
  int myints[] = {10,20,30,5,15};
  std::vector<int> v(myints,myints+5);

  std::make_heap (v.begin(),v.end(),std::greater<>{});
  std::cout << "initial min heap   : " << v.front() << '\n';

  std::pop_heap (v.begin(),v.end(),std::greater<>{}); v.pop_back();
  std::cout << "min heap after pop : " << v.front() << '\n';
  
  std::pop_heap (v.begin(),v.end(),std::greater<>{}); v.pop_back();
  std::cout << "min heap after pop : " << v.front() << '\n';

  v.push_back(99); std::make_heap (v.begin(),v.end(),std::greater<>{});
  std::cout << "max heap after push: " << v.front() << '\n';
/*
  std::sort_heap (v.begin(),v.end());

  std::cout << "final sorted range :";
  for (unsigned i=0; i<v.size(); i++)
    std::cout << ' ' << v[i];

  std::cout << '\n';
*/
  return 0;
}
