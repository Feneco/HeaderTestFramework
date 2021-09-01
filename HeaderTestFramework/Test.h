#pragma once
#include <iostream>
#include <string>

// MACROS #####################################################################

#define ASSERT_THROW(func)  assert_throw([this]() { (func); } );
#define ASSERT_NO_THROW(func)  assert_no_throw([this]() { (func); } );
#define ASSERT_TRUE(func)  assert_true([this]() { return (func); } );
#define ASSERT_FALSE(func)  assert_false([this]() { return (func); } );
#define ASSERT_EQ(x, func)  assert_eq(x, [this]() { return (func); } );
#define ASSERT_NO_EQ(x, func)  assert_no_eq(x, [this]() { return (func); } );

// DEFINITIONS ################################################################

/*!
Framework de testes em cabeçalho
\author Wagner C.C. Batalha
*/
class TEST {
private:
    unsigned int tests = 0;
    unsigned int successes = 0;
    unsigned int failures = 0;
    void test_failure(std::string details);
    void test_success(std::string details);
    std::string tested_class_name;
public:
    void set_name(std::string tested_class_name);

    // Asserções com throws
    template<typename Proc>
    void assert_no_throw(Proc f);
    template<typename Proc>
    void assert_throw(Proc f);

    // Asserções com bool
    template<typename Proc>
    void assert_true(Proc f);
    template<typename Proc>
    void assert_false(Proc f);

    // Asserção eq
    template<typename type, typename Proc>
    void assert_eq(type x, Proc f);
    template<typename type, typename Proc>
    void assert_no_eq(type x, Proc f);

    virtual void execution() = 0;
    void run();
};

// DECLARATIONS ###############################################################

inline void TEST::set_name(std::string tested_class_name)
{
    this->tested_class_name = '"' + tested_class_name + '"';
}


inline void TEST::test_failure(std::string details)
{
    std::cout << " - Failure in test " << details << std::endl;
    failures += 1;
    tests += 1;
}


inline void TEST::test_success(std::string details)
{
    std::cout << " + " << details << " Passed " << std::endl;
    successes += 1;
    tests += 1;
}


template<typename Proc>
inline void TEST::assert_no_throw(Proc f)
{
    try
    {
        f();
        test_success(__func__);
    }
    catch (...)
    {
        test_failure(__func__);
    }
}


template<typename Proc>
inline void TEST::assert_throw(Proc f)
{
    try
    {
        f();
        test_failure(__func__);
    }
    catch (...)
    {
        test_success(__func__);
    }
}


template<typename Proc>
inline void TEST::assert_true(Proc f)
{
    if (f())
    {
        test_success(__func__);
    }
    else
    {
        test_failure(__func__);
    }
}


template<typename Proc>
inline void TEST::assert_false(Proc f)
{
    if (f())
    {
        test_failure(__func__);
    }
    else
    {
        test_success(__func__);
    }
}


template<typename type, typename Proc>
inline void TEST::assert_eq(type x, Proc f)
{
    if (f() == x)
    {
        test_success(__func__);
    }
    else
    {
        test_failure(__func__);
    }
}


template<typename type, typename Proc>
inline void TEST::assert_no_eq(type x, Proc f)
{
    if (f() == x)
    {
        test_failure(__func__);
    }
    else
    {
        test_success(__func__);
    }
}


inline void TEST::run()
{
    std::cout << "Tests in " << tested_class_name << std::endl;
    try
    {
        execution();
    }
    catch (...)
    {
        std::cout << "The test in " << tested_class_name << " got unexpected exceptions!" << std::endl << std::endl;
        throw;
    }

    std::cout << "Summary of " << tested_class_name << ": ";
    if (failures)
    {
        std::cout << failures << " failure" << ((failures > 1) ? "s" : "") << " in " << tests << " test" << ((tests > 1) ? "s" : "") << std::endl << std::endl;
    }
    else
    {
        std::cout << successes << " succes" << ((successes > 1) ? "ses" : "s") << " and 0 failures" <<std::endl << std::endl;
    }
}
