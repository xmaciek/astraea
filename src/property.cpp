#include "property.hpp"

Property::Property( const std::string& name ) :
    m_name( name )
{
}

int64_t Property::toInt() const {
    return std::stoi( m_value );
}

double Property::toReal() const {
    return std::stod( m_value );
}

std::string Property::toString() const {
    return m_value;
}

void Property::setValue( const std::string& v ) {
    m_value = v;
}

std::string Property::name() const {
    return m_name;
}

void Property::setName( const std::string& name ) {
    m_name = name;
}

bool Property::operator < ( const Property& v ) const {
    return m_name < v.m_name;
}

bool Property::operator == ( const Property& v) const {
    return m_name == v.m_name;
}


bool Property::writeToStream( std::ostream& stream, uint64_t intend, uint64_t intendOffset, std::string* errOut ) const {
    if ( m_name.empty() ) {
        if ( errOut ) {
            *errOut = "Cannot write property without name";
        }
        return false;
    }
    if ( m_value.empty() ) {
        if ( errOut ) {
            *errOut = "Cannot write property without value";
        }
        return false;
    }

    std::string intendation( intend * intendOffset, ' ' );
    stream << intendation << m_name << " = " << m_value << std::endl;
    return true;
}

