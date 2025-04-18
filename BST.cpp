#include <stdexcept>

#include "BST.h"

BST::BST() {
  root = nullptr;
  size = 0;
}

void BST::freeTree(Node* node) {
  if (node) {
    freeTree(node->left);
    freeTree(node->right);
    delete node;
  }
}

BST::~BST() {
  freeTree(root);
}

// inserts a new node into the tree
BST::Node * BST::newNode(Node * node, std::string key, int value, bool& newValue) {
  if (node == nullptr) {
    newValue = true;
    return new Node(key, value);
  }

  if (key < node->key) {
    node->left = newNode(node->left, key, value, newValue);
  } else if (key > node->key) {
    node->right = newNode(node->right, key, value, newValue);
  } else {
    node->value = value;
    newValue = false;
  }

  return node;
}

// inserts a new node into the tree if the key is not found. otherwise just updates the value
void BST::set(std::string key, int value) {
  bool newValue = false;
  root = newNode(root, key, value, newValue);
  if (newValue) {
    size++;
  }
}

// finds a node in the tree by key
BST::Node * BST::findNode(Node * node, std::string key) {
  if (node == nullptr) {
    return nullptr;
  }

  if (key == node->key) {
    return node;
  } else if (key < node->key) {
    return findNode(node->left, key);
  } else {
    return findNode(node->right, key);
  }
}

// gets the value of a node by the key and throws an out of range error if its not found
int BST::at(std::string key) {
  Node * node = findNode(root, key);
  if (node == nullptr) {
    throw std::out_of_range(key);
  }

  return node->value;
}

// gets the key of a node by the index and returns an empty string if its not found
std::string BST::getKey(int value) {
    std::vector<std::string> result;
    findKeyHelper(root, value, result);
    if (result.empty()) {
        return "";
    } else {
      return result[0];
    }
}

// helper function for getKey
void BST::findKeyHelper(Node * node, int value, std::vector<std::string>& result) {
  if (!node) return;
  findKeyHelper(node->left, value, result);

  if (node->value == value) {
    result.push_back(node->key);
  }

  findKeyHelper(node->right, value, result);
}
