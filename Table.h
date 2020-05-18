//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#ifndef FINALPROJECTCPP_TABLE_H
#define FINALPROJECTCPP_TABLE_H

#include "BaseTable.h"

template<class T>
class Table: public BaseTable {
protected:
    void loadObject(T *m, pt::ptree root) {
        T *m1 = new T();
        string objectName;
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tobject) {
                objectName = i.first;
                break;
            }
        }
        if (objectName.empty()) {
            return;
        }
        for(auto &i : root.get_child(objectName)) {
            for (auto &j : i.second) {
                if (i.first == objectName) {
                    loadObject(m1, j.second);
                } else {
                    (*m1).insert(std::make_pair(j.first, j.second.data()));
                }
            }
        }
        (*m)[objectName] = m1;
    }
    void loadArray(T *m, pt::ptree root) {
        T *m1 = new T();
        vector<ElementValue> v;
        string arrayName;
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tarray) {
                arrayName = i.first;
                break;
            }
        }
        if (arrayName.empty()) {
            return;
        }
        for(auto &i : root.get_child(arrayName)) {
            if (i.first == arrayName) {
                loadArray(m1, i.second);
                v.emplace_back((*m1)[i.first].value.tarray);
            } else {
                v.emplace_back(i.second.data());
            }
        }
        (*m)[arrayName] = v;
    }

public:
    Table(const std::string& name):BaseTable(name) {}

    inline const std::string name() { return m_name; }

    void add(T& m) {
        m_elements.push_back(&m);
    }
    void print(const Model& m) {
        double buf;
        int space = 0;
        for (auto &t : m.Fields()) {
            if (t.second.Description == "ID") continue;
            switch (m[t.first].type) {
                case tnumber:
                    cout << "| " << m[t.first].value.tnumber;
                    buf = m[t.first].value.tnumber;
                    for (; buf > 0; space++) {
                        buf /= 10;
                    }
                    break;
                case tboolean:
                    if (m[t.first].value.tboolean) {
                        cout << "| +";
                    } else {
                        cout << "| -";
                    }
                    space = 1;
                    break;
                case tstring:
                    cout << "| " << *m[t.first].value.tstring;
                    space = m[t.first].value.tstring->length();
                    break;
                case tarray:
                    for (auto &i : *m[t.first].value.tarray) {
                        cout << *i.value.tstring << endl;
                    }
                    break;
                case tobject:
                    cout << endl;
                    print(*m[t.first].value.tobject);
                    break;
                case empty:
                    break;
            }
            for (int i = 0; i < 20 - space; i++) cout << " ";
            space = 0;
        }
        cout << "|" << endl;
    }
    void print(vector<T> *vv = NULL) {
        int length;
        T mm;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            length += 22;
        }
        length++;
        for (int i = 0; i < length; i++) cout << "–";
        cout << endl;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            cout << "| " << i.second.Description;
            int j = i.second.Description.length() / 2;
            for (int a = 0; a < 20 - j; a++) cout << " ";
        }
        cout << "|" << endl;
        for (int i = 0; i < length; i++) cout << "–";
        cout << endl;
        if (vv) {
            for (auto &m : *vv) {
                print(m);
            }
        } else {
            for (auto &m : m_elements) {
                print(*m);
            }
        }
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
    }
    void printM(const Model& m) {
        int length;
        T mm;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            length += 22;
        }
        length++;
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            cout << "| " << i.second.Description;
            int j = i.second.Description.length() / 2;
            for (int a = 0; a < 20 - j; a++) cout << " ";
        }
        cout << "|" << endl;
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
        print(m);
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
    }
    bool load() {
        pt::ptree root;
        string fileName = "../" + name() + "Data.json";
        pt::read_json(fileName, root);
        T *m = new T();
        T mm;

        string arrayName = "Array";
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tarray) {
                arrayName = i.first;
                break;
            }
        }

        string objectName = "Object";
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tobject) {
                objectName = i.first;
                break;
            }
        }

        for(auto &i : root.get_child(name())) {
            for(auto &a : i.second) {
                if (a.first == arrayName) {
                    loadArray(m, i.second);
                } else if (a.first == objectName) {
                    loadObject(m, i.second);
                } else {
                    (*m)[a.first] = a.second.data();
                }
            }
            m_elements.push_back(new T(*m));
            (*m).clear();
        }
        return true;
    }
    bool remove(T m) {
        int pos = 0;
        bool IsHere = false;
        int ihNum = 0;
        for (auto &i : m_elements) {
            for (auto j : i->Fields()) {
                if (j.second.Description == "ID") continue;
                if (*m[j.first].value.tstring == *(*i)[j.first].value.tstring) {
                    ihNum++;
                }
            }
            if (ihNum == i->Fields().size() - 1) {
                IsHere = true;
                break;
            } else {
                ihNum = 0;
            }
            pos++;
        }
        if (IsHere) {
            auto delPos = m_elements.begin();
            advance(delPos, pos);
            m_elements.erase(delPos);
            return true;
        }
        return false;
    }
    vector<T> find(std::function<bool(const T&)> filter = NULL, vector<T> *vv = NULL) {
        vector<T> output;
        if (vv) {
            for (auto &m : *vv) {
                if (filter && !filter(m)) {
                    continue;
                }
                output.push_back(m);
            }
        } else {
            for (auto &m : m_elements) {
                if (filter && !filter(*((T*)m))) {
                    continue;
                }
                output.push_back(*((T*)m));
            }
        }
        return output;
    }
    bool update(T m) {
        int pos = 0;
        bool IsHere = false;
        for (auto &i : m_elements) {
            if (*m["ID"].value.tstring == *(*i)["ID"].value.tstring) {
                IsHere = true;
                break;
            }
            pos++;
        }
        if (IsHere) {
            int pos2 = 0;
            for (auto &i : m_elements) {
                if (pos2 == pos) {
                    i = new T(m);
                    break;
                }
                pos2++;
            }
            return true;
        }
        return false;
    }
};





class ModelTable: public Table<ModelModel> {
public:
    ModelTable(): Table<ModelModel>("Models") {}
};

class CarTable: public Table<CarModel> {
public:
    CarTable():Table<CarModel>("Cars") {}
};

class ManagerTable: public Table<ManagerModel> {
public:
    ManagerTable():Table<ManagerModel>("Managers") {}
};

#endif //FINALPROJECTCPP_TABLE_H
