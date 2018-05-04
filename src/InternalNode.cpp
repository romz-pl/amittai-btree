#include <algorithm>
#include <cassert>
#include <iterator>
#include "InternalNode.hpp"



//
//
//
InternalNode::InternalNode( BPlusTree *tree, InternalNode *parent )
    : Node( tree, parent )
{

}

//
//
//
InternalNode::~InternalNode()
{
    for( auto mapping : m_elt )
    {
        delete mapping.m_node;
    }
}

//
//
//
bool InternalNode::is_leaf() const
{
    return false;
}

//
//
//
std::size_t InternalNode::size() const
{
    return m_elt.size();
}

//
//
//
KeyType InternalNode::key_at( std::size_t index ) const
{
    assert( index < m_elt.size() );
    return m_elt[ index ].m_key;
}

//
//
//
void InternalNode::set_key_at( std::size_t index, const KeyType& key )
{
    assert( index < m_elt.size() );
    m_elt[ index ].m_key = key;
}

//
//
//
Node* InternalNode::first_child() const
{
    assert( !m_elt.empty() );
    return m_elt.front().m_node;
}

//
//
//
void InternalNode::populate_new_root( Node *old_node, const KeyType& new_key, Node *new_node )
{
    // assert( is_sorted() );

    assert( m_elt.empty() );
    m_elt.push_back( InternalElt( DUMMY_KEY, old_node ) );
    m_elt.push_back( InternalElt( new_key, new_node ) );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::insert_node_after( Node *old_node, const KeyType& new_key, Node *new_node )
{
    // assert( is_sorted() );

    const auto pred = [ old_node ]( const InternalElt& e ){ return e.m_node == old_node; };
    const auto iter = std::find_if( m_elt.begin(), m_elt.end(), pred );
    assert( iter != m_elt.end() );

    m_elt.insert( iter + 1, InternalElt( new_key, new_node ) );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::remove( std::size_t index )
{
    // assert( is_sorted() );

    assert( index < m_elt.size() );
    m_elt.erase( m_elt.begin() + index );

    // assert( is_sorted() );
}

//
//
//
Node* InternalNode::remove_and_return_only_child()
{
    // assert( is_sorted() );

    assert( m_elt.size() == 1 );
    Node* first_child = m_elt.front().m_node;
    m_elt.pop_back();

    // assert( is_sorted() );

    return first_child;
}

//
//
//
KeyType InternalNode::replace_and_return_first_key()
{
    // assert( is_sorted() );

    const KeyType new_key = m_elt[ 0 ].m_key;
    m_elt[ 0 ].m_key = KeyType( DUMMY_KEY );

    // assert( is_sorted() );
    return new_key;
}

//
//
//
void InternalNode::move_half_to( InternalNode *recipient )
{
    // assert( is_sorted() );

    recipient->copy_half_from( m_elt );
    const auto start = m_elt.begin() + m_tree->internal_min_size();
    m_elt.erase( start, m_elt.end() );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_half_from(const std::vector< InternalElt >& ve )
{
    // assert( is_sorted() );

    for( std::size_t i = m_tree->internal_min_size(); i < ve.size(); ++i )
    {
        ve[ i ].m_node->set_parent( this );
        m_elt.push_back( ve[ i ] );
    }

    // assert( is_sorted() );
}

//
//
//
void InternalNode::move_all_to( InternalNode *recipient, std::size_t index_in_parent )
{
    // assert( is_sorted() );

    m_elt[ 0 ].m_key = get_parent()->key_at( index_in_parent );
    recipient->copy_all_from( m_elt );
    m_elt.clear();

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_all_from(const std::vector< InternalElt >& ve )
{
    // assert( is_sorted() );

    for( auto m : ve )
    {
        m.m_node->set_parent( this );
        m_elt.push_back( m );
    }

    // assert( is_sorted() );
}

//
//
//
void InternalNode::move_first_to_end_of( InternalNode *recipient )
{
    // assert( is_sorted() );

    recipient->copy_last_from( m_elt.front() );
    m_elt.erase( m_elt.begin() );
    get_parent()->set_key_at( 1, m_elt.front().m_key );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_last_from( const InternalElt& pair )
{
    // assert( is_sorted() );

    m_elt.push_back( pair );
    m_elt.back().m_node->set_parent( this );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::move_last_to_front_of( InternalNode *recipient, std::size_t parent_index )
{
    // assert( is_sorted() );

    recipient->copy_first_from( m_elt.back(), parent_index );
    m_elt.pop_back();

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_first_from( const InternalElt& pair, std::size_t parent_index )
{
    // assert( is_sorted() );

    m_elt.front().m_key = get_parent()->key_at( parent_index );
    m_elt.insert( m_elt.begin(), pair );
    m_elt.front().m_key = KeyType( DUMMY_KEY );
    m_elt.front().m_node->set_parent( this );
    get_parent()->set_key_at( parent_index, m_elt.front().m_key );

    // assert( is_sorted() );
}

//
//
//
Node* InternalNode::lookup( const KeyType& key ) const
{
/*
    if( !is_sorted() )
    {
        for( auto m : m_mappings )
        {
            std::cout << m.first.to_int64() << " " << std::flush;
        }
        std::cout << "\n";
        exit(1);
    }
*/


    assert( m_elt.front().m_key <= key );
    // assert( is_sorted() );


    //
    // Is this equivalent to what is below ??
    //

    const auto pred = [ key ]( const InternalElt& v ){ return v.m_key > key; };
    auto locator = std::find_if( m_elt.begin(), m_elt.end(), pred );
    // locator->m_key is now the least key "k" such that key < k.
    // One before is the greatest key k such that key >= k.

    assert( locator != m_elt.begin() );
    --locator;
    return locator->m_node;



    /* This algorithm should be correct?!

    const auto pred = [ key ]( const InternalElt& v ){ return v.m_key <= key ; };
    const auto locator = std::find_if( m_elt.rbegin(), m_elt.rend(), pred );

    return locator->m_node;
    */
}

//
//
//
std::size_t InternalNode::node_index( Node *node ) const
{
    // assert( is_sorted() );

    const auto pred = [ node ]( const InternalElt& v ){ return( v.m_node == node ); };
    const auto ff = std::find_if( m_elt.begin(), m_elt.end(), pred );
    assert( ff != m_elt.end() );

    const auto index = std::distance( m_elt.begin(), ff );
    assert( index >= 0 );

    return static_cast< std::size_t >( index );
}

//
//
//
Node* InternalNode::neighbor( std::size_t index ) const
{
    // assert( is_sorted() );

    return m_elt[ index ].m_node;
}

//
//
//
bool InternalNode::is_sorted() const
{
    const auto pred = []( const InternalElt& ma, const InternalElt& mb ){ return ma.m_key < mb.m_key ; };

    return std::is_sorted( m_elt.begin(), m_elt.end(), pred );
}

//
//
//
InternalNode* InternalNode::internal()
{
    return this;
}

//
//
//
const InternalNode* InternalNode::internal() const
{
    return this;
}


