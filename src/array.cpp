#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include "stat.hpp"

template <typename T>
class array {
    T *storage{nullptr};
    uint32_t length;
    public:
    array(uint32_t length): length(length) {
        storage = new T[length];
    }

    ~array() {
        delete [] storage;
    }

    T& operator [] (size_t idx) {
        return storage[idx];
    }

    uint32_t size() const {
        return length;
    }

    array(array&& rhs) noexcept {
        *this = rhs;
    }

    array& operator = (array&& rhs) noexcept {
        if (storage != nullptr) {
            delete [] storage;
        }

        storage = rhs.storage;
        length = rhs.length;
        rhs.storage = nullptr;
        rhs.length = 0;
        return *this;
    }

    array(const array&) = delete;
    array& operator = (const array&) = delete;
};

template <typename T>
class ArrayStack {
    protected:
    array<T> _array;
    uint32_t length{0};

    public:
    ArrayStack(): _array(4) {
    }

    ArrayStack(uint32_t capacity): _array(capacity) {
    }

    uint32_t size() const {
        return length;
    }

    T& get(size_t idx) {
        return _array[idx];
    }

    T set(size_t idx, const T& val) {
        T tmp = std::move(_array[idx]);
        _array[idx] = val;
        return tmp;
    }

    T set(size_t idx, T&& val) noexcept {
        T tmp = std::move(_array[idx]);
        _array[idx] = std::move(val);
        return tmp;
    }

    void add(size_t idx, T val) {
        if (length + 1 > _array.size()) {
            resize();
        }

        for (size_t i = length; i > idx; --i) {
            _array[i] = std::move(_array[i - 1]);
        }

        _array[idx] = std::move(val);
        ++length;
    }

    T remove(size_t idx) {
        T tmp = std::move(_array[idx]);

        for (size_t i = idx; i < length - 1; ++i) {
            _array[i] = std::move(_array[i + 1]);
        }

        --length;

        if (_array.size() >= 3 * length) {
            resize();
        }

        return tmp;
    }

    protected:
    virtual void resize() {
        uint32_t new_size = length + length / 2;
        array<T> new_array(new_size);
        for (size_t i = 0; i < length; ++i) {
            new_array[i] = std::move(_array[i]);
        }

        _array = std::move(new_array);
    }
};

template <typename T>
class FastArrayStack : public ArrayStack<T> {
    public:
    FastArrayStack() {}

    FastArrayStack(uint32_t capacity): ArrayStack<T>(capacity) {}

    protected:
    void resize() override {
        uint32_t new_size = this->length + this->length / 2;
        array<T> new_array(new_size);
        std::copy(&this->_array[0], &this->_array[this->length], &new_array[0]);
        // for (size_t i = 0; i < length; ++i) {
        //     new_array[i] = std::move(_array[i]);
        // }

        this->_array = std::move(new_array);
    }
};

template <typename T>
class ArrayQueue {
    array<T> _array;
    size_t start_idx{0};
    uint32_t length{0};
    public:

    ArrayQueue(): array(4) {}
    ArrayQueue(uint32_t capacity) : _array(capacity) {}

    template <typename U>
    void add(U&& val) {
        if (length + 1 > _array.size()) {
            resize();
        }

        _array[ (start_idx + length) % _array.size() ] = std::move(val);
        ++length;
    }

    T& get(size_t idx) {
        return _array[ (start_idx + idx) % _array.size() ];
    }

    bool empty() const {
        return length == 0;
    }

    auto size() const {
        return length;
    }

    T remove() {
        assert(length != 0);

        T ret(std::move(_array[start_idx]));

        start_idx = (start_idx + 1) % _array.size();
        --length;

        if (_array.size() >= 3 * length) {
            resize();
        }

        return ret;
    }

    private:
    void resize() {
        uint32_t new_size = length + length/2;
        array<T> new_array(new_size);

        for (size_t i = 0; i != length; ++i) {
            auto j = (start_idx + i) % _array.size();
            new_array[i] = std::move(_array[j]);
        }

        _array = std::move(new_array);
        start_idx = 0;
    }
};

template <typename T, template<typename> typename Array>
void test_add(Array<T>& stack, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        stack.add(i, T{i});
    }

    assert(stack.size() == count);
    for (size_t i = 0; i < count; ++i) {
        assert(stack.get(i).value == i);
    }
    std::cout << "test_add > " << stack.get(0) << std::endl;
}

template <typename T, template<typename> typename Array>
void test_set(Array<T>& stack) {
    stack.get(0) = T(23);
    stack.get(stack.size() - 1) = T(32);

    assert(stack.get(0).value == 23);
    assert(stack.get(stack.size() - 1).value == 32);

    std::cout << "test_set > " << stack.get(0) << std::endl;
}

template <typename T, template<typename> typename Array>
void test_remove(Array<T>& stack) {
    auto size = stack.size();
    stack.remove(0);
    stack.remove(stack.size()-1);

    assert(stack.size() == size - 2);

    for (size_t i = 0; i < stack.size(); ++i) {
        assert(stack.get(i).value == i + 1);
    }

    std::cout << "test_remove > " << stack.get(0) << std::endl;
}

void test_vector(size_t count) {
    std::vector<cnt<1>> stack;
    for (size_t i = 0; i < count; ++i) {
        stack.push_back(cnt<1>());
    }

    std::cout << "test_vector > " << stack[0] << std::endl;
}

void test_array_stack() {
    std::cout << "ArrayStack test" << std::endl;
    ArrayStack<cnt<0>> stack(4);

    test_add(stack, 6);
    test_vector(6);
    test_set(stack);
    test_remove(stack);
}

void test_fast_array_stack() {
    std::cout << "FastArrayStack test" << std::endl;
    FastArrayStack<cnt<2>> stack(4);

    test_add(stack, 6);
    test_vector(6);
    test_set(stack);
    test_remove(stack);
}

template <typename T, template <typename> typename Queue>
void test_queue_add(Queue<T>& queue, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        queue.add(T{i});
    }

    assert(queue.size() == count);
    for (size_t i = 0; i < count; ++i) {
        assert(queue.get(i).value == i);
    }
    std::cout << "test_queue_add > " << queue.get(0) << std::endl;
}

template <typename T, template<typename> typename Queue>
void test_queue_remove(Queue<T>& queue, size_t count) {
    assert(queue.size() == count);

    for (size_t i = 0; i < count; ++i) {
        assert(queue.remove().value == i);
    }

    assert(queue.size() == 0);

    std::cout << "test_queue_remove > " << T() << std::endl;
}

void test_queue() {
    std::cout << "ArrayQueue test" << std::endl;
    ArrayQueue<cnt<3>> queue(4);

    test_queue_add(queue, 6);
    test_queue_remove(queue, 6);
}

void _main() {
    test_array_stack();
    test_fast_array_stack();
    test_queue();
    // std::cout << cnt<0>() << std::endl;
}