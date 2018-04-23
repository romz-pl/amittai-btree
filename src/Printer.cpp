#include <iostream>
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
        std::cout << " " << currentNode->to_string(verbose());
        std::cout << " |";
        if (!currentNode->is_leaf()) {
            auto internalNode = static_cast<InternalNode*>(currentNode);
            internalNode->queue_up_children(next_rank);
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
        std::cout << leafNode->to_string(m_verbose);
        leafNode = leafNode->next();
    }
    std::cout << " |" << std::endl;
}
