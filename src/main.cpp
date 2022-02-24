#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "tokenize.hh"
#include "parse.hh"
#include "symbols.hh"
#include "builtin.hh"
#include "eval.hh"
#include "env.hh"

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/tilde.h>
static char *history_file = nullptr;
#endif

bool getline(bool term, const char *prompt, std::string &line)
{
#ifdef HAVE_READLINE
    if (term) {
        char *res = readline(prompt);
        if (res == nullptr) return false;

        add_history(res);
        append_history(1, history_file);
        line = res;
        return true;
    }
#endif
    std::cout << prompt;
    std::getline(std::cin, line);
    if (std::cin.eof()) return false;
    return true;
}

int repl(bool terminal)
{
    std::string line;
    Symbols s;
    Env env;
    Builtin b(s);
    b.install(&env);
    while (!std::cin.eof()) {
        if (!getline(terminal, terminal ? "ligma> " : "", line))
            break;

        try {
            Value *ev = evalLine(s, env, line);
            if (ev != nullptr) {
                std::cout << ev->toString() << "\n";
            } else std::cout << "nil\n";
        }
        catch (std::string s) {
            std::cerr << s << "\n";
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
#ifdef HAVE_READLINE
    history_file = tilde_expand("~/.ligma-history");
    read_history(history_file);
#endif
    return repl(isatty(fileno(stdin)));
}
