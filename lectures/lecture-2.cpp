#include "../common/c.hpp"
#include "../common/print.hpp"
#include "../common/random.hpp"

#include <iostream>
#include <optional>
#include <memory>
#include <vector>

using namespace std;

void UniquePtrExample() {
  unique_ptr<C> ptr(new C);
//  auto ptr2 = ptr;  // CE
  new C;
};

void MakeUniqueExample() {
  auto ptr = make_unique<C>();
  print(ptr.get());
}

void UniquePtrMoveExample() {
  auto ptr = make_unique<C>();
  print(ptr.get());
  auto ptr2 = move(ptr);
  print(ptr.get(), ptr2.get());

  auto MakeUniqueC = []() {
    return make_unique<C>();
  };
  auto ptr3 = MakeUniqueC();
}

void MoveExample1() {
  {
    C a;
    C b;
    b = a;  // copy
  }
  {
    C a;
    C b;
    b = move(a);  // move
  }
}

void MoveExample2() {
  auto MakeC = []() {
    return C();  // move?
  };
  auto a = MakeC();
}

void MoveExample3() {
  auto MakeC = []() {
    C a;
    C b;
    if (Rand() & 1) {
      return a;  // move
    } else {
      return b;  // move
    }
  };
  auto a = MakeC();
}

void ExceptionsExample() {
  class DivisionByZeroException : public exception {
//   public:
//    [[nodiscard]] const char* what() const noexcept override {
//      return "Division by zero";
//    }
  };

  auto Divide = [](int a, int b) -> int{
    if (b == 0) {
      C tmp;
      throw DivisionByZeroException();
    }
    return a / b;
  };

  int a = Rand() % 3, b = Rand() % 3;
  try {
    print(Divide(a, b));
  } catch (exception& e) {
    cout << "Exception caught: " << e.what() << endl;
  }
}

void ConstMethodExample() {
  class C {
   public:
    void OnlyConst() const {
      cout << "OnlyConst" << endl;
    }

    void OnlyNonConst() {
      cout << "OnlyNonConst" << endl;
    }

    void Both() {
      cout << "Both" << endl;
    }

    void Both() const {
      cout << "Both const" << endl;
    }
  };

  cout << "Output for a:" << endl;
  C a;
  a.OnlyConst();
  a.OnlyNonConst();
  a.Both();
  cout << endl;

  cout << "Output for b:" << endl;
  const C b;
  b.OnlyConst();;
//  b.OnlyNonConst();
  b.Both();
}

void ConstructorDestructorExample() {
  class Vector {
   public:
    Vector(int x, int y) : x_(new int(x)), y_(new int(y)) {  // RAII
    }

    Vector() : Vector(0, 0) {
    }

//    Vector(const Vector& other) = default;
    Vector(const Vector& other) : Vector(*other.x_, *other.y_) {
    }

    Vector(Vector&& other) {  // MISTAKE HERE
      swap(x_, other.x_);
      swap(y_, other.y_);
    }

    Vector& operator=(const Vector& other) {
      *x_ = *other.x_;
      *y_ = *other.y_;
      return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
      swap(x_, other.x_);
      swap(y_, other.y_);
      return *this;
    }

    Vector& operator-=(const Vector& other) {
      *x_ -= *other.x_;
      *y_ -= *other.y_;
      return *this;
    }

    int64_t operator*(const Vector& other) const {
      return 1ll * *x_ * *other.y_ - 1ll * *y_ * *other.x_;
    }

    int64_t operator,(const Vector& other) const {
      return 1ll * *x_ * *other.x_ + 1ll * *x_ * *other.x_;
    }  // (v1, v2) -> scalar product

    Vector operator-() const {
      return Vector(-*x_, -*y_);
    }

    Vector operator+(const Vector& other) const {
      return Vector(*x_ + *other.x_, *y_ + *other.y_);
    }

    int x() const {
      return *x_;
    }

    int y() const {
      return *y_;
    }

    ~Vector() {
      delete x_;
      delete y_;
    }

   private:
    int* x_;
    int* y_;
  };

  Vector v1(1, 2), v2(2, 3);
  auto v3 = v1 + v2;
  cout << v3.x() << " " << v3.y() << endl;
}

void RealNew() {
  auto New = []<class T>() {
    void* mem = ::operator new(sizeof(T));
    new (mem) T();
    return (T*)mem;
  };
  auto a = New.operator()<C>();
  delete a;
}

void RealDelete() {
  auto Delete = []<class T>(T* ptr) {
    ptr->~T();
    operator delete(ptr);
  };
  auto a = new C();
  Delete(a);
}

int main() {
//  UniquePtrExamples();
//  MakeUniqueExample();
//  UniquePtrMoveExample();
//  MoveExample1();
//  MoveExample2();
//  MoveExample3();
//  ExceptionsExample();
//  ConstMethodExample();
//  ConstructorDestructorExample();
//  RealNew();
//  RealDelete();
}
