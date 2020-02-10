#include <iostream>
#include <cassert>
#include "stat.hpp"

template <typename T>
class SinglyLinkedList {
    struct Node {
        Node *next{nullptr};
        T value;

        template <typename U>
        Node(U&& v): value(std::move(v)) {}
    };

    Node *head{nullptr};
    Node *tail{nullptr};
    size_t length{0};

    public:

    SinglyLinkedList() {}

    SinglyLinkedList(const SinglyLinkedList&) = delete;

    SinglyLinkedList(SinglyLinkedList&& rhs) noexcept {
        *this = std::move(rhs);
    }

    SinglyLinkedList& operator = (const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator = (SinglyLinkedList&& rhs) noexcept {
        head = rhs.head;
        tail = rhs.tail;
        length = rhs.length;
        rhs.head = nullptr;
        rhs.tail = nullptr;
        rhs.length = 0;
        return *this;
    }

    ~SinglyLinkedList() {
        while (head != nullptr) {
            auto *next = head->next;
            delete head;
            head = next;
        }
    }

    auto size() const {
        return length;
    }

    auto empty() const {
        return length == 0;
    }

    T& peek() {
        assert(head != nullptr);
        return head->value;
    }

    template <typename U>
    void add(U&& value) {
        if (tail == nullptr) {
            head = tail = new Node(std::move(value));
        } else {
            auto *node = new Node(std::move(value));
            tail->next = node;
            tail = node;
        }

        ++length;
    }

    T remove() {
        assert(head != nullptr);
        T val(std::move(head->value));
        Node *tmp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }

        delete tmp;
        --length;

        return val;
    }

    template <typename U>
    void push(U&& value) {
        auto *node = new Node(std::move(value));

        if (tail == nullptr) {
            head = tail = node;
        } else {
            node->next = head;
            head = node;
        }

        ++length;
    }

    T pop() {
        return remove();
    }
};

template <typename T, template<typename> typename List>
void test_list_add(List<T>& list, size_t count) {
    for (size_t i = 0; i != count; ++i) {
        list.add(T{i});
    }

    assert(list.size() == count);
    std::cout << "test_list_add > " << list.peek() << std::endl;
}

template <typename T, template<typename> typename List>
void test_list_remove(List<T>& list, size_t count) {
    for (size_t i = 0; i != count; ++i) {
        T c = list.remove();
        assert(c.value == i);
    }

    assert(list.empty());
    assert(list.size() == 0);
    std::cout << "test_list_remove > " << T{0} << std::endl;
}

template <typename T, template<typename> typename List>
void test_list_push(List<T>& list, size_t count) {
    for (size_t i = 0; i != count; ++i) {
        list.push(T{i});
    }

    assert(list.size() == count);
    std::cout << "test_list_push > " << list.peek() << std::endl;
}

template <typename T, template<typename> typename List>
void test_list_pop(List<T>& list, size_t count) {
    for (size_t i = 0; i != count; ++i) {
        T c = list.pop();
        assert(c.value == count - i - 1);
    }

    assert(list.empty());
    assert(list.size() == 0);
    std::cout << "test_list_pop > " << T{0} << std::endl;
}

void test_singly_linked_list() {
    std::cout << "SinglyLinkedList test" << std::endl;
    {
        SinglyLinkedList<cnt<0>> list;

        test_list_add(list, 7);
        test_list_remove(list, 7);
    }

    {
        SinglyLinkedList<cnt<1>> list;
        test_list_push(list, 7);
        test_list_pop(list, 7);
    }
}

void _main() {
    test_singly_linked_list();
}