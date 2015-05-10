#include <iostream>
#include <vector>

#include "netsniffer.h"


using namespace std;

int main(int argc, char *argv[])
{

    try {

        NetSniffer app;
        return app.run(argc, argv);
    }
    catch (Poco::Exception &e)
    {
        cerr << e.message() << ". code: " << e.code();
        Poco::Util::Application::EXIT_SOFTWARE;
    }

    return 1;
}
