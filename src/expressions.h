/*
 * expressions.h
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_EXPRESSIONS_H_
#define SRC_EXPRESSIONS_H_

#include <string>

namespace Frizz {

    class BasicExp {
        public:
            std::string evaluate(std::string root_path);
    };

    class AssignmentExp : public BasicExp {
    public:
        AssignmentExp(std::string name, std::string value): name(name), value(value) {};
        std::string evaluate(std::string root_path);

    private:
        bool is_src();

    private:
        std::string name;
        std::string value;
    };

}

#endif /* SRC_EXPRESSIONS_H_ */
