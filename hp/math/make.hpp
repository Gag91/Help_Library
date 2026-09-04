#pragma once

#include <limits>
#include <type_traits>

namespace hp
{
    namespace math
    {
        namespace make
        {

            template <typename T>
            constexpr T positive(T a)
            {
                static_assert(std::is_arithmetic_v<T>, "Argument must be arithmetic");
                if (a > 0)
                {
                    return a;
                }
                if (a < 0)
                {
                    if (a == std::numeric_limits<T>::min())
                    {
                        return std::numeric_limits<T>::max();
                    }
                    return -a;
                }
                return 0;
            }

            // ----- Convert to negative
            template <typename T>
            constexpr T negative(T a)
            {
                static_assert(std::is_arithmetic_v<T>, "Argument must be arithmetic");
                if (a < 0)
                {
                    return a;
                }
                if (a > 0)
                {
                    if (a == std::numeric_limits<T>::max())
                    {
                        return std::numeric_limits<T>::min();
                    }
                    return -a;
                }
                return 0;
            }

            // ----- Convert to int
            template <typename T>
            constexpr auto Int(T a) -> int
            {
                static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
                if constexpr (std::is_floating_point_v<T>)
                {
                    if (a > static_cast<T>(std::numeric_limits<int>::max()) ||
                        a < static_cast<T>(std::numeric_limits<int>::min()))
                    {
                        return (a > 0) ? std::numeric_limits<int>::max()
                                       : std::numeric_limits<int>::min();
                    }
                    return static_cast<int>(a);
                }
                else
                {
                    return a;
                }
            }

            // ----- Convert to float
            template <typename T>
            constexpr auto Float(T a) -> float
            {
                static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
                if constexpr (std::integral<T>)
                {
                    if (static_cast<long double>(a) > std::numeric_limits<float>::max() ||
                        static_cast<long double>(a) < std::numeric_limits<float>::lowest())
                    {
                        return (a > 0) ? std::numeric_limits<float>::max()
                                       : std::numeric_limits<float>::lowest();
                    }
                    return static_cast<float>(a);
                }
                else
                {
                    return a;
                }
            }

            // ----- Convert to double
            template <typename T>
            constexpr auto Double(T a) -> double
            {
                static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
                if constexpr (std::integral<T>)
                {
                    if (a > static_cast<T>(std::numeric_limits<double>::max()) ||
                        a < static_cast<T>(std::numeric_limits<double>::lowest()))
                    {
                        return (a > 0) ? std::numeric_limits<double>::max()
                                       : std::numeric_limits<double>::lowest();
                    }
                    return static_cast<double>(a);
                }
                else
                {
                    return a;
                }
            }

            // ----- Convert to long double
            template <typename T>
            constexpr auto LongDouble(T a) -> long double
            {
                static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
                if constexpr (std::integral<T>)
                {
                    if (a > static_cast<T>(std::numeric_limits<long double>::max()) ||
                        a < static_cast<T>(std::numeric_limits<long double>::lowest()))
                    {
                        return (a > 0) ? std::numeric_limits<long double>::max()
                                       : std::numeric_limits<long double>::lowest();
                    }
                    return static_cast<long double>(a);
                }
                else
                {
                    return a;
                }
            }

            // ----- Convert to long long
            template <typename T>
            constexpr auto LongLong(T a) -> long long
            {
                static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
                if constexpr (std::is_floating_point_v<T>)
                {
                    if (a > static_cast<T>(std::numeric_limits<long long>::max()) ||
                        a < static_cast<T>(std::numeric_limits<long long>::min()))
                    {
                        return (a > 0) ? std::numeric_limits<long long>::max()
                                       : std::numeric_limits<long long>::min();
                    }
                    return static_cast<long long>(a);
                }
                else
                {
                    return a;
                }
            }

        } // namespace make
    } // namespace math
} // namespace hp