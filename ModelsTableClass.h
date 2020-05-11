//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#ifndef FINALPROJECTCPP_MODELSTABLECLASS_H
#define FINALPROJECTCPP_MODELSTABLECLASS_H

#include "IncludeLib.h"
#include "ModelStruct.h"

class ModelsTableClass {
private:
    void print(ModelStruct m);

    void loadObject(ModelStruct &m, pt::ptree root);
    void loadArray(ModelStruct &m, pt::ptree root);

    pt::ptree saver();
    pt::ptree saver(ElementValue i, map<pair<string, string>, ElementType> b);
    pt::ptree saver(ModelStruct i);
public:
    list<ModelStruct> Models;

    void add(ModelStruct m);
    void print(vector<ModelStruct> *vv = NULL);
    void load();
    void save();
    void Delete(ModelStruct m);
    vector<ModelStruct> find(std::function<bool(const ModelStruct &)> filter = NULL, vector<ModelStruct> *vv = NULL);
    void update(ModelStruct m, ModelStruct New);
};


#endif //FINALPROJECTCPP_MODELSTABLECLASS_H
