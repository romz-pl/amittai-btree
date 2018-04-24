#include <iostream>
#include <sstream>
#include "InternalNode.hpp"
#include "LeafNode.hpp"
#include "Node.hpp"

#include "Printer.hpp"

Printer::Printer()
    : m_verbose( false )
{

}

bool Printer::verbose() const
{
    return m_verbose;
}

void Printer::set_verbose( bool verbose )
{
    m_verbose = verbose;
}

void Printer::print_tree( Node *root ) const
{
    if (!root) {
        print_empty_tree();
    } else {
        print_non_empty_tree(root);
    }
}

void Printer::print_empty_tree() const
{
    std::cout << "Empty tree." << std::endl;
}

void Printer::print_non_empty_tree( Node *root ) const
{
    std::queue<Node*> queue0;
    std::queue<Node*> queue1;
    auto currentRank = &queue0;
    auto nextRank = &queue1;
    currentRank->push(root);
    while (!currentRank->empty()) {
        print_current_rank(currentRank, nextRank);
        auto tmp = currentRank;
        currentRank = nextRank;
        nextRank = tmp;
    }
}

void Printer::print_current_rank( std::queue< Node* >* current_rank, std::queue< Node* >* next_rank) const
{
    std::cout << "|";
    while(!current_rank->empty()) {
        Node* currentNode = current_rank->front();
        std::cout << " " << to_string( currentNode, verbose());
        std::cout << " |";
        if (!currentNode->is_leaf()) {
            auto internalNode = static_cast<InternalNode*>(currentNode);
            internal_node_queue_up_children( internalNode, next_rank);
        }
        current_rank->pop();
    }
    std::cout << std::endl;
}

void Printer::print_leaves( Node *root )
{
    if (!root) {
        print_empty_tree();
        return;
    }
    auto node = root;
    while (!node->is_leaf()) {
        node = static_cast<InternalNode*>(node)->first_child();
    }
    auto leafNode = static_cast<LeafNode*>(node);
    while (leafNode) {
        std::cout << "| ";
        std::cout << to_string( leafNode, m_verbose);
        leafNode = leafNode->next();
    }
    std::cout << " |" << std::endl;
}

void Printer::internal_node_queue_up_children( InternalNode* internal, std::queue< Node* >* queue ) const
{
    for (auto mapping : internal->m_elt)
    {
        queue->push(mapping.m_node);
    }
}

std::string Printer::to_string( const Node* node, bool verbose )
{
    if( node->is_leaf() )
    {
        return to_string( static_cast< const LeafNode* >( node ), verbose );
    }

    return to_string( static_cast< const InternalNode* >( node ), verbose );
}

std::string Printer::to_string( const LeafNode* leaf, bool verbose )
{
    std::ostringstream keyToTextConverter;
    if (verbose) {
        keyToTextConverter << "[" << std::hex << leaf << std::dec << "]<" << leaf->m_elt.size() << "> ";
    }
    bool first = true;
    for (auto mapping : leaf->m_elt) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << mapping.m_key.to_int64();
    }
    if (verbose) {
        keyToTextConverter << "[" << std::hex << leaf->m_next << ">";
    }
    return keyToTextConverter.str();
}

std::string Printer::to_string( const InternalNode* internal, bool verbose )
{
    if (internal->m_elt.empty()) {
        return "";
    }
    std::ostringstream keyToTextConverter;
    if (verbose) {
        keyToTextConverter << "[" << std::hex << internal << std::dec << "]<" << internal->m_elt.size() << "> ";
    }
    auto entry = verbose ? internal->m_elt.begin() : internal->m_elt.begin() + 1;
    auto end = internal->m_elt.end();
    bool first = true;
    while (entry != end) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << std::dec << entry->m_key.to_int64();
        if (verbose) {
            keyToTextConverter << "(" << std::hex << entry->m_node << std::dec << ")";
        }
        ++entry;
    }
    return keyToTextConverter.str();
}

std::string Printer::to_string( const Record& rec )
{
    std::ostringstream oss;
    oss << rec.value();
    return oss.str();
}


