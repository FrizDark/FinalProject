//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#ifndef FINALPROJECTCPP_VIEW_H
#define FINALPROJECTCPP_VIEW_H

#include "Table.h"

class ViewModel {
protected:
    struct JoinFields {
        BaseTable *leftTable;
        std::string leftField;
        BaseTable *rightTable;
        std::string rightField;
    };

    list<JoinFields> _joins;

    map<std::string, TypeName> m_fields;
    map<std::string, ElementValue> m_values;
    vector<std::string> m_printFields;

    void join(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield);

public:

    ViewModel(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield);

    void joinFields(list<JoinFields> joins);
};

class View: public ViewModel {
private:
    virtual void print(pair<std::string, ElementValue> t);

public:
    View(BaseTable& ltable, const std::string lfield, BaseTable& rtable, const std::string rfield): ViewModel(ltable, lfield, rtable, rfield) {}

    vector<pair<std::string, TypeName>> Fields();

    vector<pair<std::string, ElementValue>> Values();

    ElementValue get(const std::string&);

    virtual void print();
    vector<pair<std::string, ElementValue>> find(std::function<bool(pair<std::string, ElementValue>&)> filter = NULL);
//    ElementValue operator[] (std::string);
};

class CarModelView: public View {
public:
    CarModelView():View(CarTable::instance(), "Model_ID", ModelTable::instance(), "ID") {
        m_printFields.push_back("left.Color");
        m_printFields.push_back("left.Mark");
        m_printFields.push_back("left.Price");
        m_printFields.push_back("right.Mark");
        m_printFields.push_back("right.Model");
        m_printFields.push_back("right.Type");
    }
};

#endif //FINALPROJECTCPP_VIEW_H
