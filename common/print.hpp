#pragma once

#include <ostream>
#include <map>
#include <set>

#define print_stream(stream, ...)         \
do {                                      \
  stream << "[" << #__VA_ARGS__ << "]: "; \
  print_to(stream, ##__VA_ARGS__);        \
  stream << std::endl;                    \
} while (0)

#define print(...) print_stream(std::cout, __VA_ARGS__)

template<class T>
std::enable_if_t<
  std::is_integral_v<T> ||
  std::is_floating_point_v<T>>
print_single(std::ostream& out, T arg) {
  out << arg;
}

void print_single(std::ostream& out, const std::string& s) {
  out << "\"" << s << "\"";
}

template<class T>
void print_single(std::ostream& out, T* ptr) {
  out << ptr;
}


template<class T>
void print_single(std::ostream& out, const std::vector<T>& v);

template<class T, class U>
void print_single(std::ostream& out, const std::pair<T, U>& v);

template<class T, size_t N>
void print_single(std::ostream& out, const std::array<T, N>& v);

template<class... Types>
void print_single(std::ostream& out, const std::tuple<Types...>& v);


template<class T>
void print_single(std::ostream& out, const std::vector<T>& v) {
  out << "[";
  bool is_first = true;
  for (auto& x : v) {
    if (!is_first) {
      out << ", ";
    }
    is_first = false;
    print_single(out, x);
  }
  out << "]";
}

template<class T, class U>
void print_single(std::ostream& out, const std::pair<T, U>& v) {
  out << "(";
  print_single(out, v.first);
  out << ", ";
  print_single(out, v.second);
  out << ")";
}

template<class T, size_t N>
void print_single(std::ostream& out, const std::array<T, N>& v) {
  out << "[";
  bool is_first = true;
  for (auto& x : v) {
    if (!is_first) {
      out << ", ";
    }
    is_first = false;
    out << x;
  }
  out << "]";
}

template<class... Types>
void print_single(std::ostream& out, const std::tuple<Types...>& v) {
  out << "<";
  out << get<0>(v);
  [&v, &out]<size_t... i>(std::index_sequence<i...>) {
    ((out << ", ", print_single(out, get<i + 1>(v))), ...);
  }(std::make_index_sequence<sizeof...(Types) - 1>{});
  out << ">";
}

template<class TKey, class TValue, class TCmp, class TAlloc>
void print_single(std::ostream& out, const std::map<TKey, TValue, TCmp, TAlloc>& m) {
  out << "{";
  bool is_first = true;
  for (auto& [key, value] : m) {
    if (!is_first) {
      out << ", ";
    }
    is_first = false;
    print_single(out, key);
    out << ": ";
    print_single(out, value);
  }
  out << "}";
}

template<class TKey, class TCmp, class TAlloc>
void print_single(std::ostream& out, const std::set<TKey, TCmp, TAlloc>& s) {
  out << "{";
  bool is_first = true;
  for (auto& x : s) {
    if (!is_first) {
      out << ", ";
    }
    is_first = false;
    print_single(out, x);
  }
  out << "}";
}

void print_to(std::ostream& out, auto FirstArgs, auto... RestOfArgs) {
  print_single(out, FirstArgs);
  if constexpr (sizeof...(RestOfArgs) != 0) {
    out << " ";
  }
  if constexpr (sizeof...(RestOfArgs) != 0) {
    print_to(out, RestOfArgs...);
  }
}
