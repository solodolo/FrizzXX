/*
 * frizz_config.h
 *
 *  Created on: Aug 13, 2020
 *      Author: dmmettlach
 */

#ifndef FRIZZ_CONFIG_H_
#define FRIZZ_CONFIG_H_

#include <string>

namespace Frizz {
    class FrizzConfig {
        public:
            void load_configuration(std::string path_to_config);
            std::string get_static_files_root();
            std::string get_build_path();

        private:
            std::string static_files_root;
            std::string build_path;
    };
}

#endif