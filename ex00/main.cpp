#include "BitcoinExchange.hpp"
#include <iostream>
#include <exception>

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    BitcoinExchange exchange;
    try {
        exchange.readDatabase( "data.csv" );
    } catch ( const std::exception& e ) {
        std::cerr << "Error: could not open database file (data.csv)." << std::endl;
        return 1;
    }
    exchange.processInput( argv[ 1 ] );
    return 0;
}