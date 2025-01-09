// Don't change this or you will be sad :(
#include <prqueue.h>

#include <map>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace std;
using namespace testing;

// test default constr
TEST(PRQueueCore, defaultconstructor) {
  prqueue<int> prq;
  EXPECT_EQ(prq.size(), 0) << "size != 0";
  EXPECT_EQ(prq.getRoot(), nullptr) << "root != nullptr";
}

// test enqueue
TEST(PRQueueCore, testEnqueueAndclear) {
  prqueue<int> prq;
  prq.enqueue(10, 5);
  EXPECT_EQ(prq.size(), 1) << "expected size is not 1";

  prq.enqueue(20, 2);
  EXPECT_EQ(prq.size(), 2) << "expected size is not 2";

  prq.enqueue(30, 10);
  EXPECT_EQ(prq.size(), 3) << "expected size is not 3";

  auto root = prq.getRoot();

  // manually checks if all nodes correct
  EXPECT_EQ(root->priority, 5) << "expected parent priority != 5";
  EXPECT_EQ(root->value, 10) << "expected parent value != 10";
  EXPECT_EQ(root->left->priority, 2) << "expected left child priority != 2";
  EXPECT_EQ(root->left->value, 20) << "expected left child value != 20";
  EXPECT_EQ(root->right->priority, 10) << "expected right child priority != 10";
  EXPECT_EQ(root->right->value, 30) << "expected right child value != 30";

  prq.clear();
  EXPECT_EQ(prq.size(), 0) << "size should be zero";
  EXPECT_EQ(prq.getRoot(), nullptr) << "root != nullptr";
}

//
//

TEST(PRQueueUsing, testAsStr) {
  prqueue<string> prq;
  prq.enqueue("Goku", 3);
  prq.enqueue("Vegeta", 2);
  prq.enqueue("Gohan", 1);
  prq.enqueue("Trunks", 4);

  string cmpword =
      "1 value: Gohan\n2 value: Vegeta\n3 value: Goku\n4 value: Trunks\n";
  // cout << cmpword << endl << prq.as_string();
  EXPECT_EQ(prq.as_string(), cmpword) << "as_string not correct";
}

TEST(PRQueueUsing, testPeek) {
  prqueue<int> prq;
  prq.enqueue(5, 0);
  prq.enqueue(10, 2);
  prq.enqueue(15, 3);
  prq.enqueue(20, 10);
  prq.enqueue(11, 5);

  EXPECT_EQ(prq.peek(), 5) << "peek does not return smallest prioity";
}

TEST(PRQueueUsing, testDeqceue) {
  prqueue<int> prq;
  prq.enqueue(5, 0);
  prq.enqueue(10, 22);
  prq.enqueue(15, 3);
  prq.enqueue(20, 10);
  prq.enqueue(11, 5);
  prq.enqueue(3, 1);

  EXPECT_EQ(prq.dequeue(), 5) << "deque does not return 5";
  EXPECT_EQ(prq.size(), 5) << "size != 5";
  EXPECT_EQ(prq.peek(), 3) << "dequeue does not remove smallest prioty node";
  prq.dequeue();
  EXPECT_EQ(prq.peek(), 15) << "dequeue does not remove smallest prioty node";

  prqueue<int> prq2;
  prq2.enqueue(10, 10);  // root
  prq2.enqueue(30, 5);
  prq2.enqueue(40, 2);  // smallest
  prq2.enqueue(50, 3);

  //   prq2.dequeue();
  //   prq2.enqueue(70, 3);
  string cmpword = "2 value: 40\n3 value: 50\n5 value: 30\n10 value: 10\n";
  EXPECT_EQ(prq2.as_string(), cmpword) << "part of the tree is lost";
  prq2.dequeue();
  //   EXPECT_NE(prq2.as_string(), cmpword) << "part of the tree is lost";
  //   prq2.enqueue(50, 1);
  cmpword = "3 value: 50\n5 value: 30\n10 value: 10\n";
  EXPECT_EQ(prq2.as_string(), cmpword) << "part of the tree is lost";

  prqueue<int> prq3;
  prq2.enqueue(10, 1);
  prq3.dequeue();
}

//
//

TEST(PRQueueDuplicates, testdupes) {
  prqueue<int> prq;
  prq.enqueue(10, 1);
  prq.enqueue(20, 2);
  prq.enqueue(30, 3);
  prq.enqueue(40, 4);
  prq.enqueue(101, 1);
  prq.enqueue(102, 1);

  string dupe =
      "1 value: 10\n"
      "1 value: 101\n"
      "1 value: 102\n"
      "2 value: 20\n"
      "3 value: 30\n"
      "4 value: 40\n";

  // cout << prq.as_string() << endl;

  EXPECT_EQ(prq.as_string(), dupe)
      << "enqueue doesnt add dupicated to linked list";

  prq.enqueue(301, 3);

  dupe =
      "1 value: 10\n"
      "1 value: 101\n"
      "1 value: 102\n"
      "2 value: 20\n"
      "3 value: 30\n"
      "3 value: 301\n"
      "4 value: 40\n";

  EXPECT_EQ(prq.as_string(), dupe)
      << "enqueue doesnt add dupicate to linked list";

  prq.enqueue(103, 1);

  dupe =
      "1 value: 10\n"
      "1 value: 101\n"
      "1 value: 102\n"
      "1 value: 103\n"
      "2 value: 20\n"
      "3 value: 30\n"
      "3 value: 301\n"
      "4 value: 40\n";

  EXPECT_EQ(prq.as_string(), dupe) << "enqueue doesnt add dupicate in order";
}

TEST(PRQueueDuplicates, insertdupes) {
  prqueue<int> prq;

  prq.enqueue(10, 1);
  prq.enqueue(20, 2);
  prq.enqueue(30, 2);
  prq.enqueue(40, 2);
  prq.enqueue(50, 2);

  vector<int> intarr = {20, 30, 40, 50};
  vector<int> real;
  auto root = prq.getRoot();
  root = root->right;
  auto curr = root;
  //   cout << curr->value << endl;
  //   real.push_back(curr->value);
  while (curr->link != nullptr) {
    // cout << curr->value << endl;
    real.push_back(curr->value);
    curr = curr->link;
  }
  // cout << curr->value << endl;
  real.push_back(curr->value);

  EXPECT_EQ(intarr, real) << "linked list not queued in order";
}

TEST(PRQueueDuplicates, testdequeue) {
  prqueue<int> prq;

  prq.enqueue(30, 3);
  prq.enqueue(40, 4);
  prq.enqueue(20, 2);
  prq.enqueue(1, 1);
  prq.enqueue(10, 1);
  prq.enqueue(100, 1);

  prq.dequeue();

  vector<int> intarr = {10, 100};
  vector<int> real;
  auto root = prq.getRoot();
  root = root->left;
  root = root->left;
  auto curr = root;
  while (curr->link != nullptr) {
    real.push_back(curr->value);
    curr = curr->link;
  }
  real.push_back(curr->value);

  EXPECT_EQ(intarr, real) << "dequeue not working";
}

//
//

TEST(PRQueueCopying, testcopy) {
  prqueue<int> prq;
  prq.enqueue(10, 2);
  prq.enqueue(20, 1);
  prq.enqueue(30, 2);
  prq.enqueue(40, 2);

  prqueue<int> copy = prq;

  EXPECT_EQ(copy.size(), prq.size()) << "copy contruct not the same sz";

  EXPECT_EQ(copy.dequeue(), 20);
  EXPECT_EQ(copy.dequeue(), 10);
  EXPECT_EQ(copy.dequeue(), 30);
  EXPECT_EQ(copy.dequeue(), 40);

  EXPECT_EQ(copy.dequeue(), 0) << "there was more to deque";

  EXPECT_EQ(copy.size(), 0);

  prqueue<int> prq2;
  prq2.enqueue(10, 1);
  prq2.enqueue(20, 2);
  prq2.enqueue(101, 1);

  prqueue<int> prq3;
  prq3.enqueue(50, 5);
  prq3.enqueue(60, 6);

  prq2 = prq3;

  EXPECT_EQ(prq2.size(), prq3.size()) << "assignment operator not the same sz";
  EXPECT_EQ(prq2.dequeue(), prq3.dequeue());
  EXPECT_EQ(prq2.dequeue(), prq3.dequeue());
  EXPECT_EQ(prq2.dequeue(), prq3.dequeue());
  EXPECT_EQ(prq2.dequeue(), prq3.dequeue());
}

TEST(PRQueueCopying, testcopydupes) {
  prqueue<int> prq;
  prq.enqueue(30, 3);
  prq.enqueue(301, 3);
  prq.enqueue(302, 3);
  prq.enqueue(10, 1);

  prqueue<int> copy;
  copy = prq;

  EXPECT_EQ(copy.size(), prq.size());

  EXPECT_EQ(copy.dequeue(), 10);
  EXPECT_EQ(copy.dequeue(), 30);
  EXPECT_EQ(copy.dequeue(), 301);
  EXPECT_EQ(copy.dequeue(), 302);
  EXPECT_EQ(copy.dequeue(), 0);

  EXPECT_EQ(prq.dequeue(), 10);
  EXPECT_EQ(prq.dequeue(), 30);
  EXPECT_EQ(prq.dequeue(), 301);
  EXPECT_EQ(prq.dequeue(), 302);
}

//
//

TEST(PRQueueEquality, testequal) {
  prqueue<int> prq1, prq2;

  prq1.enqueue(10, 1);
  prq1.enqueue(20, 2);
  prq1.enqueue(30, 3);
  prq1.enqueue(301, 3);

  prq2.enqueue(10, 1);
  prq2.enqueue(20, 2);
  prq2.enqueue(30, 3);
  prq2.enqueue(301, 3);

  EXPECT_TRUE(prq1 == prq2) << "exact same queue";

  prq2.enqueue(101, 1);

  EXPECT_FALSE(prq1 == prq2) << "queues should be different";

  prqueue<int> prq3, prq4;

  prq3.enqueue(10, 1);
  prq3.enqueue(20, 2);
  prq3.enqueue(30, 3);
  prq3.enqueue(301, 3);

  prq4.enqueue(10, 1);
  prq4.enqueue(20, 2);
  prq4.enqueue(30, 3);
  prq4.enqueue(302, 3);

  EXPECT_FALSE(prq3 == prq4) << "linked list equality test failed";
}