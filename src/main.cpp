//
//  main.cpp
//  BPlusTree.2a
//
//  Created by Amittai Aviram on 6/10/16.
//  Copyright © 2016 Amittai Aviram. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <iostream>
#include <sstream>
#include <cassert>
#include "BPlusTree.hpp"
#include "Definitions.hpp"
#include "Printer.hpp"

void test_a();
void test_b();
void test_c();


int main()
{
    test_b();
    return 0;
}

void test_a()
{
    const std::size_t order = 10;
    BPlusTree tree( order );


   tree.insert( 1, 1 );
   tree.remove( 1 );

   assert( tree.is_empty() );

}

void test_b()
{
    Printer printer;
    const std::size_t order = 3;
    const std::size_t item_no = 10;
    BPlusTree tree( order );

    for( std::size_t i = 0; i < item_no; i++ )
    {
        tree.insert( i, i );
        printer.print_tree( tree.m_root );
        std::cout << "\n";
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        assert( tree.search( i ) != nullptr );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        tree.remove( i );
        printer.print_tree( tree.m_root );
        std::cout << "\n";
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        std::cout << i << " " << std::flush;
        assert( tree.search( i ) == nullptr );
    }

}

void test_c()
{
    const std::size_t order = 10;
    const std::size_t item_no = 100;
    BPlusTree tree( order );

    for( std::size_t i = 0; i < item_no; i++ )
    {
        tree.insert( i, i );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        tree.remove( i );
    }

    assert( tree.is_empty() );

}



/*

std::string introMessage(std::size_t aOrder) {
    std::ostringstream oss;
    oss << "B+ Tree of Order " << aOrder << std::endl;
    oss << "C++ implementation by Amittai Aviram -- afa13@columbia.edu -- Version " << VERSION << std::endl;
    oss << "Following Silberschatz, Korth, Sidarshan, Database Concepts, 5th ed." << std::endl;
    oss << "To build a B+ tree of a different order, start again and enter the order" << std::endl;
    oss << "as an integer argument:  bpt <order>  " << std::endl;
    oss << "(" << MIN_ORDER << " <= order <= " << MAX_ORDER << ")." << std::endl;
    oss << "To start with input from a file of newline-delimited integers," << std::endl;
    oss << "start again and enter the order followed by the filename:" << std::endl;
    oss << "bpt <order> <inputfile> ." << std::endl << std::endl;
    return oss.str();
}

std::string usageMessage() {
    std::string message =
    "Enter any of the following commands after the prompt > :\n"
    "\ti <k>  -- Insert <k> (an integer, <k> >= 0) as both key and value).\n"
    "\tf <k>  -- Find the value under key <k>.\n"
    "\tp <k> -- Print the path from the root to key k and its associated value.\n"
    "\tr <k1> <k2> -- Print the keys and values found in the range [<k1>, <k2>]\n"
    "\td <k>  -- Delete key <k> and its associated value.\n"
    "\tx -- Destroy the whole tree.  Start again with an empty tree of the same order.\n"
    "\tt -- Print the B+ tree.\n"
    "\tl -- Print the keys of the leaves (bottom row of the tree).\n"
    "\tv -- Toggle output of pointer addresses (\"verbose\") in tree and leaves.\n"
    "\tq -- Quit. (Or use Ctl-D.)\n"
    "\t? -- Print this help message.\n\n";
    return message;
}

std::size_t getOrder(std::size_t argc, const char * argv[]) {
    if (argc > 1) {
        std::size_t order = 0;
        std::istringstream iss(argv[1]);
        if ((iss >> order) && iss.eof() &&
            order >= MIN_ORDER && order <= MAX_ORDER) {
            return order;
        } else {
            std::cerr << "Invalid order specification: "
            << argv[1] << std::endl;
            std::cerr << "Order must be an integer such that " << MIN_ORDER
            << " <= <order> <= " << MAX_ORDER << std::endl;
            std::cerr << "Proceeding with order " << DEFAULT_ORDER << std::endl;
        }
    }
    return DEFAULT_ORDER;
}



std::size_t main(std::size_t argc, const char * argv[]) {
    char instruction;
    std::size_t key = 0;
    bool quit = false;
    bool verbose = false;
    std::size_t order = getOrder(argc, argv);
    std::cout << introMessage(order);
    std::cout << usageMessage();
    BPlusTree tree(order);
    if (argc > 2) {
        tree.readInputFromFile(argv[2]);
        std::cout << "Input from file " << argv[2] << ":" << std::endl;
        tree.print();
    }
    while (!quit)
    {
        std::cout << "> ";
        std::cin >> instruction;
        switch (instruction) {
            case 'd':
                std::cin >> key;
                tree.remove(key);
                tree.print(verbose);
                break;
            case 'i':
                std::cin >> key;
                if (key < 0) {
                    std::cout << usageMessage();
                }
                tree.insert(key, key);
                tree.print(verbose);
                break;
            case 'f':
                std::cin >> key;
                tree.printValue(key);
                break;
            case 'l':
                tree.printLeaves(verbose);
                break;
            case 'p':
                std::cin >> key;
                tree.printPathTo(key, verbose);
                break;
            case 'q':
                quit = true;
                break;
            case 'r':
                std::size_t key2;
                std::cin >> key;
                std::cin >> key2;
                tree.printRange(key, key2);
                break;
            case 't':
                tree.print(verbose);
                break;
            case 'v':
                verbose = !verbose;
                tree.print(verbose);
                break;
            case 'x':
                tree.destroyTree();
                tree.print();
                break;
            case '?':
                std::cout << usageMessage();
                break;
            default:
                std::cin.ignore(256, '\n');
                std::cout << usageMessage();
                break;
        }
    }
    return 0;
}
*/

