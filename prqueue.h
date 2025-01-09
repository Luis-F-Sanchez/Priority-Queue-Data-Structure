#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
 public:
  // within prqueue
  struct NODE {
    int priority;
    T value;
    NODE* parent;
    NODE* left;
    NODE* right;
    NODE* link;  // Link to duplicates -- Part 2 only
  };

 private:
  NODE* root;
  size_t sz;

  // Utility pointers for begin and next.
  NODE* curr;
  NODE* temp;  // Optional

  // TODO_STUDENT: add private helper function definitions here
  // resursive function for clear()
  void clearRecur(NODE* node) {
    if (node == nullptr) return;

    // delete nodes in post order traversale
    clearRecur(node->left);
    clearRecur(node->right);
    NODE* temp = node;
    while (temp != nullptr) {
      NODE* next = temp->link;  // save node in linked list
      delete temp;              // delete the current node
      temp = next;              // move to next node
    }
    // delete node;
  }

  void inorderTraversal(NODE* root, ostringstream& ss) const {
    if (root == nullptr) {
      return;
    }

    inorderTraversal(root->left, ss);
    // ss << root->priority << " value: " << root->value << endl;

    NODE* current = root;  // copy of our node
    while (current != nullptr) {
      ss << current->priority << " value: " << current->value
         << endl;  // add info to our SS
      // move through linked list, if any, otherwise currrent is nullptr and
      // moves to next node
      current = current->link;
    }

    inorderTraversal(root->right, ss);
  }

  NODE* copyHelp(const NODE* copiedNode, NODE* parent) {
    if (copiedNode == nullptr) {
      return nullptr;
    }

    // make new node copy data
    NODE* newNode = new NODE();
    newNode->value = copiedNode->value;
    newNode->priority = copiedNode->priority;
    newNode->parent = parent;
    newNode->left = nullptr;   // copiedNode->left
    newNode->right = nullptr;  // copiedNode->right

    // copy over items in linked list
    NODE* otherLink = copiedNode->link;  // save node link
    NODE* head = nullptr;                // head ptr
    NODE* tail = nullptr;                // tail ptr
    while (otherLink != nullptr) {
      NODE* listNode = new NODE();               // make a new node for list
      listNode->value = otherLink->value;        // copy the next node's value
      listNode->priority = otherLink->priority;  // copy node's prioity
      listNode->link = nullptr;  // code doesn't work without this

      // this is just to make adding nodes easier
      if (head == nullptr) {
        head = listNode;  // set the head
        tail = listNode;  // set tail to head for 1st node
      } else {
        tail->link = listNode;  // add new node list
        tail = listNode;        // update tail
      }

      sz++;
      otherLink = otherLink->link;  // go to next node in link
    }
    newNode->link = head;  // set our created list

    // travel in order
    newNode->left = copyHelp(copiedNode->left, newNode);
    newNode->right = copyHelp(copiedNode->right, newNode);

    sz++;
    return newNode;
  }

  bool equalTravel(const NODE* node1, const NODE* node2) const {
    if (node1 == nullptr &&
        node2 == nullptr) {  // check left and right nodes for a tree
      return true;
    }

    if (node1 == nullptr || node2 == nullptr) {  // check if one node is null
      return false;
    }

    // compair values + prioty
    if (node1->value != node2->value || node1->priority != node2->priority) {
      return false;
    }

    // compare linked lists
    NODE* link1 = node1->link;  // get link
    NODE* link2 = node2->link;  // get link
    while (link1 != nullptr && link2 != nullptr) {
      // check values of each node in linked list
      if (link1->value != link2->value || link1->priority != link2->priority) {
        return false;
      }
      // go to next node
      link1 = link1->link;
      link2 = link2->link;
    }

    // check if loop exited bc of not same list
    if (link1 != nullptr || link2 != nullptr) {
      return false;
    }

    // recursively check each node in order
    return equalTravel(node1->left, node2->left) &&
           equalTravel(node1->right, node2->right);
  }

 public:
  /// Creates an empty `prqueue`.
  /// Runs in O(1).
  prqueue() {
    sz = 0;
    root = nullptr;
    curr = nullptr;
    temp = nullptr;
  }

  /// Returns the number of elements in the `prqueue`.
  /// Runs in O(1).
  size_t size() const {
    return sz;
  }

  /// Returns a pointer to the root node of the BST.
  /// Used for testing the internal structure of the BST. Do not edit or
  /// change.
  /// Runs in O(1).
  NODE* getRoot() {
    return root;
  }

  /// Adds `value` to the `prqueue` with the given `priority`.
  /// Uses the priority to determine the location in the underlying tree.
  /// Runs in O(H + M), where H is the height of the tree, and M is
  /// the number of duplicate priorities.
  void enqueue(T val, int prity) {
    // initalize
    NODE* iNode = new NODE();
    iNode->value = val;
    iNode->priority = prity;
    iNode->parent = nullptr;
    iNode->left = nullptr;
    iNode->right = nullptr;

    // check for new/empty tree
    if (root == nullptr) {
      root = iNode;
      sz++;
      return;
    }

    // traverse
    // here we just go to the correct location in the tree
    NODE* curr = root;  // start at root
    NODE* parent = nullptr;
    while (curr != nullptr) {
      parent = curr;                  // save parent, used later
      if (prity == curr->priority) {  // checks for duplicates
        NODE* temp = curr;
        while (temp->link != nullptr) {
          temp = temp->link;  // travel to end of linked list
        }
        // iNode->parent = parent;
        temp->link = iNode;  // append new node
        sz++;                // increment sz
        return;
      } else if (prity < curr->priority) {  // checks for correct placemnt
        curr = curr->left;                  // go left bc of BST
      } else {
        curr = curr->right;  // BST, bigger nodes go right
      }
    }

    // insert new node
    iNode->parent = parent;          // set saved parent
    if (prity < parent->priority) {  // check L or R of parent, then insert
      parent->left = iNode;
    } else {
      parent->right = iNode;
    }

    sz++;  // update size
  }

  /// Returns the value with the smallest priority in the `prqueue`, but does
  /// not modify the `prqueue`.
  /// If the `prqueue` is empty, returns the default value for `T`.
  /// Runs in O(H + M), where H is the height of the tree, and M is
  /// the number of duplicate priorities.
  T peek() const {
    // if tree is empty, return T
    if (root == nullptr) {
      return T{};
    }

    // traveres
    NODE* curr = root;
    while (curr->left != nullptr) {
      curr = curr->left;  // keep going left in BST for smallest
    }

    return curr->value;  // return when found
  }

  /// Returns the value with the smallest priority in the `prqueue` and
  /// removes it from the `prqueue`.
  /// If the `prqueue` is empty, returns the default value for `T`.
  /// Runs in O(H + M), where H is the height of the tree, and M is
  /// the number of duplicate priorities.
  T dequeue() {
    // check empts tree
    if (root == nullptr) {
      return T{};
    }

    // go to smallest node
    NODE* current = root;
    while (current->left != nullptr) {
      current = current->left;
    }

    // get min
    T minVal = current->value;

    if (current->link != nullptr) {
      NODE* next = current->link;          // get next  node
      current->value = next->value;        // make curent node val into
      current->priority = next->priority;  // copy proity
      // current->parent = next->parent;      // keep the parent pointer
      // current->left = next->left;          // keep the left pointer
      // current->right = next->right;        // keep the right pointer

      current->link = next->link;  // update link ptr to skip dequeued node

      delete next;
      sz--;
      return minVal;
    }

    // chekcs if it's just the root
    if (root->left == nullptr && root->right == nullptr) {
      delete root;     // Delete the root node
      root = nullptr;  // Set root to nullptr
      sz = 0;          // Reset size to 0
      return minVal;
    }

    // check if leftmost has right child
    if (current->right != nullptr) {
      // check if current is the root node
      if (current != root) {
        // final check to correctly make current's child it's parent's child
        if (current == current->parent->left) {
          current->parent->left = current->right;
        } else {
          current->parent->right = current->right;
        }
        current->right->parent = current->parent;  // update child's parent
      } else {                                     // if current is the root
        current->right->parent = nullptr;
        root = current->right;
      }
    } else {  // remove leaf node
      if (current != root) {
        // correctly set parent's child to nullptr
        if (current == current->parent->left) {
          current->parent->left = nullptr;
        } else {
          current->parent->right = nullptr;
        }
      } else {
        // if current is root with no left child but has a right child
        root = current->right;  // update right child to be root
        root->parent = nullptr;
      }
    }

    delete current;
    sz--;

    return minVal;
  }

  /// Converts the `prqueue` to a string representation, with the values
  /// in-order by priority.
  /// Example:
  /// ```c++
  /// prqueue<string> names;
  /// names.enqueue("Gwen", 3);
  /// names.enqueue("Jen", 2);
  /// names.enqueue("Ben", 1);
  /// names.enqueue("Sven", 2);
  /// ```
  /// Calling `names.as_string()` would return the following multi-line
  /// string:
  /// ```text
  /// 1 value: Ben
  /// 2 value: Jen
  /// 2 value: Sven
  /// 3 value: Gwen
  /// ```
  /// Runs in O(N), where N is the number of values.
  string as_string() const {
    // ogrinially used cout, but seends to be returned as a single string
    ostringstream ss;
    inorderTraversal(root, ss);
    return ss.str();
  }

  /// Empties the `prqueue`, freeing all memory it controls.
  /// Runs in O(N), where N is the number of values.
  void clear() {
    clearRecur(root);  // recursive helper funcion that deletes
    root = nullptr;    // reset root
    sz = 0;            // update size
  }

  /// Destructor, cleans up all memory associated with `prqueue`.
  /// Runs in O(N), where N is the number of values.
  ~prqueue() {
    clear();
  }

  /// Copy constructor.
  /// Copies the value-priority pairs from the provided `prqueue`.
  /// The internal tree structure must be copied exactly.
  /// Runs in O(N), where N is the number of values in `other`.
  prqueue(const prqueue& other) {
    root = nullptr;
    sz = 0;
    if (other.root != nullptr) {
      root = copyHelp(other.root, nullptr);
    }
  }

  /// Assignment operator; `operator=`.
  ///
  /// Clears `this` tree, and copies the value-priority pairs from the
  /// provided `prqueue`. The internal tree structure must be copied exactly.
  ///
  /// Runs in O(N + O), where N is the number of values in `this`, and O is
  /// the number of values in `other`.
  prqueue& operator=(const prqueue& other) {
    if (this == &other) {  // check if equal to self
      return *this;
    }

    clear();  // fuck the og tree

    // copy over
    if (other.root != nullptr) {             // check for empty tree
      root = copyHelp(other.root, nullptr);  // copy tree (reuse helper func)
      sz = other.sz;
    } else {
      root = nullptr;
      sz = 0;
    }

    return *this;
  }

  /// Checks if the contents of `this` and `other` are equivalent.
  ///
  /// Two `prqueues` are equivalent if they have the same priorities and
  /// values, as well as the same internal tree structure.
  ///
  /// These two `prqueue`s would be considered equivalent, because
  /// they have the same internal tree structure:
  ///
  /// ```c++
  /// prqueue<string> a;
  /// a.enqueue("2", 2);
  /// a.enqueue("1", 1);
  /// a.enqueue("3", 3);
  /// ```
  ///
  /// and
  ///
  /// ```c++
  /// prqueue<string> b;
  /// a.enqueue("2", 2);
  /// a.enqueue("3", 3);
  /// a.enqueue("1", 1);
  /// ```
  ///
  /// While this `prqueue`, despite containing the same priority-value pairs,
  /// would not be considered equivalent, because the internal tree structure
  /// is different.
  ///
  /// ```c++
  /// prqueue<string> c;
  /// a.enqueue("1", 1);
  /// a.enqueue("2", 2);
  /// a.enqueue("3", 3);
  /// ```
  ///
  /// Runs in O(N) time, where N is the maximum number of nodes in
  /// either `prqueue`.
  ///
  bool operator==(const prqueue& other) const {
    if (sz != other.sz) {  // automatically false without same size
      return false;
    }

    return equalTravel(root, other.root);
  }

  /// Resets internal state for an iterative inorder traversal.
  ///
  /// See `next` for usage details.
  ///
  /// O(H), where H is the maximum height of the tree.
  void begin() {
    // TODO_STUDENT
  }

  /// Uses the internal state to return the next in-order value and priority
  /// by reference, and advances the internal state. Returns true if the
  /// reference parameters were set, and false otherwise.
  ///
  /// Example usage:
  ///
  /// ```c++
  /// pq.begin();
  /// T value;
  /// int priority;
  /// while (pq.next(value, priority)) {
  ///   cout << priority << " value: " << value << endl;
  /// }
  /// ```
  ///
  /// Runs in worst-case O(H + M) or O(H), depending on implementation, where
  /// H is the height of the tree, and M is the number of duplicate
  /// priorities.
  bool next(T& value, int& priority) {
    // TODO_STUDENT
    return false;
  }
};
