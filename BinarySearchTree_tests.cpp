#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;


TEST(test_comprehensive) {
    // Add your tests here
    BinarySearchTree<int> tree;

   //test everything for empty tree
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.size() == 0u);
    ASSERT_TRUE(tree.height() == 0u);
    ASSERT_TRUE(tree.min_element() == tree.end());
    ASSERT_TRUE(tree.max_element() == tree.end());
    ASSERT_TRUE(tree.min_greater_than(3) == tree.end());
    ASSERT_TRUE(tree.find(5) == tree.end()); 
    ASSERT_TRUE(tree.begin() == tree.end());


    //test everything for one element 
    
    tree.insert(0);
    
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    ASSERT_TRUE(*tree.min_element() == 0); 
    ASSERT_TRUE(*tree.max_element() == 0); 
    ASSERT_TRUE(tree.min_greater_than(0) == tree.end());
    ASSERT_TRUE(*tree.begin() == 0);
    ASSERT_TRUE(tree.find(8) == tree.end()); 
    ASSERT_TRUE(tree.find(0) != tree.end()); 

    tree.insert(9);
    tree.insert(3);
    tree.insert(4);
    tree.insert(-1);

    ASSERT_TRUE(tree.size() == 5);

    ASSERT_TRUE(tree.height() == 4);

    ASSERT_TRUE(tree.check_sorting_invariant());

    ASSERT_TRUE(*tree.max_element() == 9);
    ASSERT_TRUE(*tree.min_element() == -1);
    ASSERT_TRUE(*tree.min_greater_than(3) == 4);
    ASSERT_TRUE(*tree.min_greater_than(0) == 3);
    ASSERT_TRUE(tree.find(9) != tree.end()); 
    ASSERT_TRUE(tree.find(7) == tree.end());
    ASSERT_TRUE(tree.find(-2) == tree.end());

    cout << "cout << tree.to_string()" << endl;
    cout << tree.to_string() << endl << endl;

    cout << "cout << tree" << endl << "(uses iterators)" << endl;
    cout << tree << endl << endl;

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
     cout << "preorder" << endl;
     cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "0 -1 9 3 4 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "-1 0 3 4 9 ");
}

TEST(test_inorder_preorder_right) {

    BinarySearchTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    ASSERT_TRUE(tree.size() == 5);
    ASSERT_TRUE(tree.height() == 5);
    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << "cout << tree.to_string()" << endl;
    cout << tree.to_string() << endl << endl;

    cout << "cout << tree" << endl << "(uses iterators)" << endl;
    cout << tree << endl << endl;

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
     cout << "preorder" << endl;
     cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "1 2 3 4 5 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "1 2 3 4 5 ");
}

TEST(test_inorder_preorder_left) {

    BinarySearchTree<int> tree;

    tree.insert(0);
    tree.insert(-1);
    tree.insert(-3);
    tree.insert(-4);
    tree.insert(-5);

    ASSERT_TRUE(tree.size() == 5);
    ASSERT_TRUE(tree.height() == 5);
    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << "cout << tree.to_string()" << endl;
    cout << tree.to_string() << endl << endl;

    cout << "cout << tree" << endl << "(uses iterators)" << endl;
    cout << tree << endl << endl;

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
     cout << "preorder" << endl;
     cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "0 -1 -3 -4 -5 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "-5 -4 -3 -1 0 ");
}

TEST(test_copying_and_equality) {

    BinarySearchTree<int> tree;

    tree.insert(0);
    tree.insert(-1);
    tree.insert(1);
    tree.insert(-3);
    tree.insert(3);
    tree.insert(-4);
    tree.insert(4);

    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 4);
    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << "cout << tree.to_string()" << endl;
    cout << tree.to_string() << endl << endl;

    cout << "cout << tree" << endl << "(uses iterators)" << endl;
    cout << tree << endl << endl;

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
     cout << "preorder" << endl;
     cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "0 -1 -3 -4 1 3 4 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "-4 -3 -1 0 1 3 4 ");

    BinarySearchTree<int> tree2 = tree; 

    ASSERT_TRUE(tree2.size() == 7);
    ASSERT_TRUE(tree2.height() == 4);
     
    ASSERT_TRUE(tree.check_sorting_invariant());

    ASSERT_TRUE(*tree.max_element() == 4);
    ASSERT_TRUE(*tree.min_element() == -4);
    ASSERT_TRUE(*tree.min_greater_than(3) == 4);
    ASSERT_TRUE(*tree.min_greater_than(0) == 1);
    ASSERT_TRUE(tree.find(0) != tree.end()); 
    ASSERT_TRUE(tree.find(7) == tree.end());
    ASSERT_TRUE(tree.find(-2) == tree.end());

    BinarySearchTree<int> tree3 = BinarySearchTree(tree); 

    ASSERT_TRUE(tree3.size() == 7);
    ASSERT_TRUE(tree3.height() == 4);

    ASSERT_TRUE(*tree.max_element() == 4);
    ASSERT_TRUE(*tree.min_element() == -4);
    ASSERT_TRUE(*tree.min_greater_than(3) == 4);
    ASSERT_TRUE(*tree.min_greater_than(0) == 1);
    ASSERT_TRUE(tree.find(0) != tree.end()); 
    ASSERT_TRUE(tree.find(7) == tree.end());
    ASSERT_TRUE(tree.find(-2) == tree.end());
    
    
}


TEST_MAIN()
