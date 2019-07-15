#include <script/script.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{
    describe("C++", []{
        it("[value binding]", [&]{
            Script scr(Script::Cpp);
            std::string output;
            scr.setScriptContents("out << number << string << std::endl;");

            AssertThat(scr.bindValue("number", 42), Equals(true));
            AssertThat(scr.bindValue("string", std::string{"hello"}), Equals(true));
            AssertThat(scr.evaluate(output), Equals(0));
            AssertThat(output, Equals("42hello\n"));
        });
    });
});
