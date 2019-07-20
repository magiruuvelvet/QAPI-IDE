#include <script/script.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{

    if (ScriptingInterface::hasJavaScriptSupport())
    {

    describe("JavaScript", []{
        it("[static value binding]", [&]{
            Script scr(Script::JavaScript);
            std::string output;
            scr.setScriptContents("console.print('' + number + string);");

            AssertThat(scr.bindValue("number", 42), Equals(true));
            AssertThat(scr.bindValue("string", std::string{"hello"}), Equals(true));
            AssertThat(scr.evaluate(output), Equals(0));
            AssertThat(output, Equals("42hello\n"));
        });

        it("[error stream]", [&]{
            Script scr(Script::JavaScript);
            std::string output, error;
            scr.setScriptContents(R"(console.error("this is an error");)");

            AssertThat(scr.evaluate(output, error), Equals(0));
            AssertThat(output, Equals(""));
            AssertThat(error, Equals("this is an error\n"));
        });

        it("[return value]", [&]{
            Script scr(Script::JavaScript);
            scr.setScriptContents("return 42;");
            auto ret = scr.evaluate();
            AssertThat(ret, Equals(42));
        });

        it("[if statement]", [&]{
            Script scr(Script::JavaScript);
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
