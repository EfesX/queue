#pragma once

#include <stdint.h>
#include <iostream>

namespace efesx {
namespace queue {
namespace detail {

enum class value_t : uint8_t
{
    null_t = 0,  
    int_t,
    char_t,
    bool_t,  
    string_t,
    array_t,
    object_t,
    container_t,
};

std::ostream& operator<<(std::ostream& out, const value_t& val){
    switch (val)
    {
        case value_t::null_t:       { out << "null_t";      break; }
        case value_t::int_t:        { out << "int_t";       break; }
        case value_t::bool_t:       { out << "bool_t";      break; }
        case value_t::string_t:     { out << "string_t";    break; }
        case value_t::array_t:      { out << "array_t";     break; }
        case value_t::object_t:     { out << "object_t";    break; }
        case value_t::char_t:       { out << "char_t";      break; }
        case value_t::container_t:  { out << "conteiner_t"; break; }
        default:                    {out << "other_t";      break; }
    }
    return out;
}

}; // namespace detail
}; // namespace queue
}; // namespace detail
