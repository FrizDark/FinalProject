//
// Created by Dmitriy Steshenko on 09.05.2020.
//

#include "Menu.h"

void print_Success() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                              Успех                              |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_NotFound() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                           Не найдено                            |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_TryAgain() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                          Попробуйте ещё                         |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_Search() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                              Поиск                              |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_Create() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                             Создать                             |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}



void Menu::MainMenu() {
    bool Exit = true;
    int menu = 0;

    CarModelView cmv;

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1                    Менеджеры | Модели                       2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3                       Машины | Показать машины и модели     4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 Показать машины и менеджеров | Выход                        0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        switch (menu) {
            case 0:
                //TODO: Save all
                Exit = false;
                break;
            case 1:
                ManagerMenu();
                break;
            case 2:
                CarModelMenu();
                break;
            case 3:
                CarMenu();
                break;
            case 4:
                cmv.print();
                break;
            case 5:
                break;
            default:
                print_TryAgain();
        }
    }
}

void Menu::CarModelMenu() {
//    static ModelTable carModelTable = _carModelMenu;

    bool Exit = true;
    int menu = 0;
    ModelModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<ModelModel()> Finder = [](){
        print_Search();
        ModelTable::instance().print();
        string s;
        ModelModel m;
        vector<ModelModel> a;
        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) ModelTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = ModelTable::instance().find([i, s](ModelModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = ModelTable::instance().find([i, s](ModelModel el) { return *el[i.first].value.tstring == s;});
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                ModelTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<ModelModel*()> Adder = [](){
        print_Create();
        random_generator gen;
        string s;
        ModelModel *m = new ModelModel();

        for (auto &i : m->Fields()) {
            if (i.second.Description == "ID") {
                (*m)["ID"] = to_string(gen());
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            (*m)[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                             Модель                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        switch (menu) {
            case 0:
                Exit = false;
                break;
            case 1:
                ModelTable::instance().add(*Adder());
                break;
            case 2:
                if (ModelTable::instance().remove(Finder())) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                } else {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                           Не найдено                            |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 3:
                m = Finder();
                s = *m["ID"].value.tstring;
                m = *Adder();
                m["ID"] = s;
                if (ModelTable::instance().update(m)) {
                    print_Success();
                } else {
                    print_NotFound();
                }
                break;
            case 4:
                ModelTable::instance().print();
                break;
            case 5:
                if (ModelTable::instance().save()) {
                    print_Success();
                }
                break;
            case 6:
                if (ModelTable::instance().load()) {
                    print_Success();
                }
                break;
            case 7:
                ModelTable::instance().printM(Finder());
                break;
            default:
                print_TryAgain();
        }
    }
//    _carModelMenu = carModelTable;
}

void Menu::CarMenu() {
//    static CarTable carTable = _carMenu;

    bool Exit = true;
    int menu = 0;
    CarModel m;
    string s, field, value;
    int n = 1;
    int pos;

    std::function<CarModel()> Finder = [](){
        print_Search();
        CarTable::instance().print();
        string s;
        CarModel m;
        vector<CarModel> a;

        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) CarTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = CarTable::instance().find([i, s](CarModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = CarTable::instance().find([i, s](CarModel el) { return *el[i.first].value.tstring == s;});
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                CarTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<CarModel*(ModelTable carModelTable)> Adder = [](ModelTable carModelTable){
        print_Create();
        random_generator gen;
        string s;
        CarModel *m = new CarModel();

        ModelModel cmm;

        carModelTable.print();
        vector<ModelModel> a;
        for (auto &i : cmm.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) carModelTable.print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = carModelTable.find([i, s](ModelModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = carModelTable.find([i, s](ModelModel el) { return *el[i.first].value.tstring == s;});
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                carModelTable.print(&a);
                cmm = a[0];
                break;
            }
            cmm[i.first] = s;
        }
        cmm["ID"] = *a[0]["ID"].value.tstring;
        a.clear();
        s.clear();

        for (auto &i : (*m).Fields()) {
            if (i.second.Description == "ID") {
                if (i.first == "ID") {
                    (*m)["ID"] = to_string(gen());
                } else {
                    (*m)["Model_ID"] = *cmm["ID"].value.tstring;
                }
                continue;
            }
            if (i.first == "Mark") {
                (*m)["Mark"] = *cmm["Mark"].value.tstring;
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            (*m)[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                            Автомобиль                           |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        switch (menu) {
            case 0:
                Exit = false;
                break;
            case 1:
                CarTable::instance().add(*Adder(ModelTable::instance()));
                break;
            case 2:
                if (CarTable::instance().remove(Finder())) {
                    print_Success();
                } else {
                    print_NotFound();
                }
                break;
            case 3:
                m = Finder();
                s = *m["ID"].value.tstring;
                m = *Adder(ModelTable::instance());
                m["ID"] = s;
                if (CarTable::instance().update(m)) {
                    print_Success();
                } else {
                    print_NotFound();
                }
                break;
            case 4:
                CarTable::instance().print();
                break;
            case 5:
                if (CarTable::instance().save()) {
                    print_Success();
                }
                break;
            case 6:
                if (ModelTable::instance().elements().empty()) ModelTable::instance().load();
                if (CarTable::instance().load()) {
                    print_Success();
                }
                break;
            case 7:
                CarTable::instance().printM(Finder());
                break;
            default:
                print_TryAgain();
        }
    }
//    _carMenu = carTable;
}

void Menu::ManagerMenu() {
//    static ManagerTable managerTable = _managerMenu;

    bool Exit = true;
    int menu = 0;
    ManagerModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<ManagerModel()> Finder = [](){
        print_Search();
        ManagerTable::instance().print();
        string s;
        ManagerModel m;
        vector<ManagerModel> a;
        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) ManagerTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = ManagerTable::instance().find([i, s](ManagerModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = ManagerTable::instance().find([i, s](ManagerModel el) { return *el[i.first].value.tstring == s;});
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                ManagerTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<ManagerModel*()> Adder = [](){
        print_Create();
        random_generator gen;
        string s;
        int n;
        ManagerModel *m = new ManagerModel();

        for (auto &i : (*m).Fields()) {
            if (i.second.Description == "ID") {
                (*m)["ID"] = to_string(gen());
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            (*m)[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                            Менеджер                             |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        switch (menu) {
            case 0:
                Exit = false;
                break;
            case 1:
                ManagerTable::instance().add(*Adder());
                break;
            case 2:
                if (ManagerTable::instance().remove(Finder())) {
                    print_Success();
                } else {
                    print_NotFound();
                }
                break;
            case 3:
                m = Finder();
                s = *m["ID"].value.tstring;
                m = *Adder();
                m["ID"] = s;
                if (ManagerTable::instance().update(m)) {
                    print_Success();
                } else {
                    print_NotFound();
                }
                break;
            case 4:
                ManagerTable::instance().print();
                break;
            case 5:
                if (ManagerTable::instance().save()) {
                    print_Success();
                }
                break;
            case 6:
                if (ManagerTable::instance().load()) {
                    print_Success();
                }
                break;
            case 7:
                ManagerTable::instance().printM(Finder());
                break;
            default:
                print_TryAgain();
        }
    }
//    _managerMenu = managerTable;
}
