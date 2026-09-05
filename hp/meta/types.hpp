#pragma once

#include <meta>
#include <string_view>
 
namespace hp {

    // ----- Returns the type of a value
    template <typename T>
    consteval std::string_view type_of(T&& a) {
        return std::meta::display_string_of(^^T);
    }

    
}