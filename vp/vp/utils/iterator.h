#ifndef VP_ITERATOR
#define VP_ITERATOR

#include <utility>
#include <boost/function.hpp>
#include <cstdlib>

namespace vp {
template<typename T1, typename T2, bool>
struct chooseType {
    typedef T2 type;
};

template<typename T, typename R>
class secondFunc {
private:
    typedef R (fun)(const typename T::second_type &);
    typedef boost::function<fun> func;
    func f_;
public:
    secondFunc(func f) :
            f_(f) {
    }

    R operator()(const T & pair) {
        return f_(pair.second);
    }
};

template<typename T1, typename T2>
struct chooseType<T1, T2, true> {
    typedef T1 type;
};

template<typename T>
struct stripPointer {
    typedef T type;
};

template<typename T>
struct stripPointer<T *> {
    typedef T type;
};
template<typename T>
struct stripPointer<T &> {
    typedef T type;
};

template<typename T, typename R>
class pseudoContainer {
private:
    const T & container_;
    typedef typename T::value_type type;
    //typedef typename stripPointer<typename T::value_type>::type type;

    typedef R fun(type);
    typedef boost::function<fun> func;
    func f_;

    template<typename IT, bool C>
    class base_iterator {
    private:
        typedef typename pseudoContainer::func func;
        IT it_;
        func f_;
        typedef R btype;
        //typedef typename stripPointer<R>::type btype;
    public:

        typedef typename chooseType<const btype, btype, C>::type value_type;
        //typedef value_type & reference;
        //typedef value_type * pointer;

        base_iterator(IT it, func f) :
                it_(it), f_(f) {
        }

        base_iterator() {
        }

        base_iterator & operator++() {
            ++it_;
            return *this;
        }

        base_iterator operator++(int) {
            base_iterator ret(*this);
            ++it_;
            return ret;
        }

        value_type operator*() {
            return f_(*it_);
        }

        /*pointer operator->(){
         return &(*this);
         }*/

        const IT & iterator() const {
            return it_;
        }

    };

public:
    typedef base_iterator<typename T::const_iterator, true> const_iterator;

    pseudoContainer(const T & container, func f) :
            container_(container), f_(f) {
    }
    /*
     iterator begin(){
     return iterator(t_.begin(), member_);
     }

     iterator end(){
     return iterator(t_.end(), member_)
     }*/

    const_iterator begin() const {
        return const_iterator(container_.begin(), f_);
    }

    const_iterator end() const {
        return const_iterator(container_.end(), f_);
    }

};

/*
 template<typename T, typename R>
 bool operator==(const typename pseudoContainer<T, R>::const_iterator & left, const typename pseudoContainer<T, R>::const_iterator & right){
 return left.iterator() == right.iterator();
 }*/

template<typename T>
bool operator==(const T & left, const T & right) {
    return left.iterator() == right.iterator();
}

template<class T> bool operator!=(const T& x, const T& y) {
    return !(x == y);
}

}
;

#endif
