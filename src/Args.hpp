
#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <list>

/// TODO

class Args
{
    public:
        Args(int argc, char** args);
        ~Args() {}

        std::list<std::string> args;

    private:

    protected:
};

#endif // ARGS_H
