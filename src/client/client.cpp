#include <iostream>
#include <fstream>

#include <vector>

#include <boost/program_options.hpp>

#include "client/client_process.hpp"
#include "client/client_cfg.hpp"
#include "inet_address.hpp"
#include "log/logger.hpp"


int parse_cfg(int& argc, const char** &argv) {



    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "help message")
            ("config-file",              boost::program_options::value<std::string>(), "set compression level");
//            ("client.bind_ip",           boost::program_options::value<std::string>(), "set client's ip bind address")
//            ("client.bind_port",         boost::program_options::value<int>(),         "set client's ip bind address")
//            ("client.data_block_size",   boost::program_options::value<int>(),         "set client's ip bind address")
//            ("client.data_check",        boost::program_options::value<bool>(),        "set client's ip bind address")
//            ("client.data_check_method", boost::program_options::value<std::string>(), "set client's ip bind address")
//            ("server.ip",                boost::program_options::value<std::string>(), "set client's ip bind address")
//            ("server.port",              boost::program_options::value<int>(),         "set client's ip bind address")
//            ("server.send_to_ip",        boost::program_options::value<std::string>(), "set client's ip bind port")
//            ("server.send_to_port",      boost::program_options::value<int>(),         "set client's ip bind port");
    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(
                boost::program_options::parse_command_line(argc, argv, desc), vm
        );

        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        boost::program_options::variables_map config_vm;
        if (vm.count("config-file")) {
            std::cout << vm["config-file"].as<std::string>();

            std::ifstream config_file(vm["config-file"].as<std::string>());
            boost::program_options::store(
                    boost::program_options::parse_config_file(config_file, desc), config_vm
            );
            config_file.close();

            boost::program_options::notify(config_vm);

        }
        else {
            std::cerr << "error: config-file not set" << std::endl;
            std::cerr << desc << std::endl;
            return -1;
        }

        put::c_cfg = new put::ClientCfg(
                put::InetAddress(
                        config_vm["client.bind_ip"].as<std::string>().c_str(),
                        static_cast<in_port_t >(config_vm["client.bind_port"].as<int>())
                ),
                config_vm["client.data_block_size"].as<int>(),
                config_vm["client.data_check"].as<bool>(),
                config_vm["client.data_check_method"].as<std::string>(),
                put::InetAddress(
                        config_vm["client.bind_ip"].as<std::string>().c_str(),
                        static_cast<in_port_t >(config_vm["client.bind_port"].as<int>())
                ),
                put::InetAddress(
                        config_vm["client.bind_ip"].as<std::string>().c_str(),
                        static_cast<in_port_t >(config_vm["client.bind_port"].as<int>())
                )

        );

        return 0;
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return -1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
        return -1;
    }
}


int main(int argc, const char ** argv) {
    LOG_TRACE << "Program Start";
    parse_cfg(argc, argv);
    client_process();
    return 0;
}

