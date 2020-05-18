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


public:

    virtual const map<std::string, TypeName> Fields() const = 0;
    inline const map<std::string, ElementValue> Values() const { return _values; }
    inline const void insert(pair<string, ElementValue> src) { _values.insert(src); }
    inline const void clear() { _values.clear(); }

    ElementValue& operator[] (std::string const &);
    const ElementValue operator[](std::string const &) const;
    Model& operator=(const Model&);
    virtual Model* clone() = 0;

};

class ModelModel: public Model {
public:
    ModelModel() {}
    ModelModel(const ModelModel& src): Model(src) {}

    const map<std::string, TypeName> Fields() const;
    virtual Model* clone() { return new ModelModel(*this); }

};

class CarModel: public Model {
public:
    CarModel() {}
    CarModel(const CarModel& src):Model(src) {}

    virtual const map<std::string, TypeName> Fields() const;
    virtual Model* clone() { return new CarModel(*this); }

};

class ManagerModel: public Model {
public:
    ManagerModel() {}
    ManagerModel(const ManagerModel& src):Model(src) {}

    virtual const map<std::string, TypeName> Fields() const;
    virtual Model* clone() { return new ManagerModel(*this); }

};

#endif //FINALPROJECTCPP_MODEL_H
