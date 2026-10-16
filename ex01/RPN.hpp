#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>

class RPN {

private:

    std::stack< int > _operands;
    bool performOperation( char oper );
public:
    RPN( void );
    RPN( const RPN& src );
    ~RPN( void );
    RPN& operator=( const RPN& rhs );
    int calculate( const std::string& expr );
};

#endif