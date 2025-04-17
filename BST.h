#ifndef BST_H
#define BST_H

#include <string>
#include <vector>

class BST {
    private:
      struct Node {
        std::string key;
        int value;
        Node * left;
        Node * right;

        Node(std::string key, int value) : key(key), value(value), left(nullptr), right(nullptr) {}
      };

      Node * root;
      int size;
      void freeTree(Node *);
      void findKeyHelper(Node *, int, std::vector<std::string>& result);
      Node * newNode(Node *, std::string, int, bool&);
      Node * findNode(Node *, std::string);

    public:
        BST();
        ~BST();

        void set(std::string, int);
        int at(std::string);
        std::string getKey(int);
};

#endif