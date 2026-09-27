#pragma once

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
    struct is_smart_pointer : std::false_type {};
    template <typename T>
    struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};
    template <typename T>
    struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

    template <typename>
    struct is_unique_ptr : std::false_type {};
    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    template <typename>
    struct is_shared_ptr : std::false_type {};
    template <typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

    // ----- Saving Data
    template <typename T = std::string>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    bool save(const std::string &filename, const std::vector<std::pair<std::string, T>> &args) {
        std::ofstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            return false;
        }

        for (size_t i = 0; i < args.size(); i++) {
            file << args[i].first << ": " << args[i].second << "\n";
        }

        return file.good();
    }

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
                std::cout << "its a pointer\n";
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

    // ----- Loading Data
    template <typename T = std::string>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    bool load(const std::string &filename, const std::vector<std::pair<std::string, T *>> &arg) {
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            for (size_t i = 0; i < arg.size(); i++) {
                if (line.rfind(arg[i].first + ": ", 0) == 0) {
                    std::string value = line.substr(arg[i].first.size() + 2);
                    if constexpr (std::is_same_v<T, std::string>) {
                        *arg[i].second = value;
                    } else {
                        std::stringstream ss(value);
                        ss >> *arg[i].second;
                    }
                    break;
                }
            }
        }
        return true;
    }

    // ----- Loading specified data
    template <typename T = std::string>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    std::optional<T> load(const std::string &filename, const std::string &arg) {
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            return std::nullopt;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.rfind(arg + ": ", 0) == 0) {
                std::string value = line.substr(arg.size() + 2);

                if constexpr (std::is_same_v<T, std::string>) {
                    return value;
                } else {
                    std::stringstream ss(value);
                    T result;
                    if (ss >> result) {
                        return result;
                    }
                    return std::nullopt;
                }
            }
        }
        return std::nullopt;
    }

} // namespace hp