//
// Created by Dmitriy Steshenko on 01.05.2020.
//

#include "ModelStruct.h"

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

ElementValue::ElementValue(ModelStruct const &i) {
    type = tobject;
    value.tobject = new ModelStruct(i);
}

ElementValue::~ElementValue() {
    switch (type) {
        case tstring:
            delete value.tstring;
            break;
        case tarray:
            delete value.tarray;
            break;
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
            value.tobject = src.value.tobject;
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
            value.tobject = new ModelStruct(*src.value.tobject);
            break;
    }
    return *this;
}

ModelStruct::ModelStruct() {
    Fields.insert(make_pair(make_pair("ID", "ID"), tstring));
    Fields.insert(make_pair(make_pair("Mark", "Марка"), tstring));
    Fields.insert(make_pair(make_pair("Model", "Модель"), tstring));
    Fields.insert(make_pair(make_pair("Type", "Тип"), tstring));
}

ModelStruct::ModelStruct(string Mark) {
    ModelStruct();
    random_generator gen;
    Values.insert(std::make_pair("ID", to_string(gen())));
    Values.insert(std::make_pair("Mark", Mark));
}

ModelStruct::ModelStruct(string Id, string Mark) {
    ModelStruct();
    Values.insert(std::make_pair("ID", Id));
    Values.insert(std::make_pair("Mark", Mark));
}

string ModelStruct::ID() {
    return *(Values["ID"].value.tstring);
}

string ModelStruct::mark()  {
    return *(Values["Mark"].value.tstring);
}

string ModelStruct::model() {
    return *(Values["Model"].value.tstring);
}

string ModelStruct::type() {
    return *(Values["Type"].value.tstring);
}

ModelStruct::ModelStruct(const ModelStruct &src) {
    Fields = map<pair<string, string>, ElementType>(src.Fields);
    Values = map<std::string, ElementValue>(src.Values);
}

ModelStruct &ModelStruct::operator=(const ModelStruct &src) {
    Fields = map<pair<string, string>, ElementType>(src.Fields);
    Values = map<std::string, ElementValue>(src.Values);
    return *this;
}

ElementValue &ModelStruct::operator[](const string &idx) {
    return Values[idx];
}

std::vector<ElementValue> ModelStruct::array() {
    return *(Values["Array"].value.tarray);
}

ModelStruct ModelStruct::object() {
    return *(Values["Object"].value.tobject);
}
