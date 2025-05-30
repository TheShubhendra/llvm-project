// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc -DLOAD
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -o - -DLOAD | FileCheck %s
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -emit-pch -o %t
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -include-pch %t -o - -DLOAD | FileCheck %s

// RUN: %clang_cc1 -verify -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - -DOMP5 | FileCheck %s --check-prefix HOST5
// RUN: %clang_cc1 -verify -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc -DOMP5
// RUN: %clang_cc1 -verify -fopenmp -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -o - -DOMP5 | FileCheck %s --check-prefix DEV5

// RUN: %clang_cc1 -verify -fopenmp -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -o - -DOMP5 | FileCheck %s --check-prefix KMPC-ONLY

// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - -DOMP5 | FileCheck %s --check-prefix SIMD-ONLY
// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc -DOMP5
// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -o - -DOMP5 | FileCheck %s --check-prefix SIMD-ONLY

// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc -fopenmp-version=45
// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -o - -fopenmp-version=45 | FileCheck %s --check-prefix SIMD-ONLY
// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -emit-pch -o %t -fopenmp-version=45
// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -fopenmp-is-target-device -fvisibility=protected -fopenmp-host-ir-file-path %t-ppc-host.bc -include-pch %t -verify -o - -fopenmp-version=45 | FileCheck %s --check-prefix SIMD-ONLY

// expected-no-diagnostics

// SIMD-ONLY-NOT: {{__kmpc|__tgt}}
// KMPC-ONLY-NOT: __tgt

// CHECK-NOT: define {{.*}}{{baz1|baz4|maini1|Base|virtual_}}
// CHECK-DAG: Bake
// CHECK-NOT: @{{hhh|ggg|fff|eee}} =
// CHECK-DAG: @flag = protected global i8 undef,
// CHECK-DAG: @dx = {{protected | }}global i32 0,
// CHECK-DAG: @dy = {{protected | }}global i32 0,
// CHECK-DAG: @bbb = {{protected | }}global i32 0,
// CHECK-DAG: weak constant %struct.__tgt_offload_entry {
// CHECK-DAG: @ccc = external global i32,
// CHECK-DAG: @ddd = {{protected | }}global i32 0,
// CHECK-DAG: @hhh_decl_tgt_ref_ptr = weak global ptr null
// CHECK-DAG: @ggg_decl_tgt_ref_ptr = weak global ptr null
// CHECK-DAG: @fff_decl_tgt_ref_ptr = weak global ptr null
// CHECK-DAG: @eee_decl_tgt_ref_ptr = weak global ptr null
// CHECK-DAG: @{{.*}}maini1{{.*}}aaa = internal global i64 23,
// CHECK-DAG: @pair = {{.*}}addrspace(3) global %struct.PAIR undef
// CHECK-DAG: @_ZN2SS3SSSE ={{ protected | }}global i32 1,
// CHECK-DAG: @b ={{ protected | }}global i32 15,
// CHECK-DAG: @d ={{ protected | }}global i32 0,
// CHECK-DAG: @c = external global i32,
// CHECK-DAG: @globals ={{ protected | }}global %struct.S zeroinitializer,
// CHECK-DAG: [[STAT:@.+stat]] = internal global %struct.S zeroinitializer,
// CHECK-DAG: [[STAT_REF:@.+]] = internal constant ptr [[STAT]]
// CHECK-DAG: @out_decl_target ={{ protected | }}global i32 0,
// CHECK-DAG: @llvm.compiler.used = appending global [1 x ptr] [ptr [[STAT_REF]]],

// CHECK-DAG: define {{.*}}i32 @{{.*}}{{foo|bar|baz2|baz3|FA|f_method}}{{.*}}()
// CHECK-DAG: define {{.*}}void @{{.*}}TemplateClass{{.*}}(ptr {{[^,]*}} %{{.*}})
// CHECK-DAG: define {{.*}}i32 @{{.*}}TemplateClass{{.*}}f_method{{.*}}(ptr {{[^,]*}} %{{.*}})

#ifndef HEADER
#define HEADER

int dx = 0;
extern int dx;
#pragma omp declare target to(dx)

int dy = 0;
#pragma omp begin declare target

extern int dy;
#pragma omp end declare target

#pragma omp declare target
bool flag [[clang::loader_uninitialized]];
extern int bbb;
#pragma omp end declare target
#pragma omp declare target
extern int bbb;
#pragma omp end declare target

#pragma omp declare target
extern int aaa;
int bbb = 0;
extern int ccc;
int ddd = ccc;
#pragma omp end declare target

#pragma omp declare target
extern int bbb;
#pragma omp end declare target

extern int eee;
int fff = 0;
extern int ggg;
int hhh = 0;
#pragma omp declare target link(eee, fff, ggg, hhh)

int out_decl_target = 0;
#ifdef OMP5
#pragma omp declare target(out_decl_target)
#endif

#pragma omp declare target
void lambda () {
#ifdef __cpp_lambdas
  (void)[&] { (void)out_decl_target; };
#else
#pragma clang __debug captured
  {
    (void)out_decl_target;
  }
#endif
};
#pragma omp end declare target

template <typename T>
class TemplateClass {
  T a;
public:
  TemplateClass() {}
  T f_method() const { return a; }
};

int foo();

static int baz1() { return 0; }

int baz2();

int baz4() { return 5; }

template <typename T>
T FA() {
  TemplateClass<T> s;
  return s.f_method();
}

#pragma omp declare target
struct S {
  int a;
  S(int a) : a(a) {}
};

int foo() { return 0; }
int b = 15;
int d;
S globals(d);
static S stat(d);
#pragma omp end declare target
int c;

int bar() { return 1 + foo() + bar() + baz1() + baz2(); }

int maini1() {
  int a;
  static long aa = 32 + bbb + ccc + fff + ggg;
// CHECK-DAG: define weak_odr protected void @__omp_offloading_{{.*}}maini1{{.*}}_l[[@LINE+1]](ptr {{.*}}, ptr noundef nonnull align {{[0-9]+}} dereferenceable({{[0-9]+}}) %{{.*}}, i64 {{.*}}, i64 {{.*}})
#pragma omp target map(tofrom \
                       : a, b)
  {
    S s(a);
    static long aaa = 23;
    a = foo() + bar() + b + c + d + aa + aaa + FA<int>();
  }
  return baz4();
}

int baz3() { return 2 + baz2(); }
int baz2() {
// CHECK-DAG: define weak_odr protected void @__omp_offloading_{{.*}}baz2{{.*}}_l[[@LINE+1]](ptr {{.*}}, i64 {{.*}})
#pragma omp target parallel
  ++c;
  return 2 + baz3();
}

extern int create() throw();

static __typeof(create) __t_create __attribute__((__weakref__("__create")));

int baz5() {
  bool a;
// CHECK-DAG: define weak_odr protected void @__omp_offloading_{{.*}}baz5{{.*}}_l[[@LINE+1]](ptr {{.*}}, i64 {{.*}})
#pragma omp target
  a = __extension__(void *) & __t_create != 0;
  return a;
}

template <typename T>
struct Base {
  virtual ~Base() {}
};

template class Base<int>;

template <typename T>
struct Bake {
  virtual ~Bake() {}
};

#pragma omp declare target
template class Bake<int>;
#pragma omp end declare target

struct BaseNonT {
  virtual ~BaseNonT() {}
};

#pragma omp declare target
struct BakeNonT {
  virtual ~BakeNonT() {}
};
#pragma omp end declare target

template <typename T>
struct B {
  virtual void virtual_foo();
};

void new_bar() { new B<int>(); }

template <typename T>
void B<T>::virtual_foo() {
#pragma omp target
  {}
}

struct A {
  virtual void emitted() {}
};

template <typename T>
struct C : public A {
  virtual void emitted();
};

template <typename T>
void C<T>::emitted() {
#pragma omp target
  {}
}

int main() {
  A *X = new C<int>();
  X->emitted();
  return 0;
}

// CHECK-DAG: define {{.*}}void @__omp_offloading_{{.*}}virtual_foo{{.*}}_l[[@LINE-25]](ptr {{.*}})
// CHECK-DAG: define {{.*}}void @__omp_offloading_{{.*}}emitted{{.*}}_l[[@LINE-11]](ptr {{.*}})

template <typename T>
struct TTT {
  virtual void emitted() {
#pragma omp target
  {}
  }
};

// CHECK-DAG: define {{.*}}void @__omp_offloading_{{.*}}emitted{{.*}}_l[[@LINE-5]](ptr {{.*}})

// CHECK-DAG: declare extern_weak noundef signext i32 @__create()

// CHECK-NOT: define {{.*}}{{baz1|baz4|maini1|Base|virtual_}}

// CHECK-DAG: !{{{.+}}virtual_foo

#ifdef OMP5
void host_fun() {}
#pragma omp declare target to(host_fun) device_type(host)
void device_fun() {}
#pragma omp declare target to(device_fun) device_type(nohost)
// HOST5-NOT: define {{.*}}void {{.*}}device_fun{{.*}}
// HOST5: define {{.*}}void {{.*}}host_fun{{.*}}
// HOST5-NOT: define {{.*}}void {{.*}}device_fun{{.*}}

// DEV5-NOT: define {{.*}}void {{.*}}host_fun{{.*}}
// DEV5: define {{.*}}void {{.*}}device_fun{{.*}}
// DEV5-NOT: define {{.*}}void {{.*}}host_fun{{.*}}
#endif // OMP5

struct PAIR {
  int a;
  int b;
};

#pragma omp declare target
PAIR pair __attribute__((address_space(3), loader_uninitialized));
#pragma omp end declare target

#endif // HEADER

#ifdef LOAD
#pragma omp declare target
void new_bar1() {
  TTT<int> *X = new TTT<int>();
  X->emitted();
}
#pragma omp end declare target

struct SS {
#pragma omp declare target
  static int SSS;
#pragma omp end declare target
};
int SS::SSS = 1;

#endif
