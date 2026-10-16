#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

static std::string trim( const std::string& str ) {
    std::size_t start = str.find_first_not_of( " \t" );
    if ( start == std::string::npos )
        return "";
    std::size_t end = str.find_last_not_of( " \t" );
    return str.substr( start, end - start + 1 );
}

static int toInt( const std::string& digits ) {
    std::stringstream ss( digits );
    int value;
    ss >> value;
    return value;
}
BitcoinExchange::BitcoinExchange( void ) : _database() {}
BitcoinExchange::BitcoinExchange( const BitcoinExchange& src ) : _database( src._database ) {}

BitcoinExchange::~BitcoinExchange( void ) {}
BitcoinExchange& BitcoinExchange::operator=( const BitcoinExchange& rhs ) {
    if ( this != &rhs )
        _database = rhs._database;
    return *this;
}

bool BitcoinExchange::validateDate( const std::string& dateStr ) {
    if ( dateStr.size() != 10 || dateStr[ 4 ] != '-' || dateStr[ 7 ] != '-' )
        return false;
    for ( std::size_t i = 0; i < dateStr.size(); i++ ) {
        if ( i == 4 || i == 7 )
            continue;
        if ( dateStr[ i ] < '0' || dateStr[ i ] > '9' )
            return false;
    }

    int year = toInt( dateStr.substr( 0, 4 ) );
    int month = toInt( dateStr.substr( 5, 2 ) );
    int day = toInt( dateStr.substr( 8, 2 ) );
    
    if ( month < 1 || month > 12 || day < 1 || day > 31 )
        return false;

    static const int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool isLeapYear = ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );
    if ( month == 2 && isLeapYear )
        return day <= 29;
    return day <= daysInMonth[ month ];
}

bool BitcoinExchange::extractValue( const std::string& str, double& val ) {
    std::stringstream ss( str );
    ss >> val;
    if ( ss.fail() )
        return false;
    std::string rest;
    ss >> rest;
    return rest.empty();
}


bool BitcoinExchange::getExchangeRate(const std::string& dateStr, double& rateVal) const{
    if (_database.empty())
        return false;

    std::map< std::string, double >::const_iterator it = _database.upper_bound(dateStr);
    if (it == _database.begin()) 
        return false;

    --it;    
    rateVal = it->second;
    return true;
}
void BitcoinExchange::readDatabase( const std::string& file ) {
    std::ifstream ifs( file.c_str() );
    if ( !ifs.is_open() )
        throw std::runtime_error( "could not open database" );

    std::string line;
    while ( std::getline( ifs, line ) ) {
        std::string trimmed = trim( line );
        if ( trimmed.empty() || trimmed == "date,exchange_rate" )
            continue;

        std::size_t sep = trimmed.find( ',' );
        if ( sep == std::string::npos )
            continue;

        std::string dateStr = trim( trimmed.substr( 0, sep ) );
        std::string rateStr = trim( trimmed.substr( sep + 1 ) );

        double rateVal;
        if ( !validateDate( dateStr ) || !extractValue( rateStr, rateVal ) )
            continue;

        _database[ dateStr ] = rateVal;
    }
}

void BitcoinExchange::processInput( const std::string& file ) {
    std::ifstream ifs( file.c_str() );
    if ( !ifs.is_open() ) {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    while ( std::getline( ifs, line ) ) {
        std::string trimmed = trim( line );
        if ( trimmed.empty() || trimmed.rfind( "date", 0 ) == 0 )
            continue;

        std::size_t sep = trimmed.find( '|' );
        if ( sep == std::string::npos ) {
            std::cout << "Error: bad input => " << trimmed << std::endl;
            continue;
        }

        std::string dateStr = trim( trimmed.substr( 0, sep ) );
        std::string valueStr = trim( trimmed.substr( sep + 1 ) );

        if ( !validateDate( dateStr ) ) {
            std::cout << "Error: bad input => " << dateStr << std::endl;
            continue;
        }

        double val;
        if ( valueStr.empty() || !extractValue( valueStr, val ) ) {
            std::cout << "Error: bad input => " << trimmed << std::endl;
            continue;
        }

        if ( val < 0 ) {
            std::cout << "Error: not a positive number." << std::endl;
            continue;
        }

        if ( val > 1000 ) {
            std::cout << "Error: too large a number." << std::endl;
            continue;
        }

        double rateVal;
        if ( !getExchangeRate( dateStr, rateVal ) ) {
            std::cout << "Error: bad input => " << dateStr << std::endl;
            continue;
        }

        std::cout << dateStr << " => " << val << " = " << val * rateVal << std::endl;
    }
}