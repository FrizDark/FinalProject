//
// Created by Dmitriy Steshenko on 09.05.2020.
//

#include "Menu.h"

void Menu::MainMenu() {
    bool Exit = true;
    int menu = 0;
    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1                    Менеджеры | Модели                       2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3                       Машины | Выход                        0 |" << endl;
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
            default:
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                          Попробуйте ещё                         |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
}

void Menu::CarModelMenu() {
    static ModelTable carModelTable = _carModelMenu;

    bool Exit = true;
    int menu = 0;
    ModelModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<ModelModel()> Finder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Поиск                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        carModelTable.print();
        string s;
        ModelModel m;
        vector<ModelModel> a;
        for (auto &i : m.Fields()) {
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
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                           Не найдено                            |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            }
            if (a.size() == 1) {
                carModelTable.print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<ModelModel*()> Adder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                             Создать                             |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
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
                carModelTable.add(*Adder());
                break;
            case 2:
                if (carModelTable.remove(Finder())) {
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
                if (carModelTable.update(m)) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                } else {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                           Не найдено                            |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 4:
                carModelTable.print();
                break;
            case 5:
                if (carModelTable.save()) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 6:
                if (carModelTable.load()) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 7:
                carModelTable.printM(Finder());
                break;
            default:
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                          Попробуйте ещё                         |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
    _carModelMenu = carModelTable;
}

void Menu::CarMenu() {
    static CarTable carTable = _carMenu;

    bool Exit = true;
    int menu = 0;
    CarModel m;
    string s, field, value;
    int n = 1;
    int pos;

    std::function<CarModel()> Finder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Поиск                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        carTable.print();
        string s;
        CarModel m;
        vector<CarModel> a;

        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) carTable.print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = carTable.find([i, s](CarModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = carTable.find([i, s](CarModel el) { return *el[i.first].value.tstring == s;});
            }
            if (a.empty()) {
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                           Не найдено                            |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            }
            if (a.size() == 1) {
                carTable.print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<CarModel*(ModelTable carModelTable)> Adder = [](ModelTable carModelTable){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                             Создать                             |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
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
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                           Не найдено                            |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
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
                    (*m)["MarkID"] = *cmm["ID"].value.tstring;
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
                carTable.add(*Adder(_carModelMenu));
                break;
            case 2:
                if (carTable.remove(Finder())) {
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
                m = *Adder(_carModelMenu);
                m["ID"] = s;
                if (carTable.update(m)) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                } else {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                           Не найдено                            |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 4:
                carTable.print();
                break;
            case 5:
                if (carTable.save()) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 6:
                if (_carModelMenu.load() && carTable.load()) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 7:
                carTable.printM(Finder());
                break;
            default:
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                          Попробуйте ещё                         |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
    _carMenu = carTable;
}

void Menu::ManagerMenu() {
    static ManagerTable managerTable = _managerMenu;

    bool Exit = true;
    int menu = 0;
    ManagerModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<ManagerModel()> Finder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Поиск                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        managerTable.print();
        string s;
        ManagerModel m;
        vector<ManagerModel> a;
        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) managerTable.print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = managerTable.find([i, s](ManagerModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = managerTable.find([i, s](ManagerModel el) { return *el[i.first].value.tstring == s;});
            }
            if (a.empty()) {
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                           Не найдено                            |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
            }
            if (a.size() == 1) {
                managerTable.print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<ManagerModel*()> Adder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                             Создать                             |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
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
                managerTable.add(*Adder());
                break;
            case 2:
                if (managerTable.remove(Finder())) {
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
                if (managerTable.update(m)) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                } else {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                           Не найдено                            |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 4:
                managerTable.print();
                break;
            case 5:
                if (managerTable.save()) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 6:
                if (managerTable.load()) {
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    cout << "|                              Успех                              |" << endl;
                    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                }
                break;
            case 7:
                managerTable.printM(Finder());
                break;
            default:
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                          Попробуйте ещё                         |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
    _managerMenu = managerTable;
}
