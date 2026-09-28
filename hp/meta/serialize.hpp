#pragma once

#include "hp/concepts/concept.hpp"
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

    template <typename T>
    bool load_class(std::istream &file, T &value);

    template <typename T>
    bool save_class(std::ostream &file, T &value);

    // ----- Saving data from structs/classes members
    template <typename T>
    bool save(const std::string &filename, T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        std::ofstream file(filename, std::ios::out | std::ios::binary);
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
            } else if constexpr (std::is_same_v<Mtype, std::string>) {
                std::string result;
                for (const char &c : value.[:m:]) {
                    switch (c) {
                        case '"':
                            result += "\\\"";
                            break;
                        case '\\':
                            result += "\\\\";
                            break;
                        case '\n':
                            result += "\\n";
                            break;
                        case '\t':
                            result += "\\t";
                            break;
                        case '\r':
                            result += "\\r";
                            break;
                        default:
                            result += c;
                    }
                }
                file << result << "\n";
            } else if constexpr (Writable<Mtype>) {
                file << value.[:m:] << "\n";
            } else if constexpr (std::is_class_v<Mtype>) {
                save_class(file, value.[:m:]);
                file << "\n";
            } else {
                file << value.[:m:] << "\n";
            }
        }
        return file.good();
    }

    // ----- Saving data from structs/classes
    template <typename T>
    bool save_class(std::ostream &file, T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        file << std::meta::identifier_of(^^T) << " { ";
        bool first = true;

        template for (constexpr auto m : members) {
            if (!first) {
                file << ", ";
            }
            first = false;

            using Mtype = [:std::meta::type_of(m):];
            file << std::meta::identifier_of(m) << ": ";

            if constexpr (std::is_same_v<Mtype, std::string>) {
                std::string result;
                for (const char &c : value.[:m:]) {
                    switch (c) {
                        case '"':
                            result += "\\\"";
                            break;
                        case '\\':
                            result += "\\\\";
                            break;
                        case '\n':
                            result += "\\n";
                            break;
                        case '\t':
                            result += "\\t";
                            break;
                        case '\r':
                            result += "\\r";
                            break;
                        default:
                            result += c;
                    }
                }
                file << result;
            } else if constexpr (Writable<Mtype>) {
                file << value.[:m:];
            } else if constexpr (std::is_class_v<Mtype>) {
                save_class(file, value.[:m:]);
            } else {
                file << value.[:m:];
            }
        }

        file << " }";
        return file.good();
    }

    // ----- Loading Data from structs/classes members
    template <typename T>
    bool load(const std::string &filename, T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));
        std::ifstream file(filename);
        if (!file.is_open())
            return false;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;

            template for (constexpr auto m : members) {
                std::size_t pos = line.find(":");
                if (pos == std::string::npos)
                    continue;

                std::string identifier = line.substr(0, pos);
                std::string n_value = line.substr(pos + 2);

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
                    } else if constexpr (std::is_class_v<Mtype> && !Writable<Mtype>) {
                        std::istringstream ss(n_value);
                        load_class(ss, value.[:m:]);
                    } else {
                        value.[:m:] = hp::str::from_string<Mtype>(n_value);
                    }
                }
            }
        }
        return file.good();
    }

    // ----- Loading Data from structs/classes
    template <typename T>
    bool load_class(std::istream &file, T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        std::string token;
        file >> token;
        file >> token;

        template for (constexpr auto m : members) {
            using Mtype = std::remove_cvref_t<decltype(value.[:m:])>;

            file >> token;
            if (token.ends_with(':')) {
                token.pop_back();
            }

            if (token == std::meta::identifier_of(m)) {
                std::string n_value;
                file >> n_value;

                if (n_value.ends_with(',') || n_value.ends_with('}')) {
                    n_value.pop_back();
                }

                if constexpr (std::is_class_v<Mtype> && !Writable<Mtype>) {
                    load_class(file, value.[:m:]);
                } else {
                    value.[:m:] = hp::str::from_string<Mtype>(n_value);
                }
            }
        }
        return file.good();
    }
} // namespace hp
