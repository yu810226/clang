// RUN: %clang_cc1 -emit-llvm -g %s -o - | FileCheck %s
template<typename T> struct Identity {
  typedef T Type;
};

void f(Identity<int>::Type a) {}
void f(Identity<int> a) {}
void f(int& a) { }

template<typename T> struct A {
  A<T> *next;
};
void f(A<int>) { }

struct B { };

void f() {
  int B::*a = 0;
  void (B::*b)() = 0;
}

namespace EmptyNameCrash {
  struct A { A(); };
  typedef struct { A x; } B;
  B x;
}

// PR4890
namespace PR4890 {
  struct X {
    ~X();
  };

  X::~X() { }
}

namespace VirtualDtor {
  struct Y {
    virtual ~Y();
  };
  
  Y::~Y() { }
}

namespace VirtualBase {
  struct A { };
  struct B : virtual A { };

  void f() {
    B b;
  }
}

void foo() {
  const wchar_t c = L'x';
  wchar_t d = c;
}

namespace b5249287 {
template <typename T> class A {
  struct B;
};

class Cls {
  template <typename T> friend class A<T>::B;
};

Cls obj;
}

namespace pr9608 { // also pr9600
struct incomplete;
incomplete (*x)[3];
// CHECK: metadata [[INCARRAYPTR:![0-9]*]], i32 0, i32 1, [3 x i8]** @_ZN6pr96081xE, null} ; [ DW_TAG_variable ] [x]
// CHECK: [[INCARRAYPTR]] = {{.*}}metadata [[INCARRAY:![0-9]*]]} ; [ DW_TAG_pointer_type ]
// CHECK: [[INCARRAY]] = {{.*}}metadata [[INCTYPE:![0-9]*]], metadata {{![0-9]*}}, i32 0, i32 0} ; [ DW_TAG_array_type ] [line 0, size 0, align 0, offset 0] [from incomplete]
// CHECK: [[INCTYPE]] = {{.*}} ; [ DW_TAG_structure_type ] [incomplete]{{.*}} [fwd]
}
