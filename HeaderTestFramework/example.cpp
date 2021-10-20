#include <algorithm>
#include <iostream>
#include <string>
#include <ctype.h>
#include <stdexcept>
#include "Test.h"

// Exemplo de classe a ser testada
class Password {
private:
    std::string value;
public:
    void set(std::string pass)
    {
        auto check_alpha = [](char& c) -> void
        {
            if (!std::isalpha(c))
            {
                throw std::invalid_argument("c isn't alpha");
            }
        };
        std::for_each(pass.begin(), pass.end(), check_alpha);
        value = pass;
    }

    std::string get() const
    {
        return value;
    }

    bool compare(std::string pass) const
    {
        return pass == value;
    }
};

// Classe de test que herda TEST
class Password_Test : public TEST
{
public:
    Password_Test() {
        set_name(typeid(this).name());
    }

    void execution() override
    {
        Password p;
        assert_throw( [&]() {
                p.set("123");
            });
        assert_no_throw([&]() {
                p.set("abc");
            });

    }
};


int main()
{
    // Execução do teste.
    Password_Test password_test;
    password_test.run();
}
