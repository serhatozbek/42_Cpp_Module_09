#include "RPN.hpp"
#include <sstream>
#include <stdexcept>

RPN::RPN( void ) : _operands() {}
RPN::RPN( const RPN& src ) : _operands(src._operands) {}
RPN::~RPN( void ) {}
RPN& RPN::operator=( const RPN& rhs ) {
    if ( this != &rhs )
        _operands = rhs._operands;
    return *this;
}

bool RPN::performOperation(char oper) {
    if ( _operands.size() < 2 )
        return false;
    
    int a = _operands.top();
    _operands.pop();
    int b = _operands.top();
    _operands.pop();

    if ( oper == '+' )
        _operands.push( b + a );
    else if ( oper == '-' )
        _operands.push( b - a );
    else if ( oper == '/' ) {
        if ( a == 0 )
            return false;
        _operands.push( b / a );
    } else if ( oper == '*' )
        _operands.push( b * a );
    else
        return false;

    return true;
}

int RPN::calculate( const std::string& expr ) {
    while ( !_operands.empty() )
        _operands.pop();

    std::stringstream ss(expr);
    std::string token;

    while ( ss >> token ) {
        if ( token.size() == 1 && ( token[ 0 ] == '+' || token[ 0 ] == '-' || token[ 0 ] == '/' || token[ 0 ] == '*' ) ) {
            if ( !performOperation( token[ 0 ] ) )
                throw std::runtime_error( "Error" );
        } else if ( token.size() == 1 && token[ 0 ] >= '0' && token[ 0 ] <= '9' ) {
            _operands.push( token[ 0 ] - '0' );
        } else {
            throw std::runtime_error( "Error" );
        }
    }

    if ( _operands.size() != 1 )
        throw std::runtime_error( "Error" );

    return _operands.top();
}