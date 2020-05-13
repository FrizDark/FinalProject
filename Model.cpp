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

map<std::string, TypeName> CarModelModel::fields() const {
    map<std::string, TypeName> f;
    TypeName tn = {tstring, "ID"};
    f.insert(make_pair("ID", tn));
    tn = {tstring, "Марка"};
    f.insert(make_pair("Mark", tn));
    tn = {tstring, "Модель"};
    f.insert(make_pair("Model", tn));
    tn = {tstring, "Тип"};
    f.insert(make_pair("Type", tn));
    return f;
}

map<std::string, TypeName> CarModel::fields() const {
    map<std::string, TypeName> f;
    TypeName tn = {tstring, "ID"};
    f.insert(make_pair("ID", tn));
    tn = {tstring, "ID"};
    f.insert(make_pair("MarkID", tn));
    tn = {tstring, "Марка"};
    f.insert(make_pair("Mark", tn));
    tn = {tstring, "Цвет"};
    f.insert(make_pair("Color", tn));
    tn = {tnumber, "Цена"};
    f.insert(make_pair("Price", tn));
    return f;
}

map<std::string, TypeName> ManagerModel::fields() const {
    map<std::string, TypeName> f;
    TypeName tn = {tstring, "ID"};
    f.insert(make_pair("ID", tn));
    tn = {tstring, "Имя"};
    f.insert(make_pair("FirstName", tn));
    tn = {tstring, "Фамилия"};
    f.insert(make_pair("LastName", tn));
    tn = {tnumber, "Возраст"};
    f.insert(make_pair("Age", tn));
    tn = {tstring, "Город"};
    f.insert(make_pair("City", tn));
    tn = {tnumber, "Номер_телефона"};
    f.insert(make_pair("MobileNumber", tn));
    return f;
}
