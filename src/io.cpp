#include <iostream>
#include <fstream>
#include "io.h"
#include "InternalNode.hpp"
#include "LeafNode.hpp"
#include "Node.hpp"

Io::Io( BPlusTree &tree )
    : m_tree( tree )
{

}

LeafNode* Io::find_leaf_node( KeyType key, bool printing, bool verbose )
{
    if (m_tree.is_empty()) {
        if (printing) {
            std::cout << "Not found: empty tree." << std::endl;
        }
        return nullptr;
    }
    auto node = m_tree.m_root;
    if (printing) {
        std::cout << "Root: ";
        if (m_tree.m_root->is_leaf())
        {
            auto leaf = static_cast< LeafNode* >( m_tree.m_root );
            std::cout << "\t" << Printer::to_string( leaf, verbose );
        }
        else
        {
            auto internal = static_cast< InternalNode* >( m_tree.m_root );
            std::cout << "\t" << Printer::to_string( internal, verbose );
        }
        std::cout << std::endl;
    }
    while (!node->is_leaf()) {
        auto internalNode = static_cast<InternalNode*>(node);
        if (printing && node != m_tree.m_root)
        {
            std::cout << "\tNode: " << Printer::to_string( internalNode, verbose ) << std::endl;
        }
        node = internalNode->lookup(key);
    }
    return static_cast<LeafNode*>(node);
}

void Io::read_input_from_file( std::string file_name )
{
    std::size_t key;
    std::ifstream input(file_name);
    while (input) {
        input >> key;
        m_tree.insert(key, key);
    }
}

void Io::print( bool verbose )
{
    m_printer.set_verbose(verbose);
    m_printer.print_tree(m_tree.m_root);
}

void Io::print_leaves( bool verbose )
{
    m_printer.set_verbose(verbose);
    m_printer.print_leaves(m_tree.m_root);
}



void Io::print_value( KeyType key, bool verbose )
{
    print_value(key, false, verbose);
}

void Io::print_value( KeyType key, bool print_path, bool verbose )
{
    LeafNode* leaf = find_leaf_node(key, print_path, verbose);
    if (!leaf) {
        std::cout << "Leaf not found with key " << key << "." << std::endl;
        return;
    }
    if (print_path) {
        std::cout << "\t";
    }
    std::cout << "Leaf: " << Printer::to_string( leaf, verbose) << std::endl;
    Record* record = leaf->lookup(key);
    if (!record) {
        std::cout << "Record not found with key " << key << "." << std::endl;
        return;
    }
    if (print_path) {
        std::cout << "\t";
    }
    std::cout << "Record found at location " << std::hex << record << std::dec << ":" << std::endl;
    std::cout << "\tKey: " << key << "   Value: " << record->value() << std::endl;
}

void Io::print_path_to( KeyType key, bool verbose )
{
    print_value(key, true, verbose);
}

void Io::print_range( KeyType start, KeyType end )
{
    auto rangeVector = range(start, end);
    for (auto entry : rangeVector) {
        std::cout << "Key: " << std::get<0>(entry);
        std::cout << "    Value: " << std::get<1>(entry);
        std::cout << "    Leaf: " << std::hex << std::get<2>(entry) << std::dec << std::endl;
    }
}

std::vector< BPlusTree::EntryType > Io::range( KeyType start, KeyType end )
{
    auto startLeaf = find_leaf_node(start);
    auto endLeaf = find_leaf_node(end);
    std::vector<std::tuple<KeyType, ValueType, LeafNode*>> entries;
    if (!startLeaf || !endLeaf) {
        return entries;
    }
    leaf_node_copy_range_starting_from( startLeaf, start, entries);
    startLeaf = startLeaf->next();
    while (startLeaf != endLeaf)
    {
        leaf_node_copy_range( startLeaf, entries);
        startLeaf = startLeaf->next();
    }
    leaf_node_copy_range_until( startLeaf, end, entries);
    return entries;
}

void Io::leaf_node_copy_range_starting_from( LeafNode* leaf, KeyType key, std::vector< LeafNode::EntryType >& vector )
{
    bool found = false;
    for (auto mapping : leaf->m_mappings) {
        if (mapping.first == key) {
            found = true;
        }
        if (found) {
            vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), leaf));
        }
    }
}

void Io::leaf_node_copy_range_until( LeafNode* leaf, KeyType key, std::vector< LeafNode::EntryType >& vector )
{
    bool found = false;
    for (auto mapping : leaf->m_mappings) {
        if (!found) {
            vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), leaf));
        }
        if (mapping.first == key) {
            found = true;
        }
    }
}

void Io::leaf_node_copy_range( LeafNode* leaf, std::vector< LeafNode::EntryType >& vector )
{
    for (auto mapping : leaf->m_mappings) {
        vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), leaf));
    }
}

