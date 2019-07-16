#include <script/script.hpp>

#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

go_bandit([]{

    if (ScriptingInterface::hasJavaScriptSupport())
    {

    describe("JavaScript", []{
    });

    }
});
