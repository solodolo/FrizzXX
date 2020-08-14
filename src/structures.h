/*
 * structures.h
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_STRUCTURES_H_
#define SRC_STRUCTURES_H_

#include <string>

namespace Frizz {

    class BasicStruct {
        public:
            std::string evaluate(std::string root_path);
    };

    class AssignmentStruct : public BasicStruct {
    public:
        AssignmentStruct(std::string name, std::string value): name(name), value(value) {};
        std::string evaluate(std::string root_path);

    private:
        bool is_src();

    private:
        std::string name;
        std::string value;
    };

}

#endif /* SRC_STRUCTURES_H_ */
