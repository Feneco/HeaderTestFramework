#pragma once
#include <iostream>
#include <string>

// MACROS #####################################################################

#define ASSERT_THROW(func)  assert_throw([&]() { (func); } );
#define ASSERT_NO_THROW(func)  assert_no_throw([&]() { (func); } );
#define ASSERT_TRUE(func)  assert_true([&]() { return (func); } );
#define ASSERT_FALSE(func)  assert_false([&]() { return (func); } );
#define ASSERT_EQ(x, func)  assert_eq(x, [&]() { return (func); } );
#define ASSERT_NO_EQ(x, func)  assert_no_eq(x, [&]() { return (func); } );

// DEFINITIONS ################################################################

#ifndef HTF_INCLUDE
#define HTF_INCLUDE

/*! \brief Framework de testes em cabeçalho
*
* Classe para testes de unidade. Feito para ser versátil, nítido e claro.
* Utiliza lambdas para executar testes
*
* ## Utilização
*
* Cria-se uma classe que herda de TEST. Define-se então nesta classe o procedimento
* ao dar override na função virtual execution() usando:
*
* - assert_no_throw(Proc f)
* - assert_throw(Proc f)
* - assert_true(Proc f)
* - assert_false(Proc f)
* - assert_eq(type x, Proc f)
* - assert_no_eq(type x, Proc f)
*
* Para rodar os testes cria-se um objeto desta classe e chama-se a função run().
* O procedimento definido em execution() será executaro e ao final dos tests será
* exibido um resumo com a quantidade de sucessos e falhas.
*
* \author Wagner C.C. Batalha
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
    /*! \brief Configura o título do teste a ser exibido no resumo de testes.
    *
    * \parm tested_class_name título do teste.
    */
    void set_name(std::string tested_class_name);

    /*! \brief Asserção de inexistência de throw.
    *
    * \param f Lambda do processo a ser executado.
    */
    template<typename Proc>
    void assert_no_throw(Proc f);

    /*! \brief Asserção de existência de throw.
    *
    * \param f Lambda do processo a ser executado.
    */
    template<typename Proc>
    void assert_throw(Proc f);

    /*! \brief Asserção de retorno true
    *
    * \param f Lambda do processo a ser executado. Este lambda deverá retornar um
    * valor booleano.
    */
    template<typename Proc>
    void assert_true(Proc f);

    /*! \brief Asserção de retorno false
    *
    * \param f Lambda do processo a ser executado. Este lambda deverá retornar um
    * valor booleano.
    */
    template<typename Proc>
    void assert_false(Proc f);

    /*! \brief Asserção de valor idêntico a x
    *
    * \param x O valor de comparação.
    * \param f Lambda do processo a ser executado. Este lambda deverá retornar um
    * do mesmo tipo que x.
    */
    template<typename type, typename Proc>
    void assert_eq(type& x, Proc f) const;

    /*! \brief Asserção de valor diferente a x
    *
    * \param x O valor de comparação.
    * \param f Lambda do processo a ser executado. Este lambda deverá retornar um
    * do mesmo tipo que x.
    */
    template<typename type, typename Proc>
    void assert_no_eq(type& x, Proc f);

    /*! \brief Procedimento de testes
    *
    * A Classe que herdar de TEST deverá dar override nesta função virtual com um
    * procedimento de testes usando assert_no_throw(Proc f), assert_throw(Proc f),
    * assert_true(Proc f), assert_false(Proc f), assert_eq(type x, Proc f), assert_no_eq(type x, Proc f)
    *
    * Caso haja uma exceção inesperada, ele parará o teste.
    */
    virtual void execution() = 0;

    /*! \brief Retorna quantidade de erros */
    int get_failure_qtd() const;

    /*! \brief Execução do procedimento definido em execution()
    */
    void run();
};

// DECLARATIONS ###############################################################

inline void TEST::set_name(std::string tested_class_name) {
    this->tested_class_name = '"' + tested_class_name + '"';
}


inline void TEST::test_failure(std::string details) {
    std::cout << " - Failure in test " << details << std::endl;
    failures += 1;
    tests += 1;
}


inline void TEST::test_success(std::string details) {
    std::cout << " + " << details << " Passed " << std::endl;
    successes += 1;
    tests += 1;
}


template<typename Proc>
inline void TEST::assert_no_throw(Proc f) {
    try {
        f();
        test_success(__func__);
    }
    catch (...) {
        test_failure(__func__);
    }
}


template<typename Proc>
inline void TEST::assert_throw(Proc f) {
    try {
        f();
        test_failure(__func__);
    }
    catch (...) {
        test_success(__func__);
    }
}


template<typename Proc>
inline void TEST::assert_true(Proc f) {
    if (f()) {
        test_success(__func__);
    }
    else {
        test_failure(__func__);
    }
}


template<typename Proc>
inline void TEST::assert_false(Proc f) {
    if (!f()) {
        test_success(__func__);
    }
    else {
        test_failure(__func__);
    }
}


template<typename type, typename Proc>
inline void TEST::assert_eq(type& x, Proc f) const {
    if (f() == x) {
        test_success(__func__);
    }
    else {
        test_failure(__func__);
    }
}


template<typename type, typename Proc>
inline void TEST::assert_no_eq(type& x, Proc f) {
    if (f() == x) {
        test_failure(__func__);
    }
    else {
        test_success(__func__);
    }
}


inline void TEST::run() {
    std::cout << "Tests in " << tested_class_name << std::endl;
    try {
        execution();
    }
    catch (const std::exception& e) {
        std::cout << "The test in " << tested_class_name << " got unexpected exceptions!" << std::endl;
        std::cout << e.what() << std::endl << std::endl;
        throw;
    }

    std::cout << "Summary of " << tested_class_name << ": ";
    if (failures) {
        std::cout << failures << " failure" << ((failures > 1) ? "s" : "") << " in " << tests << " test" << ((tests > 1) ? "s" : "") << std::endl << std::endl;
    }
    else {
        std::cout << successes << " succes" << ((successes > 1) ? "ses" : "s") << " and 0 failures" << std::endl << std::endl;
    }
}

inline int TEST::get_failure_qtd() const
{
    return failures;
}

#endif // HTF_INCLUDE
