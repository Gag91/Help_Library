#include "hp/meta/meta.hpp"
#include <cassert>
#include <string>
#include <utility>

enum class Role {
    Admin,
    User,
    Guest
};

class Account {
  public:
    int id;
    Role role;
    std::string username;

    Account(int id, Role role, std::string username, std::string secret)
        : id(id), role(role), username(std::move(username)), secret_(std::move(secret)) {}

  private:
    std::string secret_;
};

int main() {
    static_assert(hp::meta::same_as<int, int>());
    static_assert(!hp::meta::same_as<int, double>());

    int x = 10;
    const int &rx = x;
    static_assert(hp::meta::same_as<typeof(x), int>());
    static_assert(hp::meta::same_as<typeof(rx), int>());

    static_assert(hp::meta::type_name<int>() == "int");
    static_assert(hp::meta::member_count<Account>() == 4);

    constexpr auto roles = hp::meta::get_enumerators<Role>();
    static_assert(roles.size() == 3);
    static_assert(roles[0] == Role::Admin);
    static_assert(roles[1] == Role::User);
    static_assert(roles[2] == Role::Guest);

    static_assert(hp::meta::enumerator_name(Role::Admin) == "Admin");
    static_assert(hp::meta::enumerator_name(Role::User) == "User");
    static_assert(hp::meta::enumerator_name(Role::Guest) == "Guest");
    static_assert(hp::meta::enumerator_name(static_cast<Role>(99)).empty());

    Account a1{1, Role::Admin, "alice", "s3cr3t"};
    Account a2{1, Role::Admin, "alice", "s3cr3t"};
    Account a3{2, Role::User, "bob", "pass"};

    assert(hp::meta::equals(a1, a2));
    assert(!hp::meta::equals(a1, a3));

    auto [m0, m1, m2, m3] = hp::meta::get_members(a1);

    static_assert(m0.identifier == "id");
    static_assert(m0.type_name == "int");
    static_assert(m0.size == sizeof(int));
    static_assert(m0.alignment == alignof(int));
    static_assert(m0.is_arithmetic);
    static_assert(m0.is_public);
    static_assert(!m0.is_private);

    static_assert(m1.identifier == "role");
    static_assert(m1.is_enum);
    static_assert(m1.is_public);

    static_assert(m2.identifier == "username");
    static_assert(m2.is_class);
    static_assert(m2.is_public);

    static_assert(m3.identifier == "secret_");
    static_assert(m3.is_private);
    static_assert(!m3.is_public);

    m0.value = 42;
    assert(a1.id == 42);
    assert(!hp::meta::equals(a1, a2));

    std::size_t visited = 0;
    std::size_t public_count = 0;
    hp::meta::for_each_member(a1, [&](auto m) {
        ++visited;
        if constexpr (m.is_public) {
            ++public_count;
        }
        if constexpr (m.identifier == "id") {
            m.value = 100;
        }
    });

    assert(visited == 4);
    assert(public_count == 3);
    assert(a1.id == 100);

    return 0;
}