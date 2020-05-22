//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#include "View.h"

ViewModel::ViewModel(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield) {
    join(ltable, lfield, rtable, rfield);
}

void ViewModel::join(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield) {
    JoinFields jf = {&ltable, lfield, &rtable, rfield};
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
    for (auto &i : _joins) {
        for (auto &j : i.leftTable->elements()) {
            for (auto &h : j->Fields()) {
                f.push_back(make_pair(h.first, h.second));
            }
            break;
        }
        for (auto &j : i.rightTable->elements()) {
            for (auto &h : j->Fields()) {
                if (h.first == i.rightField) {
                    f.push_back(make_pair(i.leftField, h.second));
                    continue;
                }
                f.push_back(make_pair(h.first, h.second));
            }
            break;
        }
    }
    return f;
}

vector<pair<std::string, ElementValue>> View::Values() {
    vector<pair<std::string, ElementValue>> v;
    for (const auto &i : _joins) {
        vector<pair<std::string, ElementValue>> left;
        vector<pair<std::string, ElementValue>> right;

        for (auto &i : _joins) {
            for (auto &j : i.leftTable->elements()) {
                for (auto &h : j->Values()) {
                    left.push_back(make_pair(h.first, h.second));
                }
            }
            for (auto &j : i.rightTable->elements()) {
                for (auto &h : j->Values()) {
                    if (h.first == i.rightField) {
                        right.push_back(make_pair(i.leftField, h.second));
                        continue;
                    }
                    right.push_back(make_pair(h.first, h.second));
                }
            }
        }

        boost::char_separator<char> sep{"."};
        bool isFind = false;
        int idPos = 0;
        vector <std::string> id;
        for (const auto &j : i.leftTable->elements()) {
            for (const auto &h : j->Values()) {
                if (h.first == i.leftField) id.push_back(*h.second.value.tstring);
            }
        }
        while (!left.empty() && !right.empty()) {
            for (const auto &field : m_printFields) {
                tokenizer tok{field, sep};
                bool leftRight;
                for (const auto &t : tok) {
                    if (t == "left") {
                        leftRight = true;
                    } else if (t == "right") {
                        leftRight = false;
                    } else {
                        if (leftRight) {
                            for (auto j = left.begin(); j != left.end(); j++) {
                                if (j->first == t) {
                                    v.push_back(*j);
                                    left.erase(j);
                                    isFind = true;
                                    break;
                                }
                            }
                        } else {
                            for (auto &j : i.rightTable->elements()) {
                                for (auto &h : j->Values()) {
                                    if (*h.second.value.tstring == id[idPos]) {
                                        isFind = true;
                                        break;
                                    }
                                }
                                if (isFind) {
                                    for (auto &h : j->Values()) {
                                        if (h.first == t) {
                                            v.push_back(h);
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        if (!isFind) return v;
                        else isFind = false;
                    }
                }
            }
            idPos++;
        }
    }
    return v;
}

void View::print(pair<std::string, ElementValue> t) {
    int buf;
    int space = 0;
    switch (t.second.type) {
        case tnumber:
            cout << "| " << t.second.value.tnumber;
            buf = t.second.value.tnumber;
            for (; buf > 0; space++) {
                buf /= 10;
            }
            break;
        case tboolean:
            if (t.second.value.tboolean) {
                cout << "| +";
            } else {
                cout << "| -";
            }
            space = 1;
            break;
        case tstring:
            cout << "| " << *t.second.value.tstring;
            space = t.second.value.tstring->length();
            break;
        case tarray:
            for (auto &a : *t.second.value.tarray) {
                cout << *a.value.tstring << endl;
            }
            break;
        case tobject:
            cout << endl;
//                print(t.second.value.tobject);
            break;
        case empty:
            break;
    }
    for (int i = 0; i < 20 - space; i++) cout << " ";
    space = 0;
}

void View::print() {
    int length;
    for (auto &i : Fields()) {
        if (i.second.Description == "ID") continue;
        length += 22;
    }
    length++;
    for (int i = 0; i < length; i++) cout << "–";
    cout << endl;
    for (auto &i : Fields()) {
        if (i.second.Description == "ID") continue;
        cout << "| " << i.second.Description;
        int j = i.second.Description.length() / 2;
        for (int a = 0; a < 20 - j; a++) cout << " ";
    }
    cout << "|" << endl;
    for (int i = 0; i < length; i++) cout << "–";
    cout << endl;

    int fs = 0;
    auto f = Fields();
    for (auto &t : Values()) {
        if (fs == m_printFields.size()) {
            fs = 0;
            cout << "|" << endl;
        }
        if (t.first.find("ID") != std::string::npos) {
            fs++;
            continue;
        }
        print(t);
        fs++;
    }
    cout << "|" << endl;
    for (int i = 0; i < length; i++) cout << "–";
    cout << endl;
}

//ElementValue View::operator[](std::string src) {
//    return m_values[src];
//}

ElementValue View::get(const std::string& src) {
    for (auto &v: Values()) {
        if (v.first == src) return v.second;
    }
}
