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
                Exit = false;
                break;
            case 1:
                break;
            case 2:
                ModelMenu();
                break;
            case 3:
                break;
            default:
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                          Попробуйте ещё                         |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
}

void Menu::ModelMenu() {
    static CarModelTable mtc;

    bool Exit = true;
    int menu = 0;
    CarModelModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<CarModelModel()> Finder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Поиск                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        mtc.print();
        string s;
        CarModelModel m;
        vector<CarModelModel> a;
        for (auto &i : m.Fields()) {
            if (i.first == "ID") {
                continue;
            }
            mtc.print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;
            if (!a.empty()) {
                a = mtc.find([i, s](CarModelModel el) {return *el[i.first].value.tstring == s;}, &a);
            } else {
                a = mtc.find([i, s](CarModelModel el) { return *el[i.first].value.tstring == s;});
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<CarModelModel()> Adder = [](){
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                             Создать                             |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        random_generator gen;
        string s;
        CarModelModel m;

        for (auto &i : m.Fields()) {
            if (i.first == "ID") {
                m["ID"] = to_string(gen());
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            m[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1              Добавить модель | Удалить модель               2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3         Редактировать модель | Показать модели              4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5             Сохранить модели | Загрузить модели             6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7                        Найти | Назад                        0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        switch (menu) {
            case 0:
                Exit = false;
                break;
            case 1:
                mtc.add(Adder());
                break;
            case 2:
                mtc.remove(Finder());
                break;
            case 3:
                m = Finder();
                s = *m["ID"].value.tstring;
                m = Adder();
                m["ID"] = s;
                mtc.update(m);
                break;
            case 4:
                mtc.print();
                break;
            case 5:
                mtc.save();
                break;
            case 6:
                mtc.load();
                break;
            case 7:
                mtc.print();
                n = 1;
                for (auto &i : m.Fields()) {
                    if (i.first == "ID") continue;
                    cout << "| " << n << " | " << i.second.Description << " |" << endl;
                    n++;
                }
                n = 1;
                cout << "Поле -> "; cin >> pos;
                for (auto &i : m.Fields()) {
                    if (i.first == "ID") continue;
                    if (n == pos) {
                        field = i.first;
                        break;
                    }
                    n++;
                }
                cout << "Значение -> "; cin >> value;
                mtc.find([field, value](CarModelModel el) {return *el[field].value.tstring == value;});
                break;
            default:
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                cout << "|                          Попробуйте ещё                         |" << endl;
                cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        }
    }
}
