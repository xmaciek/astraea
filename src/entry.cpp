#include "entry.hpp"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

static uint64_t idCounter = 0;

Entry::Entry() :
    m_id( ++idCounter )
{
}

bool Entry::isArray() const {
    return !m_array.empty() && m_entries.empty() && m_properties.empty();
}

bool Entry::isValid() const {
    return  !m_name.empty() && ( ( m_array.empty() && m_entries.empty() && m_properties.empty() ) ||
            ( !m_array.empty() != ( !m_entries.empty() || !m_properties.empty() ) ) );
}

bool Entry::writeToFile( const std::string& fileName, uint64_t intend, std::string* errOut ) const {
    std::ofstream file( fileName );
    if ( !file.is_open() ) {
        const int errorId = errno;
        if ( errOut ) {
            std::stringstream ss;
            ss << "Cannot open file ";
            ss << fileName;
            ss << ": ";
            ss << strerror( errorId );
            *errOut = ss.str();
        }
        return false;
    }
    const bool writeResult = writeToStream( std::cerr, intend, 0, errOut );
    file.close();
    return writeResult;
}


bool Entry::writeToStream( std::ostream &stream, uint64_t intend, uint64_t intendOffset, std::string* errOut ) const {
    if ( m_name.empty() ) {
        if ( errOut ) {
            *errOut = std::string( "Cannot write entry without a name" );
        }
        return false;
    }
    const std::string intendation( intend * intendOffset, ' ' );
    stream << intendation << m_name << " = {" << std::endl;
    if ( isArray() ) {
        const std::string intendation2( intend * ( intendOffset + 1 ), ' ' );
        stream << intendation2;
        Array::const_iterator it = m_array.cbegin();
        while ( it != m_array.cend() ) {
            stream << *it << ' ';
            it++;
        }
        stream << std::endl;
    } else {
        Properties::const_iterator it = m_properties.cbegin();
        while ( it != m_properties.cend() ) {
            if ( !it->writeToStream( stream, intend, intendOffset + 1, errOut ) ) {
                return false;
            }
            it++;
        }

        Entries::const_iterator it2 = m_entries.cbegin();
        while ( it2 != m_entries.cend() ) {
            if ( !it2->writeToStream( stream, intend, intendOffset + 1, errOut ) ) {
                return false;
            }
            it2++;
        }
    }

    stream << intendation << '}' << std::endl;
    return true;
}

class Token {
public:
    Token( const std::vector<std::string> &v ) :
    m_pos( v.cbegin() ),
    m_end( v.cend() )
    { }

    Token& operator += ( int64_t i ) {
        m_pos += i;
        return *this;
    }
    Token& operator -= ( int64_t i ) {
        m_pos -= i;
        return *this;
    }

    Token& operator ++ () {
        m_pos++;
        return *this;
    }

    Token& operator -- () {
        m_pos--;
        return *this;
    }

    const std::string& operator * () const {
        return *m_pos;
    }

    operator bool () const {
        return m_pos != m_end;
    }

    bool operator ! () const {
        return m_pos == m_end;
    }

private:
    std::vector<std::string>::const_iterator m_pos;
    const std::vector<std::string>::const_iterator m_end;
};

static bool isStreamEnd( Token &token ) {
    return token && *token == "}";
}

static bool isStreamArray( Token &token ) {
    ++token;
    if ( !token ) {
        --token;
        return false;
    }
    const bool b = *token != "=";
    --token;
    return b;
}

static bool isStreamEntry( Token &token ) {
    ++token;
    if ( !token ) {
        --token;
        return false;
    }
    if ( *token != "=" ) {
        --token;
        return false;
    }
    ++token;
    if ( !token ) {
        token -= 2;
        return false;
    }
    const bool b = *token == "{";
    token -= 2;
    return b;
}

static bool isStreamProperty( Token &token ) {
    ++token;
    if ( !token ) {
        --token;
        return false;
    }
    if ( *token != "=" ) {
        --token;
        return false;
    }
    ++token;
    if ( !token ) {
        token -= 2;
        return false;
    }
    const bool b = *token != "{";
    token -= 2;
    return b;
}


bool Entry::fromStream( Token &token, std::string* errOut ) {
    m_name = *token;
    ++token;
    if ( !token ) {
        if ( errOut ) {
            *errOut = std::string( "Unexpected end of stream @" ) + std::to_string( __LINE__ );
        }
        return false;
    }
    if ( *token != "=" ) {
        if ( errOut ) {
            *errOut = "Expected '=' after " + m_name;
        }
        return false;
    }
    ++token;

    if ( !token ) {
        if ( errOut ) {
            *errOut = std::string( "Unexpected end of stream @" ) + std::to_string( __LINE__ );
        }
        return false;
    }
    if ( *token != "{" ) {
        if ( errOut ) {
            *errOut = "Expected '{' after '" + m_name + " = '";
        }
        return false;
    }
    ++token;

    while ( token ) {
        if ( isStreamEnd( token ) ) {
            ++token;
            return true;
        } else if ( isStreamArray( token ) ) {
            while ( token && *token != "}" ) {
                *this << *token;
                ++token;
            }
            ++token;
            return true;
        } else if ( isStreamProperty( token ) ) {
            Property prop( *token );
            token += 2;
            prop.setValue( *token );
            *this << prop;
            ++token;
        } else if ( isStreamEntry( token ) ) {
            Entry e;
            const bool b = e.fromStream( token, errOut );
            if ( !b ) {
                return false;
            }
            *this << e;
        }
    }

    // we should already exit above while loop with isStreamEnd()
    if ( errOut ) {
        *errOut = "Unable to find closing brace '}' for entry in file";
    }
    return false;
}

static bool startsWith( std::string& str, int8_t c ) {
    std::string::const_iterator it = str.begin();
    while ( it != str.end() ) {
        if ( isblank( *it ) ) {
            it++;
            continue;
        }
        return *it == c;
    }
    return false;
}

static std::vector<std::string> scanTokens( std::ifstream* stream ) {
    assert( stream );
    std::vector<std::string> tokens;
    std::string line;
    while ( *stream && !stream->eof() && std::getline( *stream, line ) ) {
        if ( line.empty() || startsWith( line, '#' ) ) {
            continue;
        }
        std::stringstream ss( line );
        std::istream_iterator<std::string> begin( ss );
        std::istream_iterator<std::string> end;
        tokens.insert( tokens.end(), begin, end );
    }

    return tokens;
}

bool Entry::fromFile( const std::string& fileName, Entry* entry, std::string* errOut ) {
    assert( entry );
    std::ifstream file( fileName );
    if ( !file.is_open() ) {
        const int errorId = errno;
        if ( errOut ) {
            std::stringstream ss;
            ss << "Cannot open file '";
            ss << fileName;
            ss << "': ";
            ss << strerror( errorId );
            *errOut = ss.str();
        }
        return false;
    }

    const std::vector<std::string> vec( scanTokens( &file ) );
    file.close();

    if ( vec.empty() ) {
        if ( errOut ) {
            *errOut = "File is empty";
        }
        return false;
    }

    Token token( vec );
    return entry->fromStream( token, errOut );
}


Entry& Entry::operator << ( const Entry& e ) {
    m_entries.push_back( e );
    return *this;
}

Entry& Entry::operator << ( const Property& p ) {
    m_properties.insert( p );
    return *this;
}

Entry& Entry::operator << ( const std::string& a ) {
    m_array.push_back( a );
    return *this;
}
