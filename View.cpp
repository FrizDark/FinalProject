//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#include "View.h"

ViewModel::ViewModel(BaseTable* ltable, std::string& lfield, BaseTable* rtable, std::string& rfield) {
    join(ltable, lfield, rtable, rfield);
}

void ViewModel::join(BaseTable* ltable, std::string& lfield, BaseTable* rtable, std::string& rfield) {
JoinFields jf = {ltable, lfield, rtable, rfield};
_joins.push_back(jf);
}

void ViewModel::joinFields(list<JoinFields> joins) {
    for (auto &i : joins) {
        for (auto &j : i.leftTable->elements()) {
            for (auto &h : j->Fields()) {
                m_fields.insert(make_pair(h.first, h.second));
            }
            for (auto &h : j->Values()) {
                m_values.insert(make_pair(h.first, h.second));
            }
        }
        for (auto &j : i.rightTable->elements()) {
            for (auto &h : j->Fields()) {
                if (h.first == i.rightField) {
                    m_fields.insert(make_pair(i.leftField, h.second));
                }
                m_fields.insert(make_pair(h.first, h.second));
            }
            for (auto &h : j->Values()) {
                if (h.first == i.rightField) {
                    m_values.insert(make_pair(i.leftField, h.second));
                }
                m_values.insert(make_pair(h.first, h.second));
            }
        }
    }
}

vector<pair<std::string, ElementValue>>
View::find(std::function<bool(pair<std::string, ElementValue> &)> filter) {
    vector<pair<std::string, ElementValue>> output;
    for (auto &m : Values()) {
        if (filter && !filter(m)) {
            continue;
        }
        output.push_back(m);
    }
    return output;
}

vector<pair<std::string, TypeName>> View::Fields() {
    vector<pair<std::string, TypeName>> f;
    for (auto &i : Fields()) {
        f.push_back(make_pair(i.first, i.second));
    }
    return f;
}

vector<pair<std::string, ElementValue>> View::Values() {
    vector<pair<std::string, ElementValue>> v;
    for (auto &i : m_values) {
        v.push_back(make_pair(i.first, i.second));
    }
    return v;
}

void View::print() {
    int buf;
    int space = 0;
    for (auto &t : Fields()) {
        if (t.second.Description == "ID") continue;
        switch (m_values[t.first].type) {
            case tnumber:
                cout << "| " << m_values[t.first].value.tnumber;
                buf = m_values[t.first].value.tnumber;
                for (; buf > 0; space++) {
                    buf /= 10;
                }
                break;
            case tboolean:
                if (m_values[t.first].value.tboolean) {
                    cout << "| +";
                } else {
                    cout << "| -";
                }
                space = 1;
                break;
            case tstring:
                cout << "| " << *m_values[t.first].value.tstring;
                space = m_values[t.first].value.tstring->length();
                break;
            case tarray:
                for (auto &i : *m_values[t.first].value.tarray) {
                    cout << *i.value.tstring << endl;
                }
                break;
            case tobject:
                cout << endl;
//                print(*m_values[t.first].value.tobject);
                break;
            case empty:
                break;
        }
        for (int i = 0; i < 20 - space; i++) cout << " ";
        space = 0;
    }
    cout << "|" << endl;
}

ElementValue View::operator[](std::string src) {
    return m_values[src];
}
