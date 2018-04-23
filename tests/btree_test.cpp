#include "gtest/gtest.h"
#include "BPlusTree.hpp"
#include <random>



TEST( btree, constuction )
{

    for( int order = MIN_ORDER; order <= MAX_ORDER; order++ )
    {
        ASSERT_NO_THROW( BPlusTree{ order } );
    }
}

/*
TEST( btree, search_empty )
{
    const std::uint32_t pool_size = 100;
    Tree tree( pool_size );
    Node* nb = tree.search( Key( 1 ) );
    ASSERT_TRUE( !nb );
}
*/

TEST( btree, one_item )
{
    const int order = 4;
    const int64_t k = 17;

    BPlusTree tree( order );

    ASSERT_NO_THROW( tree.insert( k, k ) );
    ASSERT_TRUE( tree.search( k ) != nullptr );

    tree.remove( k );

    ASSERT_TRUE( tree.search( k ) == nullptr  );
    ASSERT_TRUE( tree.isEmpty() );
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

    ASSERT_TRUE( tree.isEmpty() );

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
        ASSERT_TRUE( tree.search( i ) != nullptr );
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

    ASSERT_TRUE( tree.isEmpty() );

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

    ASSERT_TRUE( tree.isEmpty() );
}
*/

TEST( btree, insert_random )
{
    std::set< int64_t > sset;
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
            sset.insert( key );
            ASSERT_NO_THROW( tree.insert( key, i ) );
        }
        else
        {
            sset.erase( key );
            tree.remove( key );
        }

        // std::cout<< sset.size() << "\n" << std::flush;

        for( auto v : sset )
        {
            ASSERT_TRUE( tree.search( v ) != nullptr );
        }

    }
}


TEST( btree, erase_random )
{
    std::set< int64_t > sset;
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
        sset.insert( key );
        ASSERT_NO_THROW( tree.insert( key, i ) );
    }

    for( auto v : sset )
    {
        ASSERT_TRUE( tree.search( v ) != nullptr );
    }

    while( !sset.empty() )
    {
        const int64_t key( dist_int( rng ) );
        sset.erase( key );
        tree.remove( key );
    }

    ASSERT_TRUE( tree.isEmpty() );
}


