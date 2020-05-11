//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#ifndef FINALPROJECTCPP_MODELSTRUCT_H
#define FINALPROJECTCPP_MODELSTRUCT_H

#include "IncludeLib.h"

enum ElementType {
    empty,
    tnumber,
    tboolean,
    tstring,
    tarray,
    tobject
};

struct TypeName {
    ElementType Type;
    string Description;
};

struct ModelStruct;

struct ElementValue {
    ElementType type;
    union {
        double tnumber;
        bool tboolean;
        std::string* tstring;
        std::vector<ElementValue>* tarray;
        ModelStruct* tobject;
    } value;

    ElementValue() {
        type = empty;
    }
    ElementValue(int i);
    ElementValue(double i);
    ElementValue(bool i);
    ElementValue(std::string const &i);
    ElementValue(std::vector<ElementValue> const &i);
    ElementValue(ModelStruct const &i);

    ElementValue(const ElementValue &);
    ElementValue& operator=(const ElementValue&);

    ~ElementValue();
};

struct ModelStruct {
    map<std::string, TypeName> Fields;
    map<std::string, ElementValue> Values;

    string ID();
    string mark();
    string model();
    string type();
    std::vector<ElementValue> array();
    ModelStruct object();

    ElementValue& operator[] (std::string const &);

    ModelStruct();

    ModelStruct(string Mark);

    ModelStruct(string Id, string Mark);

    ModelStruct(const ModelStruct &);
    ModelStruct& operator=(const ModelStruct&);

};

#endif //FINALPROJECTCPP_MODELSTRUCT_H
