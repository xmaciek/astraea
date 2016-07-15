#pragma once

#include <cstdint>
#include <fstream>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include "property.hpp"

// for convinient iterators usage
// Entries::const_iterator it = m_entries.begin();
class Entry;
class Token;
typedef std::set<Property> Properties;
typedef std::vector<Entry> Entries;
typedef std::vector<std::string> Array;

class Entry {
public:
    Entry();
    const uint64_t m_id;
    std::string m_name;
    Properties m_properties;
    Entries m_entries;
    Array m_array;

    Entry& operator << ( const Entry& );
    Entry& operator << ( const Property& );
    Entry& operator << ( const std::string& );

    bool isArray() const;
    bool isValid() const;
    bool writeToFile( const std::string& fileName, uint64_t intend = 4, std::string* errOut = 0 ) const;

    static bool fromFile( const std::string&, Entry* entry, std::string* errOut = 0 );

    bool writeToStream( std::ostream &f, uint64_t intend, uint64_t intendOffset, std::string* errOut = 0 ) const;
private:
    bool fromStream( Token &token, std::string* errOut = 0 );

};


