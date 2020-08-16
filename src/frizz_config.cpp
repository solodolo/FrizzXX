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

    if(json_config.contains(Frizz::FrizzConfig::BASE_TEMPLATES_PATH_KEY)) {
        this->source_root_path = json_config[Frizz::FrizzConfig::BASE_TEMPLATES_PATH_KEY];
    }

    if(json_config.contains(Frizz::FrizzConfig::BUILD_PATH_KEY)) {
        this->build_path = json_config[Frizz::FrizzConfig::BUILD_PATH_KEY];
    }
}

std::string Frizz::FrizzConfig::get_source_root_path() {
    return this->source_root_path;
}

std::string Frizz::FrizzConfig::get_build_path() {
    return this->build_path;
}

std::filesystem::path Frizz::FrizzConfig::get_path_to_input_file(std::string filename) {
    std::filesystem::path path(this->get_source_root_path());
    path /= filename;

    return path;
}

std::filesystem::path Frizz::FrizzConfig::get_path_to_output_file(std::string filename) {
    std::filesystem::path path(this->get_build_path());
    path /= filename;

    return path;
}