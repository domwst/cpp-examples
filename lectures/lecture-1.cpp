#include "../common/stopwatch.hpp"
#include "../common/run_stress.hpp"
#include "../common/print.hpp"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>
#include <array>
#include <sstream>

using namespace std;

size_t Rand() {
  static mt19937_64 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
  return rnd();
}

size_t Rand(size_t l, size_t r) {
  return Rand() % (r - l) + l;
}

void PtrExample() {
  int a = 1, b = 2;
  int* ptr = &a;
  print(*ptr);
  *ptr = 3;
  print(a);
  ptr = &b;
  print(*ptr);
  *ptr = 4;
  print(b);
}

void StaticArrayExample() {
  int a[5] = {1, 2, 3, 4, 5};
  int *ptr = a;
  print(*(a + 2));
  print(*ptr);
  print(*(ptr + 2));
  print(ptr[2]);
  // ptr[2] == *(ptr + 2) == *(2 + ptr) == 2[ptr]
}

void PtrFun() {
  int a[5] = {1, 2, 3, 4, 5};
  print(a[2]);
  print(2[a]);
}

void DynamicArrayExample() {
  int n = Rand(5, 15);
  int* arr = new int[n];
  for (int i = 0; i < n; ++i) {
    arr[i] = i * i;
  }
  // [arr, arr + n)
  print(n);
  int sum = accumulate(arr, arr + n, 0);
  print(sum);
  delete[] arr;
}

void Dynamic2dArrayExample() {
  int n = Rand(5, 15);
  int** arr = new int*[n];

  for (int i = 0; i < n; ++i) {
    arr[i] = new int[Rand(5, 15)];
  }

  arr[1][2] = 2;

  for (int i = 0; i < n; ++i) {
    delete[] arr[i];
  }

  delete[] arr;
}

void VectorExamples() {
  vector<int> v = {1, 2, 3, 2, 1};
  cout << "Start:" << endl;
  print(v);
  cout << endl;

  v.erase(v.begin() + 1);
  cout << "erase(v.begin() + 1)" << endl;
  print(v);
  cout << endl;

  v.push_back(4);
  cout << "push_back(4)" << endl;
  print(v);
  cout << endl;

  v.insert(v.begin() + 3, 5);
  cout << "insert(v.begin() + 3, 5)" << endl;
  print(v);
  cout << endl;

  v.insert(v.begin() + 1, {6, 7, 8});
  cout << "insert(v.begin() + 1, {6, 7, 8})" << endl;
  print(v);
  cout << endl;

  vector<int> elems = {9, 10, 11};
  // [v.begin(), v.end())
  v.insert(v.end() - 1, elems.begin(), elems.end());
  cout << "insert(v.end() - 1, elems.begin(), elems.end())" << endl;
  print(v);
  cout << endl;

  v.resize(v.size() + 2);
  cout << "resize(v.size() + 2)" << endl;
  print(v);
  cout << endl;

  v.resize(v.size() - 4);
  cout << "resize(v.size() - 4)" << endl;
  print(v);
}

void Vector2dExample() {
  vector<vector<int>> v(10, vector<int>(5, 0));
  v[1].resize(10);
}

void ReserveAdvantage() {
  constexpr int size = (1 << 24) + 1;
  size_t without_reserve, with_reserve;
  {
    vector<int> v;
    StopWatch cpu;
    for (int i = 0; i < size; ++i) {
      v.push_back(i);
    }
    cout << (without_reserve = cpu.ElapsedMillis()) << " ms";
  }
  {
    vector<int> v;
    StopWatch cpu;
    v.reserve(size);
    for (int i = 0; i < size; ++i) {
      v.push_back(i);
    }
    cout << ", " << (with_reserve = cpu.ElapsedMillis()) << " ms";
  }
  cout << ", " << fixed << setprecision(3) << (double)without_reserve / with_reserve;
  cout << " times faster" << endl;
}

void Lambdas() {
  auto print_1 = []() {
    cout << 1 << endl;
  };
  cout << "print_1()" << endl;
  print_1();
  cout << endl;

  auto add = [](int a, int b) {
    return a + b;
  };
  print(add(2, 3));
  cout << endl;

  int val = 1;
  auto inc_val = [&val]() {
    ++val;
  };
  cout << "Before inc_val()" << endl;
  print(val);
  inc_val();
  cout << "After inc_val()" << endl;
  print(val);
  cout << endl;

  int a{}, b{}, c{};
  cout << "Before init()" << endl;
  print(a, b, c);
  auto init = [&]() {
    a = 1;
    b = 2;
    c = 3;
  };
  init();
  cout << "After init()" << endl;
  print(a, b, c);
  cout << endl;

  auto print_old_a = [a]() {
    cout << a << endl;
  };
  a = 2;
  cout << "print_old_a()" << endl;
  print_old_a();

  auto naturals = [a = 0]() mutable {
    cout << a++ << endl;
  };
  cout << endl;
  naturals();
  naturals();
  print(a);
}

void RangeBasedFor() {
  constexpr int n = 10;
  {
    vector<int> v(n);
    for (auto& x: v) {
      x = Rand() % 20;
    }
    print(v);
    for (auto x: v) {
      x = Rand() % 20;
    }
    cout << "After non-reference for:" << endl;
    print(v);
    cout << endl;
  }
  {
    cout << "Vector of pairs initialization:" << endl;
    vector<pair<int, int>> v(n);
    for (auto& [a, b] : v) {
      a = Rand() % 20;
      b = Rand() % 20;
    }
    print(v);
    cout << endl;
  }
  {
    cout << "Vector of arrays initialization:" << endl;
    vector<array<int, 3>> v(n);
    for (auto& [a, b, c] : v) {
      a = Rand() % 20;
      b = Rand() % 20;
      c = Rand() % 20;
    }
    print(v);
    cout << endl;
  }
  {
    vector<tuple<int, double, string>> v(n);
    for (auto& [i, d, s] : v) {
      i = Rand() % 20;
      d = double(Rand() % 20) / 10;
      s = to_string(Rand() % 20);
    }
    cout << "Vector of tuples initialization" << endl;
    print(v);
  }
}

void Algorithm() {
  constexpr int n = 10;
  vector<int> v(n);
  iota(v.begin(), v.end(), 0);  // <numeric>
  mt19937 rnd(random_device{}());
  shuffle(v.begin(), v.end(), rnd);
  cout << "Random permutation:" << endl;
  print(v);
  cout << endl;

  generate(v.begin(), v.end(), [&rnd]() {
    return rnd() % 10;
  });
  cout << "Random sequence:" << endl;
  print(v);
  cout << endl;

  sort(v.begin(), v.end());
  cout << "Sorted random sequence:" << endl;
  print(v);
  cout << endl;

  sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;
  });  // sort(v.rbegin(), v.rend());
  cout << "Sorted in reversed order sequence:" << endl;
  print(v);
  cout << endl;

  reverse(v.begin(), v.end());
  cout << "Again sorted random sequence:" << endl;
  print(v);
  cout << endl;

  {
    auto it = unique(v.begin(), v.end());
    v.resize(it - v.begin());  // v.erase(it, v.end());
    cout << "Removed repeated elements:" << endl;
    print(v);
    cout << endl;
  }

  print(accumulate(v.begin(), v.end(), 0));
  cout << endl;

  vector<int> pref_sum(v.size() + 1, 0);
  partial_sum(v.begin(), v.end(), pref_sum.begin() + 1);
  print(pref_sum);

  {
    auto it = lower_bound(v.begin(), v.end(), 4);
    cout << "First element in v greater than or equal to 4 is at pos = " << it - v.begin() << endl;
    cout << endl;
  }
  {
    auto it = upper_bound(v.begin(), v.end(), 4);
    cout << "First element in v greater than 4 is at pos = " << it - v.begin() << endl;
    cout << endl;
  }

  vector<int> u(n);
  generate(u.begin(), u.end(), [&rnd]() {
    return rnd() % 10;
  });
  sort(u.begin(), u.end());

  print(u);
//  vector<int> merged(u.size() + v.size());
//  merge(v.begin(), v.end(), u.begin(), u.end(), merged.begin());
//  print(merged);

  vector<int> merged;
  merge(v.begin(), v.end(), u.begin(), u.end(), back_inserter(merged));
  print(merged);
  cout << endl;

  rotate(v.begin(), v.begin() + 2, v.end());
  cout << "Rotated sequence" << endl;
  print(v);
}

void SetExamples() {
  set<int> s = {3, 2, 1};
  print(s);
  cout << endl;

  s.insert(2);
  s.insert(5);
  print(s);
  cout << endl;

  print(s.count(3));
  print(s.contains(3));  // C++20
  cout << endl;

  {
    auto it = s.lower_bound(5);
    cout << "it = s.lower_bound(5)" << endl;
    print(*it);
    cout << endl;
  }
  {
    auto it = s.upper_bound(5);
    cout << "it = s.upper_bound(5)" << endl;
    print(it == s.end());
  }
}

void CustomSetComparator() {
  auto cmp = [](int a, int b) {
    return a > b;
  };
//  set<int, greater<int>> s({1, 2, 3});
  set<int, decltype(cmp)> s({1, 2, 3}, cmp);
  print(s);
  print(*s.lower_bound(5));
}

void MapExamples() {
  map<int, int> m = {{1, 2}, {5, 4}};
  print(m[1], m[5], m[0]);
  print(m.size());
  print(m);
}

int WrongGetOrZero(map<int, int>& m, int key) {
  return m[key];
}

int SlowGetOrZero(const map<int, int>& m, int key) {
  if (m.count(key)) {
    return m.at(key);
  }
  return 0;
}

int FastGetOrZero(const map<int, int>& m, int key) {
  if (auto it = m.find(key); it != m.end()) {
    return it->second;
  }
  return 0;
}

void StringStreamExamples() {
//  int a, b;
//  cin >> a >> b;

//  int val, sm = 0;
//  while (cin >> val) {
//    sm += val;
//  }

  stringstream ss;
  string text, text2, text3;
  getline(getline(cin, text) >> text2, text3);
  while (getline(cin, text)) {
  }
  ss << text;
  int v;
  while (ss >> v) {
    cout << v << endl;
  }
}

void RecurrentLambdas() {
  {
    int counter = 0;
    function<void(int)> f = [&counter, &f](int depth) {
      ++counter;
      if (depth != 0) {
        f(depth - 1);
        f(depth - 1);
      }
    };
    StopWatch cpu;
    f(24);
    cout << cpu.ElapsedMillis() << " ms, ";
  }
  {
    int counter = 0;
    auto f = [&counter](auto f, int depth) -> void {
      ++counter;
      if (depth != 0) {
        f(f, depth - 1);
        f(f, depth - 1);
      }
    };
    StopWatch cpu;
    f(f, 24);
    cout << cpu.ElapsedMillis() << " ms, ";
  }
  {
    int counter = 0;
    auto f = [&counter](auto& f, int depth) -> void {
      ++counter;
      if (depth != 0) {
        f(f, depth - 1);
        f(f, depth - 1);
      }
    };
    StopWatch cpu;
    f(f, 24);
    cout << cpu.ElapsedMillis() << " ms" << endl;
  }
}

int main() {
//  PtrExample();
//  StaticArrayExample();
//  PtrFun();
//  DynamicArrayExample();
//  Dynamic2dArrayExample();
//  VectorExamples();
//  RunStress(ReserveAdvantage);
//  Lambdas();
//  RangeBasedFor();
//  Algorithm();
//  SetExamples();
//  CustomSetComparator();
//  MapExamples();
//  StringStreamExamples();
//  RunStress(RecurrentLambdas);
}
