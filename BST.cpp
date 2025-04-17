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

void BST::set(std::string key, int value) {
  bool newValue = false;
  root = newNode(root, key, value, newValue);
  if (newValue) {
    size++;
  }
}

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

int BST::at(std::string key) {
  Node * node = findNode(root, key);
  if (node == nullptr) {
    throw std::out_of_range(key);
  }

  return node->value;
}

std::string BST::getKey(int value) {
    std::vector<std::string> result;
    findKeyHelper(root, value, result);
    if (result.empty()) {
        return "";
    } else {
      return result[0];
    }
}

void BST::findKeyHelper(Node * node, int value, std::vector<std::string>& result) {
  if (!node) return;
  findKeyHelper(node->left, value, result);

  if (node->value == value) {
    result.push_back(node->key);
  }

  findKeyHelper(node->right, value, result);
}
