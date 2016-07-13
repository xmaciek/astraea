#pragma once

#include <cstdint>
#include <fstream>
#include <string>

class Property {
public:
    Property( const std::string& name );

    int64_t toInt() const;
    double toReal() const;
    std::string toString() const;
    void setValue( const std::string& v );
    std::string name() const;
    void setName( const std::string& );

    bool operator < ( const Property& ) const;
    bool operator == ( const Property& ) const;
    bool writeToStream( std::ostream& stream, uint64_t intend = 4, uint64_t intendOffset = 0, std::string* errOut = 0 ) const;

private:
    std::string m_name;
    std::string m_value;
};
