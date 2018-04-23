#include "gtest/gtest.h"
#include "BPlusTree.hpp"
#include <random>
#include <map>



TEST( btree, constuction )
{

    for( int order = MIN_ORDER; order <= MAX_ORDER; order++ )
    {
        ASSERT_NO_THROW( BPlusTree{ order } );
    }
}


TEST( btree, search_empty )
{
    const int order = 4;
    BPlusTree tree( order );
    Record* rec = tree.search( 1 );
    ASSERT_TRUE( !rec );
}


TEST( btree, one_item )
{
    const int order = 4;
    const int64_t k = 17;

    BPlusTree tree( order );

    ASSERT_NO_THROW( tree.insert( k, k ) );
    Record* rec = tree.search( k );
    ASSERT_TRUE( rec != nullptr );
    ASSERT_TRUE( rec->value() == k );

    tree.remove( k );

    ASSERT_TRUE( tree.search( k ) == nullptr  );
    ASSERT_TRUE( tree.is_empty() );
}

/*
TEST( btree, more_items )
{
    const int order = 10;
    const int item_no = 2000;
    BPlusTree tree( order );

    for( int i = 0; i < item_no; i++ )
    {
        ASSERT_NO_THROW( tree.insert( i, i ) );
    }

    for( int i = 0; i < item_no; i++ )
    {
        ASSERT_TRUE( tree.search( i ) != nullptr );
    }

    for( int i = 0; i < item_no; i++ )
    {
        tree.remove( i );
    }

    for( int i = 0; i < item_no; i++ )
    {
        //std::cout << i << " " << std::flush;
        ASSERT_TRUE( tree.search( i ) == nullptr );
    }

    ASSERT_TRUE( tree.is_empty() );

}
*/


TEST( btree, reverse_erase )
{
    const int order = 5;
    const int item_no = 20000;
    BPlusTree tree( order );

    for( int i = 0; i < item_no; i++ )
    {
        ASSERT_NO_THROW( tree.insert( i, i ) );
    }

    for( int i = 0; i < item_no; i++ )
    {
        Record* rec = tree.search( i );
        ASSERT_TRUE( rec );
        ASSERT_TRUE( rec->value() == i );
    }

    for( int i = item_no - 1; i >= 0 ; i-- )
    {
        tree.remove( i );
    }

    for( int i = 0; i < item_no; i++ )
    {
        // std::cout << i << " " << std::flush;
        ASSERT_TRUE( tree.search( i ) == nullptr );
    }

    ASSERT_TRUE( tree.is_empty() );

}

/*
TEST( btree, insert_search_delete )
{
    const int item_no = 9000;
    const unsigned seed = 12345;
    std::set< int64_t > sset;
    std::srand( seed );
    const int order = 5;

    BPlusTree tree( order );

    for( int i = 0; i < item_no; i++ )
    {
        const int64_t key = std::rand();
        // std::cout << key.get_value() << " " << std::flush;
        sset.insert( key );
        ASSERT_NO_THROW( tree.insert( key, i ) );
    }

    for( auto v : sset )
    {
        ASSERT_TRUE( tree.search( v ) != nullptr );
    }

    for( auto v : sset )
    {
        tree.remove( v );
    }

    for( auto v : sset )
    {
        ASSERT_TRUE( tree.search( v ) == nullptr );
    }

    ASSERT_TRUE( tree.is_empty() );
}
*/

TEST( btree, insert_random )
{
    std::map< KeyType, ValueType > smap;
    const int order = 5;
    BPlusTree tree( order );

    const double threshold = 0.6;
    const int iter_no = 2000;

    //std::random_device rd; // only used once to initialise (seed) engine
    //std::mt19937 rng( rd() ); // random-number engine used (Mersenne-Twister in this case)
    std::mt19937 rng;
    std::uniform_real_distribution<> dist_real( 0, 1 );

    std::uniform_int_distribution< int > dist_int( 0, iter_no/2 );

    for( int i = 0; i < iter_no; i++ )
    {
        const int64_t key( dist_int( rng ) );
        const double x = dist_real( rng );

        if( x > threshold )
        {
            smap.insert( std::make_pair( key, i ) );
            ASSERT_NO_THROW( tree.insert( key, i ) );
        }
        else
        {
            smap.erase( key );
            tree.remove( key );
        }

        // std::cout<< sset.size() << "\n" << std::flush;

        for( auto v : smap )
        {
            Record* rec = tree.search( v.first );
            ASSERT_TRUE( rec );
            ASSERT_TRUE( rec->value() == v.second );
        }

    }
}


TEST( btree, erase_random )
{
    std::map< KeyType, ValueType > smap;
    const int order = 5;
    BPlusTree tree( order );

    const int iter_no = 5000;

    //std::random_device rd; // only used once to initialise (seed) engine
    // std::mt19937 rng( rd() ); // random-number engine used (Mersenne-Twister in this case)
    std::mt19937 rng;

    std::uniform_int_distribution< int > dist_int( 0, iter_no/2 );

    for( int i = 0; i < iter_no; i++ )
    {
        const int64_t key( dist_int( rng ) );
        smap.insert( std::make_pair( key, i ) );
        ASSERT_NO_THROW( tree.insert( key, i ) );
    }

    for( auto v : smap )
    {
        Record* rec = tree.search( v.first );
        ASSERT_TRUE( rec );
        ASSERT_TRUE( rec->value() == v.second );
    }

    while( !smap.empty() )
    {
        const KeyType key( dist_int( rng ) );
        smap.erase( key );
        tree.remove( key );
    }

    ASSERT_TRUE( tree.is_empty() );
}


