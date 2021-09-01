#include <algorithm>
#include <iostream>
#include <string>
#include <ctype.h>
#include <stdexcept>
#include "Test.h"

// Example of class to be tested: a simple password storage class.
// It has two member functions: get and set
// The set function guarantees that the password should only contain letters
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

    std::string get()
    {
        return value;
    }

    bool compare(std::string pass)
    {
        return pass == value;
    }
};

// The test class
// inherit both the class TEST and the class to be
// tested in it (Password in this example).
// In the constructor use the member set_name to set the name of the class
// being tested.
// In the member execution() we determine the test procedure either using
// macros or calling the TEST assertions. Use what you prefer.
//
// Macros make it more concise in my opinion, but aren't recommended by c++
// standards. Should you use the assertions remember that it uses lambdas, and
// assert_true(), assert_false(), assert_eq() and assert_no_eq() require an
// return call in the lambda.
//
// You also do not need to inherit the class to be tested, you can simply
// create an object inside execution() and proceed with it.
class Password_Test : public TEST, public Password
{
public:
    Password_Test() {
        set_name(typeid(this).name());
    }

    void execution() override
    {
        ASSERT_THROW(set("123"));
        ASSERT_NO_THROW(set("abc"));
        ASSERT_TRUE(compare("abc"));
        set("wag");
        ASSERT_FALSE(compare("abc"));
        ASSERT_EQ("wag", get());
        ASSERT_NO_EQ("abc", get());
        //assert_throw([this]()        { set("123"); });
        //assert_no_throw([this]()     { set("abc"); });
        //assert_true([this]()         { set("abc"); return compare("abc"); });
        //assert_false([this]()        { set("def"); return compare("abc"); });
        //assert_eq("wag", [this]()    { set("wag"); return get(); });
        //assert_no_eq("abc", [this]() { return get(); });
    }
};


int main()
{
    // Solves terminal foolery
    setlocale(LC_ALL, "");
    // Test execution
    Password_Test password_test;
    password_test.run();
}
