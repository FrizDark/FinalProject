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

    void join(BaseTable* ltable, std::string& lfield, BaseTable* rtable, std::string& rfield);

public:

    ViewModel(BaseTable* ltable, std::string& lfield, BaseTable* rtable, std::string& rfield);

    void joinFields(list<JoinFields> joins);
};

class View: public ViewModel {
public:
    View(BaseTable* ltable, std::string& lfield, BaseTable* rtable, std::string& rfield): ViewModel(ltable, lfield, rtable, rfield) {}

    vector<pair<std::string, TypeName>> Fields();
    vector<pair<std::string, ElementValue>> Values();

    virtual void print();
    vector<pair<std::string, ElementValue>> find(std::function<bool(pair<std::string, ElementValue>&)> filter = NULL);
    ElementValue operator[] (std::string);
};


#endif //FINALPROJECTCPP_VIEW_H
