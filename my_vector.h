#include <exception>
#include <ostream>

template<class T>
class my_vector {
    T *array;
    int max_size;
    int current_size;
    bool is_slice;

public:
    int size() const {
        return current_size;
    }

    friend std::ostream &operator<<(std::ostream &os, const my_vector &vector) {
        for (int i = 0; i < vector.size(); ++i) {
            os << vector.array[i] << ", ";
        }
        return os;
    }

    bool empty() const {
        return size() == 0;
    }

    my_vector(const my_vector<T> &other) {
        max_size = other.max_size;
        current_size = other.current_size;
        is_slice = false;
        array = new T[other.max_size];
        for (int i = 0; i < current_size; ++i) {
            array[i] = other.array[i];
        }
    }

    explicit my_vector(int maxSize) : array(new T[maxSize]),max_size(maxSize) , current_size(0), is_slice(false){

    }

    my_vector() : array(new T[10]), max_size(10), current_size(0), is_slice(false) {
    }

    ~my_vector() {
        if (!is_slice)
            delete[] array;
    }

    void push_back(T t) {
        if (current_size == max_size) {
            max_size *= 2;
            T *tmp = new T[max_size];
            for (int i = 0; i < current_size; ++i) {
                tmp[i] = array[i];
            }
            delete[] array;
            array = tmp;
        }
        array[current_size++] = t;
    }

    my_vector<T> slice(int begin, int end) {
        if (begin > end)
            return my_vector<T>();
        my_vector<T> result(end - begin + 1);
        for (int i = begin; i <= end; ++i) {
            result.push_back(at(i));
        }
        return result;
    }


    T &at(int i) {
        return array[i];
    }

    const T &at(int i) const {
        return array[i];
    }

    bool is_sorted_ptr() const {
        for (int i = 0; i < size() - 1; ++i) {
            if (*at(i) > *at(i + 1))
                return false;
        }
        return true;
    }
};
