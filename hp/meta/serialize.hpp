#pragma once

#include "hp/other/type_traits.hpp"
#include "hp/string/string.hpp"
#include <fstream>
#include <meta>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace hp {

    // ----- Saving data from structs/classes
    template <typename T>
    bool save(const std::string &filename, T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        std::ofstream file(filename);
        if (!file.is_open())
            return false;

        template for (constexpr auto m : members) {
            using Mtype = [:std::meta::type_of(m):];

            file << std::meta::identifier_of(m) << ": ";

            if constexpr (is_smart_pointer_v<Mtype>) {
                
                auto &&ptr = value.[:m:];
                if (ptr) {
                    file << *ptr << "\n";
                } else {
                    file << "nullptr\n";
                }
            } else {
                file << value.[:m:] << "\n";
            }
        }
        return file.good();
    }

    // ----- Loading Data from structs/classes
    template <typename T>
    bool load(const std::string &filename, T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            template for (constexpr auto m : members) {
                std::size_t pos = line.find(":");
                std::string n_value = line.substr(pos + 2);
                std::string identifier = line.substr(0, pos);
                using Mtype = std::remove_cvref_t<decltype(value.[:m:])>;
                if (identifier == std::meta::identifier_of(m)) {
                    if constexpr (is_smart_pointer_v<Mtype>) {
                        using Elem = typename Mtype::element_type;

                        if (n_value == "nullptr") {
                            value.[:m:] = nullptr;
                        } else {
                            if constexpr (is_unique_ptr_v<Mtype>) {
                                value.[:m:] = std::make_unique<Elem>(hp::str::from_string<Elem>(n_value));
                            } else if constexpr (is_shared_ptr_v<Mtype>) {
                                value.[:m:] = std::make_shared<Elem>(hp::str::from_string<Elem>(n_value));
                            }
                        }
                    } else {
                        value.[:m:] = hp::str::from_string<Mtype>(n_value);
                    }
                }
            }
        }
        return file.good();
    }
} // namespace hp
