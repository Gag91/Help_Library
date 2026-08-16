namespace hp {
    namespace safe {

        // ----- Addition (checks overflow + underflow)
        template <typename T>
        [[nodiscard]] constexpr bool Add(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::safe::Add: Arguments must be arithmetic");

            return hp::overflow::check::Add(a, b) ||
                hp::underflow::check::Add(a, b);
            }

        // ----- Subtraction (checks overflow + underflow)
        template <typename T>
        [[nodiscard]] constexpr bool Sub(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::safe::Sub: Arguments must be arithmetic");

            return hp::overflow::check::Sub(a, b) ||
                hp::underflow::check::Sub(a, b);
            }

        // ----- Multiplication (checks overflow + underflow)
        template <typename T>
        [[nodiscard]] constexpr bool Mult(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::safe::Mult: Arguments must be arithmetic");

            return hp::overflow::check::Mult(a, b) ||
                hp::underflow::check::Mult(a, b);
            }

        // ----- Division (checks division by zero + overflow)
        template <typename T>
        [[nodiscard]] constexpr bool Div(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::safe::Div: Arguments must be arithmetic");

            return hp::overflow::check::Div(a, b) ||
                hp::underflow::check::Div(a, b);
            }

        // ----- Check if ANY operation is safe
        template <typename T>
        [[nodiscard]] constexpr bool any(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::safe::any: Arguments must be arithmetic");

            return Add(a, b) || Sub(a, b) || Mult(a, b) || Div(a, b);
            }

        // ----- Check if ALL operations are safe
        template <typename T>
        [[nodiscard]] constexpr bool none(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::safe::none: Arguments must be arithmetic");

            return !any(a, b);
            }
        }
    }