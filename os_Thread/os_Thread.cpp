#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// Функция потока для поиска подстроки в строке
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    string* params = reinterpret_cast<string*>(lpParam); // Приводим lpParam к типу строки
    //string* params = static_cast<string*>(lpParam);

    string str = params[0]; // Исходная строка
    string substr = params[1]; // Исходная подстрока

    int strLength = str.length();
    int substrLength = substr.length();

    for (int i = 0; i <= strLength - substrLength; ++i) {
        int j;
        for (j = 0; j < substrLength; ++j) {
            if (str[i + j] != substr[j]) {
                break; // Если символы не совпадают, выходим из цикла
            }
        }

        if (j == substrLength) {
            return i; // Если все символы подстроки совпали, возвращаем индекс начала подстроки
        }
    }

    return -1; // Если подстрока не найдена
}

int main() {
    setlocale(LC_ALL, "Russian");

    string inputStr, inputSubstr;

    // Ввод строк
    cout << "Введите строку: ";
    cin >> inputStr;

    cout << "Введите подстроку для поиска: ";
    cin >> inputSubstr;

    // Параметры для передачи в поток
    string params1[2] = { inputStr, inputSubstr };

    HANDLE hThread1;
    DWORD threadId1;

    // Создание потока в приостановленном состоянии
    hThread1 = CreateThread(NULL, 0, ThreadFunction, params1, CREATE_SUSPENDED, &threadId1);
    if (hThread1 == NULL) {
        cerr << "Ошибка при создании потока: " << GetLastError() << endl;
        return 1;
    }

    // Выполнение потока
    ResumeThread(hThread1);

    // Ожидание завершения потока
    WaitForSingleObject(hThread1, INFINITE); //останавливает выполнения программы до тех пор пока объект, идентификатор
                                             //которого передан в функцию ,не окажется в "сигнальном" состоянии
    // Получение результатов из потока
    DWORD foundIndex1, foundIndex2;
    GetExitCodeThread(hThread1, &foundIndex1);

    // Закрытие дескрипторов потока
    CloseHandle(hThread1);

    // Вывод результата
    if (foundIndex1 != static_cast<DWORD>(-1)) {
        cout << "Подстрока найдена в позиции: " << foundIndex1 << endl;
    }
    else {
        cout << "Подстрока не найдена" << endl;
    }

    return 0;
}
