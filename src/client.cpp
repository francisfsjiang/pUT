#include <iostream>

#include "boost/program_options.hpp"



int parse_command_line(int& argc, const char** &argv) {

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("compression", boost::program_options::value<int>(), "set compression level");
    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(
                boost::program_options::parse_command_line(argc, argv, desc), vm
        );
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        if (vm.count("compression")) {
            std::cout << "Compression level was set to "
            << vm["compression"].as<int>() << ".\n";
        } else {
            std::cout << "Compression level was not set.\n";
        }
        return 0;
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << desc << std::endl;
        return -1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
        return -1;
    }
}


int main(int argc, const char ** argv) {
    parse_command_line(argc, argv);
    return 0;
}

