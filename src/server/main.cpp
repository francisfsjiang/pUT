#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <cstdlib>
#include <map>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "main_process.hpp"
#include "cfg.hpp"
#include "inet_address.hpp"
#include "log/logger.hpp"

const int k_HELP_DESC_NUM = 5;

const char* k_HELP_DESC[k_HELP_DESC_NUM][2] = {
        {"h,help",                      "help message"                  },
        {"config-file",                 "set compression level"         },
        {"server.bind_ip",              "set client's ip bind address"  },
        {"server.bind_port",            "set client's ip bind address"  },
        {"server.serve_path",           "set client's ip bind address"  },
};

void parse_variables_to_map(std::map<std::string, std::string>& m, const boost::program_options::variables_map& vm) {
    for(int i = 0; i < k_HELP_DESC_NUM; ++i) {
        if (vm.count(k_HELP_DESC[i][0])) {
            m[k_HELP_DESC[i][0]] = vm[k_HELP_DESC[i][0]].as<std::string>();
        }
    }
}

int parse_cfg(int& argc, const char** &argv) {

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            (k_HELP_DESC[0 ][0] , k_HELP_DESC[0][1])
            (k_HELP_DESC[1 ][0], boost::program_options::value<std::string>(), k_HELP_DESC[1 ][1])
            (k_HELP_DESC[2 ][0], boost::program_options::value<std::string>(), k_HELP_DESC[2 ][1])
            (k_HELP_DESC[3 ][0], boost::program_options::value<std::string>(), k_HELP_DESC[3 ][1])
            (k_HELP_DESC[4 ][0], boost::program_options::value<std::string>(), k_HELP_DESC[4 ][1]);
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

        std::map<std::string, std::string> parsed_cfg;

        if (vm.count("config-file")) {
            boost::program_options::variables_map config_vm;

            std::ifstream config_file(vm["config-file"].as<std::string>());
            boost::program_options::store(
                    boost::program_options::parse_config_file(config_file, desc), config_vm
            );

            boost::program_options::notify(config_vm);

            config_file.close();

            parse_variables_to_map(parsed_cfg, config_vm);
        }
        else {
            std::cerr << "error: config-file not set" << std::endl;
            std::cerr << desc << std::endl;
            return -1;
        }

        parse_variables_to_map(parsed_cfg, vm);

        for(int i = 1; i < k_HELP_DESC_NUM; ++i) {
                LOG_INFO << k_HELP_DESC[i][0] << " : " << parsed_cfg[k_HELP_DESC[i][0]];
        }

        // check serve path
        boost::filesystem::path serve_path(parsed_cfg["server.serve_path"]);
        if (!boost::filesystem::is_directory(serve_path)) {
            LOG_FATAL << "Serve path error";
            return -1;
        }


        put::server::s_cfg = new put::server::ServerCfg(
                put::InetAddress(
                        parsed_cfg["server.bind_ip"].c_str(),
                        static_cast<in_port_t >(atoi(parsed_cfg["server.bind_port"].c_str()))
                ),
                serve_path
        );

        return 0;
    }
    catch(std::exception& e) {
        std::cerr << "Error value: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return -1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
        return -1;
    }
}


int main(int argc, const char ** argv) {
    LOG_TRACE << "Server Start";
    int ret;
    ret = parse_cfg(argc, argv);
    if (ret < 0) {
        LOG_FATAL << "Parse Config Error";
        exit(-1);
    }
    put::server::main_process();
    return 0;
}

