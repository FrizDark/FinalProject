//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#ifndef FINALPROJECTCPP_MODEL_H
#define FINALPROJECTCPP_MODEL_H

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

struct Model;

struct ElementValue {
    ElementType type;
    union {
        double tnumber;
        bool tboolean;
        std::string* tstring;
        std::vector<ElementValue>* tarray;
        Model* tobject;
    } value;

    ElementValue() {
        type = empty;
    }
    ElementValue(int i);
    ElementValue(double i);
    ElementValue(bool i);
    ElementValue(std::string const &i);
    ElementValue(std::vector<ElementValue> const &i);
    ElementValue(Model &i);

    ElementValue(const ElementValue &);
    ElementValue& operator=(const ElementValue&);

    ~ElementValue();
};

class Model {

private:
    map<std::string, ElementValue> _values;

protected:
    Model() {
    }
    Model(const Model& src) {
        _values = map<std::string, ElementValue>(src._values);
    }

    virtual map<std::string, TypeName> fields() const = 0;

public:

    inline const map<std::string, TypeName> Fields() const { return fields(); }
    inline const map<std::string, ElementValue> Values() const { return _values; }
    inline const void insert(pair<string, ElementValue> src) { _values.insert(src); }
    inline const void clear() { _values.clear(); }

    ElementValue& operator[] (std::string const &);
    const ElementValue operator[](std::string const &) const;
    Model& operator=(const Model&);
    virtual Model* clone() = 0;

};

class CarModelModel: public Model {
protected:
    virtual map<std::string, TypeName> fields() const {
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

public:
    CarModelModel() {}
    CarModelModel(const CarModelModel& src):Model(src) {}

    virtual Model* clone() { return new CarModelModel(*this); }

};

#endif //FINALPROJECTCPP_MODEL_H
