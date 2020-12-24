#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_set>

template <typename T>
struct tree_node {
    T value{};
    tree_node *left{nullptr};
    tree_node *right{nullptr};

    tree_node() {}

    tree_node(T val): value(std::move(val)) {}

    tree_node(T val, tree_node *left, tree_node *right): value(std::move(val)), left(left), right(right) {}
};

template <typename T>
using visitor = std::function<void (tree_node<T> *)>;

// (root, left, right)
template <typename T>
static void recursive_preorder(tree_node<T> *root, visitor<T> func) {
    if (root != nullptr) {
        func(root);
        recursive_preorder(root->left, func);
        recursive_preorder(root->right, func);
    }
}

// (left, root, right)
template <typename T>
static void recursive_inorder(tree_node<T> *root, visitor<T> func) {
    if (root != nullptr) {
        recursive_inorder(root->left, func);
        func(root);
        recursive_inorder(root->right, func);
    }
}

// (left, right, root)
template <typename T>
static void recursive_postorder(tree_node<T> *root, visitor<T> func) {
    if (root != nullptr) {
        recursive_postorder(root->left, func);
        recursive_postorder(root->right, func);
        func(root);
    }
}

template <typename T>
static void run_traverse(const char *name, 
                                  std::function<void (tree_node<T>*, visitor<T>)> traverse,
                                  visitor<T> visitor,
                                  tree_node<T> *root) {
    std::cout << "run " << name << std::endl;
    traverse(root, visitor);
    std:: cout << std::endl <<"====== end ======" << std::endl;
}

// (root, left, right)
template <typename T>
static void iterative_preorder(tree_node<T> *root, visitor<T> func) {
    std::stack<tree_node<T> *> stack;

    stack.push(root);

    while (!stack.empty()) {
        auto *node = stack.top();
        stack.pop();

        if (node != nullptr) {
            func(node);
            stack.push(node->right);
            stack.push(node->left);
        }
    }
}

// (left, root, right)
template <typename T>
static void iterative_inorder_with_marking(tree_node<T> *root, visitor<T> func) {
    std::stack<tree_node<T> *> stack;
    std::unordered_set<tree_node<T> *> visited;
    // auto *node = root;

    stack.push(root);

    while (!stack.empty()) {
        auto *node = stack.top();

        if (node == nullptr) {
            // std::cout << "got nullptr" << endl;
            stack.pop();
            continue;
        }
        
        if (node->left && visited.find(node->left) == visited.end()) {
            stack.push(node->left);
        } else {
            stack.pop();
            func(node);
            visited.emplace(node);
            stack.push(node->right);
        }
    }
}

// (left, root, right)
template <typename T>
static void iterative_inorder(tree_node<T> *root, visitor<T> func) {
    std::stack<tree_node<T> *> stack;
    auto *node = root;

    stack.push(root);

    while (!stack.empty()) {
        if (!stack.top()) {
            stack.pop();
            continue;
        }

        if (node) {
            // folow left child to the leaf
            stack.push(node->left);
            node = node->left;
        } else {
            node = stack.top();
            stack.pop();
            func(node);
            stack.push(node->right);
            node = node->right;
        }        
    }
}

template <typename T>
static void morris_inorder(tree_node<T> *root, visitor<T> func) {
    tree_node<T> *curr = root;
    tree_node<T> *prev = nullptr;

    while (curr) {
        if (curr->left == nullptr) {
            func(curr);
            curr = curr->right;
        } else {
            prev = curr->left;

            while (prev->right != nullptr && prev->right != curr) {
                prev = prev->right;
            }

            if (prev->right == nullptr) {
                prev->right = curr;
                curr = curr->left;
            } else {
                prev->right = nullptr;
                func(curr);
                curr = curr->right;
            }
        }
    }
}

// (left, right, root)
template <typename T>
static void iterative_postorder(tree_node<T> *root, visitor<T> func) {
    std::stack<tree_node<T> *> stack;

    auto *node = root;
    stack.push(root);

    while (!stack.empty()) {

        if (node) {
            tree_node<T> *follow = nullptr;

            // follow left child to the leaf
            if (node->left) {
                stack.push(node->left);
                follow = node->left;
            } else if (node->right) {
                // if no left child then follow right child
                stack.push(node->right);
                follow = node->right;
            }

            node = follow;
        } else {
            auto *top = stack.top();
            stack.pop();

            func(top);

            if (!stack.empty() && (stack.top()->left == top)) {
                if (stack.top()->right) {
                    stack.push(stack.top()->right);
                    node = stack.top();
                }
            }
        }        
    }
}

static tree_node<int>* node(int val) {
    return new tree_node<int>(val);
}

static tree_node<int>* node(int val, tree_node<int> *left, tree_node<int> *right) {
    return new tree_node<int>(val, left, right);
}

void _main() {
    auto *root = node(1, 
                      node(2, 
                           node(4), 
                           node(5)), 
                      node(3, 
                           nullptr,
                           node(6,
                                node(7),
                                nullptr)));

    auto printer = [] (auto *node) { std::cout << node->value << " "; };
    run_traverse<int>("r preorder (root, left, right)", recursive_preorder<int>, printer, root);
    run_traverse<int>("r inorder (left, root, right)", recursive_inorder<int>, printer, root);
    run_traverse<int>("r postorder (left, right, root)", recursive_postorder<int>, printer, root);
    
    run_traverse<int>("i preorder (root, left, right)", iterative_preorder<int>, printer, root);
    run_traverse<int>("i inorder marking (left, root, right)", iterative_inorder_with_marking<int>, printer, root);
    run_traverse<int>("i inorder (left, root, right)", iterative_inorder<int>, printer, root);
    run_traverse<int>("i morris inorder (left, root, right)", morris_inorder<int>, printer, root);
    run_traverse<int>("i postorder (left, right, root)", iterative_postorder<int>, printer, root);
    
    recursive_postorder<int>(root, [](auto *node) { delete node; });
}