/* This is really stupid. Like, really stupid. I think a few people have made
 * fake/terrible Monad wrappers before, but I haven't read them. Which means
 * this is going to be even worse than they probably are I guess. But I wanted
 * to write this from my own understanding so screw it. */

template <class A, class B, template <class> class T>
struct Invoker {
    using Call     = T<B> (*)(Invoker<A, B, T> &, T<A>);
    using Original = T<B> (*)(A);
    Original func;
    Call     bind;

    T<B> operator()(T<A> value) {
        return this->bind(*this, value);
    }
};

template <class A, class B, template <class> class T>
T<B>
bind_impl(Invoker<A, B, T> &invoker, T<A> value);

template <class T>
struct Maybe;

template <class A, class B>
Maybe<B>
bind_impl(Invoker<A, B, Maybe> &invoker, Maybe<A> value);

template <class A, class B, template <class> class T>
Invoker<A, B, T>
bind(T<B> (*f)(A)) {
    Invoker<A, B, T> invoker;
    invoker.func = f;
    invoker.bind = (typename Invoker<A, B, T>::Call)&bind_impl;
    return invoker;
}

template <class A, template <class> class T>
T<A>
unit(A value) {
    return T<A>{value};
}






template <class T>
struct Maybe {
    enum  { Just,     Nothing              } kind;
    union { T value;  decltype(nullptr) n; } value;

    Maybe() : kind{Nothing}, value{0} {}
    Maybe(T value) : kind{Just}, value{value} {}
};

template <class A, class B>
Maybe<B>
bind_impl(Invoker<A, B, Maybe> &invoker, Maybe<A> value) {
    if(value.kind == Maybe<A>::Nothing) {
        Maybe<B> r;
        return r;
    }

    auto result = (invoker.func)(value.value.value);
    Maybe<B> r(result);
    return r;
};

template <class A>
Maybe<A>
unit(A value) {
    Maybe<A> r(value);
    return r;
}

Maybe<int>
add_while_not_five(int x) {
    if(x % 5 != 0) {
        return unit(x+1);
    }

    Maybe<int> r;
    return r;
}






int
main() {
    auto foo = Maybe<int>(101);
    auto bar = bind(add_while_not_five);
    while(true) {
        foo = bar(foo);

        if(foo.kind == Maybe<int>::Just) {
            __builtin_printf("Foo Value: %d\n", foo.value.value);
        } else {
            __builtin_printf("Not Even!\n");
            return 0;
        }
    }
}
