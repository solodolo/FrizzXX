/*
 * frizz_config.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: dmmettlach
 */

#include <fstream>

#include "frizz_config.h"
#include "nlohmann/json.hpp"

void Frizz::FrizzConfig::load_configuration(std::string path_to_config) {
    std::ifstream input(path_to_config);
    nlohmann::json json_config;

    input >> json_config;

    if(json_config.contains("static_files_root")) {
        this->static_files_root = json_config["static_files_root"];
    }

    if(json_config.contains("build_path")) {
        this->build_path = json_config["build_path"];
    }
}

std::string Frizz::FrizzConfig::get_static_files_root() {
    return this->static_files_root;
}

std::string Frizz::FrizzConfig::get_build_path() {
    return this->build_path;
}