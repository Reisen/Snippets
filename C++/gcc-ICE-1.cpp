/* Causes an internal compiler error on GCC<5.1. Just thought it was cool
 * running into this bug. It's fixed now, by which I mean it still errors but
 * not in the fuck you sort of way, but I'd like to remember what caused it
 * anyway.
 *
 * $ g++ -std=gnu++11 prog.cpp
 *
 *   prog.cpp: In instantiation of 'class outer<int>::inner':
 *   /usr/include/c++/4.9/bits/unique_ptr.h:74:22:   required from 'void std::default_delete<_Tp>::operator()(_Tp*) const [with _Tp = outer<int>::inner]'
 *   /usr/include/c++/4.9/bits/unique_ptr.h:236:16:   required from 'std::unique_ptr<_Tp, _Dp>::~unique_ptr() [with _Tp = outer<int>::inner; _Dp = std::default_delete<outer<int>::inner>]'
 *   prog.cpp:6:7:   required from here
 *   prog.cpp:16:23: internal compiler error: Segmentation fault
 *    class outer<T>::inner {};
 *                       ^
 */
#include <memory>
#include <algorithm>
using std::unique_ptr;

template <class T>
class outer {
  public:
  outer();

  private:
  class inner;
  unique_ptr<inner> _inner;
};

template <class T, class U>
class outer<T>::inner {};

template <class T>
outer<T>::outer() {}

int
main() {
  outer<int> example;
}
