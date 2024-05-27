#pragma once

#include "types.hpp"
#include "meta.hpp"
#include "node.hpp"

namespace efesx {
namespace queue {
namespace detail {

std::ostream& operator<<(std::ostream& out, const node& val){
    out << val.m_type << " : ";

    switch (val.m_type)
    {
    case value_t::int_t: { 
        out << val.m_data.int_value; 
        break; 
    }
    case value_t::char_t: { 
        switch (val.m_data.char_value)
        {
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        
        default: out << std::string{val.m_data.char_value}; break;
        }
        break; 
    }
    case value_t::bool_t: { 
        out << std::boolalpha << val.m_data.bool_value; 
        break; 
    }
    case value_t::string_t: { 
        out << val.m_data.string_value->c_str(); 
        break; 
    }

    case value_t::container_t: 
    case value_t::array_t: {
        out << "[";

        for(auto it = val.m_data.array_value->begin(); it != val.m_data.array_value->end();){
            out << *it;
            
            if(++it != val.m_data.array_value->end()){
                std::cout << ", ";
            }
        }

        out << "]"; 
        break; 
    }
    default: {out << "other"; break; }
    }
    return out;
}


}; // namespace detail
}; // namespace queue
}; // namespace efesx
