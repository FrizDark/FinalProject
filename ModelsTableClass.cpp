//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#include "ModelsTableClass.h"


void ModelsTableClass::add(ModelStruct m) {
//    ModelStruct m1;
//
//    m1["ID"] = (string)"4dedb3cd-21e5-4032-aac7-a28bedd86659";
//    m1["Mark"] = (string)"BMW";
//    m1["Model"] = (string)"S9";
//    m1["Type"] = (string)"Jeep";
//
//    m["Object"].type = tobject;
//    m["Object"].value.tobject = new ModelStruct;
//    m["Object"] = m1;

    Models.push_back(m);
}

void ModelsTableClass::print(vector<ModelStruct> *vv) {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
//    cout << "| Марка               | Модель              | Тип                 |" << endl;
    ModelStruct mm;
    for (auto &i : mm.Fields) {
        if (i.first.first == "ID") continue;
        cout << "| " << i.first.second;
        int j = i.first.second.length()/2;
        for (int a = 0; a < 20 - j; a++) cout << " ";
    }
    cout << "|" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    if (vv) {
        for (auto &m : *vv) {
            print(m);
        }
    } else {
        for (auto &m : Models) {
            print(m);
        }
    }
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}

void ModelsTableClass::print(ModelStruct m) {
    if (m.Values["Mark"].type != empty) cout << "| " << m.mark();
    for (int i = 0; i < 20 - m.mark().length(); i++) cout << " ";
    if (m.Values["Model"].type != empty) cout << "| " << m.model();
    for (int i = 0; i < 20 - m.model().length(); i++) cout << " ";
    if (m.Values["Type"].type != empty) cout << "| " << m.type();
    for (int i = 0; i < 20 - m.type().length(); i++) cout << " ";
    cout << "|" << endl;
    if (m.Values["Array"].type != empty) for (auto &i : m.array()) {
        cout << *i.value.tstring << endl;
    }
    if (m.Values["Object"].type != empty) {
        cout << endl;
        print(m.object());
    }
}

void ModelsTableClass::loadObject(ModelStruct &m, pt::ptree root) {
    ModelStruct m1;
    for(auto &i : root.get_child("Object")) {
        for (auto &j : i.second) {
            if (i.first == "Object") {
                loadObject(m1, j.second);
            } else {
                m1.Values.insert(std::make_pair(j.first, j.second.data()));
            }
        }
    }
    m["Object"] = m1;
}

void ModelsTableClass::loadArray(ModelStruct &m, pt::ptree root) {
    ModelStruct m1;
    vector<ElementValue> v;
    for(auto &i : root.get_child("Array")) {
        if (i.first == "Array") {
            loadArray(m1, i.second);
            v.emplace_back(m1.array());
        } else {
            v.emplace_back(i.second.data());
        }
    }
    m.Values["Array"] = v;
}

void ModelsTableClass::load() {
    pt::ptree root;
    pt::read_json("../ModelData.json", root);
    ModelStruct m;

    for(auto &i : root.get_child("Models")) {
        for(auto &a : i.second) {
            if (a.first == "Array") {
                loadArray(m, i.second);
            } else if (a.first == "Object") {
                loadObject(m, i.second);
            } else {
                m[a.first] = a.second.data();
            }
        }
        Models.push_back(m);
        m.Values.clear();
    }
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                              Успех                              |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}

void ModelsTableClass::save() {
    pt::ptree root;
    root.add_child("Models", saver());
    pt::write_json("../TestJSON.json", root);
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                              Успех                              |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}

void ModelsTableClass::Delete(ModelStruct m) {
    int pos = 0;
    bool IsHere = false;
    for (auto &i : Models) {
        if (m.mark() == i.mark() && m.model() == i.model() && m.type() == i.type()) {
            IsHere = true;
            break;
        }
        pos++;
    }
    if (IsHere) {
        auto delPos = Models.begin();
        advance(delPos, pos);
        Models.erase(delPos);
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Успех                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    } else {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                           Не найдено                            |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    }
}

pt::ptree ModelsTableClass::saver(ElementValue i, map<pair<string, string>, ElementType> b) {
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

pt::ptree ModelsTableClass::saver() {
    pt::ptree rootObject, rootObjectElements, rOEE;
    for (auto &i : Models) {
        for (auto &j : i.Values) {
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
                        rOEE.push_back(std::make_pair("", saver(*a, i.Fields)));
                    }
                    rootObjectElements.add_child(j.first, rOEE);
                    rOEE.clear();
                    break;
                case tobject:
                    rootObjectElements.add_child(j.first, saver(i.object()));
                    break;
            }
        }
        rootObject.push_back(std::make_pair("", rootObjectElements));
        rootObjectElements.clear();
    }
    return rootObject;
}

pt::ptree ModelsTableClass::saver(ModelStruct i) {
    pt::ptree rootObject, rootObjectElements, rOEE;
    for (auto &j : i.Values) {
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
                    rOEE.push_back(std::make_pair("", saver(*a, i.Fields)));
                }
                rootObjectElements.add_child(j.first, rOEE);
                rOEE.clear();
                break;
            case tobject:
                rootObjectElements.add_child(j.first, saver(i.object()));
                break;
        }
    }
    rootObject.push_back(std::make_pair("", rootObjectElements));
    rootObjectElements.clear();
    return rootObject;
}

vector<ModelStruct> ModelsTableClass::find(std::function<bool(const ModelStruct &)> filter, vector<ModelStruct> *vv) {
    vector<ModelStruct> output;
    if (vv) {
        for (auto &m : *vv) {
            if (filter && !filter(m)) {
                continue;
            }
            output.push_back(m);
        }
    } else {
        for (auto &m : Models) {
            if (filter && !filter(m)) {
                continue;
            }
            output.push_back(m);
        }
    }
    print(&output);
    return output;
}

void ModelsTableClass::update(ModelStruct m, ModelStruct New) {
    int pos = 0;
    bool IsHere = false;
    for (auto &i : Models) {
        if (m.mark() == i.mark() && m.model() == i.model() && m.type() == i.type()) {
            IsHere = true;
            break;
        }
        pos++;
    }
    if (IsHere) {
        int pos2 = 0;
        for (auto &i : Models) {
            if (pos2 == pos) {
                i = New;
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
