//
// Created by Dmitriy Steshenko on 01.05.2020.
//

#include "Model.h"

ElementValue::ElementValue(int i) {
    type = tnumber;
    value.tnumber = i;
}

ElementValue::ElementValue(double i) {
    type = tnumber;
    value.tnumber = i;
}

ElementValue::ElementValue(bool i) {
    type = tboolean;
    value.tboolean = i;
}

ElementValue::ElementValue(std::string const &i) {
    type = tstring;
    value.tstring = new std::string(i);
}

ElementValue::ElementValue(std::vector<ElementValue> const &i) {
    type = tarray;
    value.tarray = new vector<ElementValue>(i);
}

ElementValue::ElementValue(Model &i) {
    type = tobject;
    value.tobject = i.clone();
}

ElementValue::~ElementValue() {
    switch (type) {
        case tstring:
            delete value.tstring;
            break;
        case tarray:
            delete value.tarray;
            break;
            //TODO: Fix this
//        case tobject:
//            delete value.tobject;
//            break;
        default:
            break;
    }
}

ElementValue::ElementValue(const ElementValue &src) {
    type = src.type;
    switch (type) {
        case tnumber:
            value.tnumber = src.value.tnumber;
            break;
        case tboolean:
            value.tboolean = src.value.tboolean;
            break;
        case tstring:
            value.tstring = new std::string(*src.value.tstring);
            break;
        case tarray:
            value.tarray = new std::vector<ElementValue>(*src.value.tarray);
            break;
        case tobject:
            value.tobject = src.value.tobject->clone();
            break;
        default:
            break;
    }
}

ElementValue &ElementValue::operator=(const ElementValue &src) {
    type = src.type;
    switch (type) {
        case tnumber:
            value.tnumber = src.value.tnumber;
            break;
        case tboolean:
            value.tboolean = src.value.tboolean;
            break;
        case tstring:
            value.tstring = new std::string(*src.value.tstring);
            break;
        case tarray:
            value.tarray = new std::vector<ElementValue>(*src.value.tarray);
            break;
        case tobject:
            value.tobject = src.value.tobject->clone();
            break;
        default:
            break;
    }
    return *this;
}

Model &Model::operator=(const Model &src) {
    _values = map<std::string, ElementValue>(src._values);
    return *this;
}

ElementValue &Model::operator[](const string &name) {
    //TODO: Check name in the fields
    auto value = _values.find(name);
    if (value == _values.end()) {
        _values[name] = new ElementValue();
    }
    return _values[name];
}

const ElementValue Model::operator[] (const string &name) const {
    return _values.at(name);
}