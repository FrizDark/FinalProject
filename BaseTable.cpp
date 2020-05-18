//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#include "BaseTable.h"

pt::ptree BaseTable::saver(const Model &i) {
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

pt::ptree BaseTable::saver() {
    pt::ptree rootObject, rootObjectElements, rOEE;
    for (auto &i : m_elements) {
        for (auto &j : i->Values()) {
            switch (j.second.type) {
                case empty:
                    rootObjectElements.put(j.first, NULL);
                    break;
                case tnumber:
                    rootObjectElements.put(j.first, (*i)[j.first].value.tnumber);
                    break;
                case tboolean:
                    rootObjectElements.put(j.first, (*i)[j.first].value.tboolean);
                    break;
                case tstring:
                    rootObjectElements.put(j.first, *(*i)[j.first].value.tstring);
                    break;
                case tarray:
                    for (auto a = (*i)[j.first].value.tarray->cbegin(); a != (*i)[j.first].value.tarray->cend(); a++) {
                        rOEE.push_back(std::make_pair("", saver(*a, (*i).Fields())));
                    }
                    rootObjectElements.add_child(j.first, rOEE);
                    rOEE.clear();
                    break;
                case tobject:
                    rootObjectElements.add_child(j.first, saver(*(*i)[j.first].value.tobject));
                    break;
            }
        }
        rootObject.push_back(std::make_pair("", rootObjectElements));
        rootObjectElements.clear();
    }
    return rootObject;
}

pt::ptree BaseTable::saver(ElementValue i, map<std::string, TypeName> b) {
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

bool BaseTable::save() {
    pt::ptree root;
    root.add_child(name(), saver());
    /*TODO: Save in one file:
     * string fileName = "../" + name() + ".json";
     * pt::write_json(fileName, root);
    */
    pt::write_json("../TestJSON.json", root);
    return true;
}