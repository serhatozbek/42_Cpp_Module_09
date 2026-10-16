#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

class BitcoinExchange {

private:
    std::map< std::string, double > _database;
    static bool validateDate( const std::string& dateStr );
    static bool extractValue( const std::string& str, double& val );
    bool getExchangeRate( const std::string& dateStr, double& rateVal ) const;
public:
    BitcoinExchange( void );
    BitcoinExchange( const BitcoinExchange& src );
    ~BitcoinExchange( void );
    BitcoinExchange& operator=( const BitcoinExchange& rhs );
    void readDatabase( const std::string& file );
    void processInput( const std::string& file );
};

#endif