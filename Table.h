//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#ifndef FINALPROJECTCPP_TABLE_H
#define FINALPROJECTCPP_TABLE_H

#include "IncludeLib.h"
#include "Model.h"

template<class T>
class Table {
private:
    std::string _name;
    list<T> _elements;

protected:
    void print(const Model& m) {
        int buf;
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

    void loadObject(T &m, pt::ptree root) {
        T m1;
        string objectName;
        for (auto &i : m.Fields()) {
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
                    m1.insert(std::make_pair(j.first, j.second.data()));
                }
            }
        }
        m[objectName] = m1;
    }
    void loadArray(T &m, pt::ptree root) {
        T m1;
        vector<ElementValue> v;
        string arrayName;
        for (auto &i : m.Fields()) {
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
                v.emplace_back(m1[i.first].value.tarray);
            } else {
                v.emplace_back(i.second.data());
            }
        }
        m[arrayName] = v;
    }

    pt::ptree saver(const Model& i) {
        pt::ptree rootObject, rootObjectElements, rOEE;
        for (auto &j : i.Values()) {
            switch (j.second.type) {
                case empty:
                    rootObjectElements.put(j.first, NULL);
                    break;
                case tnumber:
                    rootObjectElements.put(j.first, i[j.first].value.tnumber);
                    break;
                case tboolean:
                    rootObjectElements.put(j.first, i[j.first].value.tboolean);
                    break;
                case tstring:
                    rootObjectElements.put(j.first, *i[j.first].value.tstring);
                    break;
                case tarray:
                    for (auto a = i[j.first].value.tarray->cbegin(); a != i[j.first].value.tarray->cend(); a++) {
                        rOEE.push_back(std::make_pair("", saver(*a, i.Fields())));
                    }
                    rootObjectElements.add_child(j.first, rOEE);
                    rOEE.clear();
                    break;
                case tobject:
                    rootObjectElements.add_child(j.first, saver(*i[j.first].value.tobject));
                    break;
            }
        }
        rootObject.push_back(std::make_pair("", rootObjectElements));
        rootObjectElements.clear();
        return rootObject;
    }

    pt::ptree saver() {
        pt::ptree rootObject, rootObjectElements, rOEE;
        for (auto &i : _elements) {
            for (auto &j : i.Values()) {
                switch (j.second.type) {
                    case empty:
                        rootObjectElements.put(j.first, NULL);
                        break;
                    case tnumber:
                        rootObjectElements.put(j.first, i[j.first].value.tnumber);
                        break;
                    case tboolean:
                        rootObjectElements.put(j.first, i[j.first].value.tboolean);
                        break;
                    case tstring:
                        rootObjectElements.put(j.first, *i[j.first].value.tstring);
                        break;
                    case tarray:
                        for (auto a = i[j.first].value.tarray->cbegin(); a != i[j.first].value.tarray->cend(); a++) {
                            rOEE.push_back(std::make_pair("", saver(*a, i.Fields())));
                        }
                        rootObjectElements.add_child(j.first, rOEE);
                        rOEE.clear();
                        break;
                    case tobject:
                        rootObjectElements.add_child(j.first, saver(*i[j.first].value.tobject));
                        break;
                }
            }
            rootObject.push_back(std::make_pair("", rootObjectElements));
            rootObjectElements.clear();
        }
        return rootObject;
    }
    pt::ptree saver(ElementValue i, const map<std::string, TypeName> b) {
        pt::ptree rootObject, rootObjectElements, rOEE;
        switch (i.type) {
            case empty:
                rootObjectElements.put("", NULL);
                break;
            case tnumber:
                rootObjectElements.put("", i.value.tnumber);
                break;
            case tboolean:
                rootObjectElements.put("", i.value.tboolean);
                break;
            case tstring:
                rootObjectElements.put("", *i.value.tstring);
                break;
            case tarray:
                for (auto a = i.value.tarray->cbegin(); a != i.value.tarray->cend(); a++) {
                    rOEE.push_back(std::make_pair("", saver(*a, b)));
                }
                rootObjectElements.add_child("", rOEE);
                break;
            case tobject:
                rootObjectElements.add_child("", saver(*i.value.tobject));
                break;
        }
        return rootObjectElements;
    }

public:
    Table(const std::string& name):_name(name) {}

    inline const std::string name() { return _name; }

    void add(T m) {
        _elements.push_back(m);
    }
    void print(vector<T> *vv = NULL) {
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
        if (vv) {
            for (auto &m : *vv) {
                print(m);
            }
        } else {
            for (auto &m : _elements) {
                print(m);
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
    void load() {
        pt::ptree root;
        string fileName = "../" + name() + "Data.json";
        pt::read_json(fileName, root);
        T m;

        string arrayName = "Array";
        for (auto &i : m.Fields()) {
            if (i.second.Type == tarray) {
                arrayName = i.first;
                break;
            }
        }

        string objectName = "Object";
        for (auto &i : m.Fields()) {
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
                    m[a.first] = a.second.data();
                }
            }
            _elements.push_back(m);
            m.clear();
        }
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Успех                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    }
    void save() {
        pt::ptree root;
        root.add_child(name(), saver());
        /*TODO: Save in one file:
         * string fileName = "../" + name() + ".json";
         * pt::write_json(fileName, root);
        */
        pt::write_json("../TestJSON.json", root);
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Успех                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    }
    void remove(T m) {
        int pos = 0;
        bool IsHere = false;
        int ihNum = 0;
        for (auto &i : _elements) {
            for (auto j : i.Fields()) {
                if (j.second.Description == "ID") continue;
                if (*m[j.first].value.tstring == *i[j.first].value.tstring) {
                    ihNum++;
                }
            }
            if (ihNum == i.Fields().size() - 1) {
                IsHere = true;
                break;
            } else {
                ihNum = 0;
            }
            pos++;
        }
        if (IsHere) {
            auto delPos = _elements.begin();
            advance(delPos, pos);
            _elements.erase(delPos);
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            cout << "|                              Успех                              |" << endl;
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        } else {
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            cout << "|                           Не найдено                            |" << endl;
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
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
            for (auto &m : _elements) {
                if (filter && !filter(m)) {
                    continue;
                }
                output.push_back(m);
            }
        }
        return output;
    }
    void update(T m) {
        int pos = 0;
        bool IsHere = false;
        for (auto &i : _elements) {
            if (*m["ID"].value.tstring == *i["ID"].value.tstring) {
                IsHere = true;
                break;
            }
            pos++;
        }
        if (IsHere) {
            int pos2 = 0;
            for (auto &i : _elements) {
                if (pos2 == pos) {
                    i = m;
                    break;
                }
                pos2++;
            }
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            cout << "|                              Успех                              |" << endl;
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        } else {
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            cout << "|                           Не найдено                            |" << endl;
            cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
};





class CarModelTable: public Table<CarModelModel> {
public:
    CarModelTable():Table<CarModelModel>("Models") {}
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
