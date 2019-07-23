#include <script/script.hpp>

#include <bandit/bandit.h>

#include <benchmark.hpp>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{

    if (ScriptingInterface::hasCppSupport())
    {

    describe("C++", []{
        benchmark_it("[static value binding]", [&]{
            Script scr(Script::Cpp);
            std::string output;
            scr.setScriptContents("out << number << string << std::endl;");

            AssertThat(scr.bindValue("number", 42), Equals(true));
            AssertThat(scr.bindValue("string", std::string{"hello"}), Equals(true));
            AssertThat(scr.evaluate(output), Equals(0));
            AssertThat(output, Equals("42hello\n"));
        });

        benchmark_it("[error stream]", [&]{
            Script scr(Script::Cpp);
            std::string output, error;
            scr.setScriptContents(R"(err << "this is an error" << std::endl;)");

            AssertThat(scr.evaluate(output, error), Equals(0));
            AssertThat(output, Equals(""));
            AssertThat(error, Equals("this is an error\n"));
        });

        benchmark_it("[return value]", [&]{
            Script scr(Script::Cpp);
            scr.setScriptContents("return 42;");
            auto ret = scr.evaluate();
            AssertThat(ret, Equals(42));
        });

        benchmark_it("[if statement]", [&]{
            Script scr(Script::Cpp);
            std::string output;
            scr.setScriptContents(R"(
                if (number == 1)
                {
                    return 1;
                }
                else if (number == 2)
                {
                    return 2;
                }
                else
                {
                    return 3;
                }
            )");
            scr.bindValue("number", 2);
            auto ret = scr.evaluate(output);
            AssertThat(output, Equals(""));
            AssertThat(ret, Equals(2));
        });
    });

    }
});
