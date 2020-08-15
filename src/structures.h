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
            std::string evaluate(std::string root_path) { return this->do_evaluate(root_path); };
            virtual ~BasicStruct() {};

        private:
            virtual std::string do_evaluate(std::string root_path) = 0;
    };

    class AssignmentStruct : public BasicStruct {
        public:
            AssignmentStruct(std::string name, std::string value): name(name), value(value) {};
  

        private:
            bool is_src();
            std::string do_evaluate(std::string root_path) override;

        private:
            std::string name;
            std::string value;
    };

    class PassthroughStruct : public BasicStruct {
        public:
            PassthroughStruct(std::string value) : value(value) {};

        private:
            std::string value;
            std::string do_evaluate(std::string root_path) override;
    };

}

#endif /* SRC_STRUCTURES_H_ */
