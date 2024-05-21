#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

class GradeNode {
public:
    int data;
    GradeNode* left;
    GradeNode* right;

    GradeNode(int data) : data(data), left(nullptr), right(nullptr) {}
};

class GradeBST {
public:
    GradeNode* root;

    GradeBST() : root(nullptr) {}

    void insert(int data) {
        root = insertRec(root, data);
    }

    bool all_of(int value) const {
        return all_ofRec(root, value);
    }

    bool any_of(int value) const {
        return any_ofRec(root, value);
    }

    int count(int value) const {
        return countRec(root, value);
    }

private:
    GradeNode* insertRec(GradeNode* node, int data) {
        if (!node) return new GradeNode(data);
        if (data < node->data)
            node->left = insertRec(node->left, data);
        else
            node->right = insertRec(node->right, data);
        return node;
    }

    bool all_ofRec(GradeNode* node, int value) const {
        if (!node) return true;
        if (node->data != value) return false;
        return all_ofRec(node->left, value) && all_ofRec(node->right, value);
    }

    bool any_ofRec(GradeNode* node, int value) const {
        if (!node) return false;
        if (node->data == value) return true;
        return any_ofRec(node->left, value) || any_ofRec(node->right, value);
    }

    int countRec(GradeNode* node, int value) const {
        if (!node) return 0;
        int count = (node->data == value) ? 1 : 0;
        return count + countRec(node->left, value) + countRec(node->right, value);
    }
};

class Student {
public:
    string last_name;
    string initials;
    GradeBST grades;

    Student(const string& ln, const string& ini, const GradeBST& gr)
            : last_name(ln), initials(ini), grades(gr) {}

    string toString() const {
        string result = last_name + " " + initials + ": ";
        inOrder(grades.root, result);
        return result;
    }

private:
    void inOrder(GradeNode* node, string& result) const {
        if (!node) return;
        inOrder(node->left, result);
        result += to_string(node->data) + " ";
        inOrder(node->right, result);
    }
};

class StudentNode {
public:
    Student data;
    StudentNode* left;
    StudentNode* right;

    StudentNode(const Student& data) : data(data), left(nullptr), right(nullptr) {}
};

class StudentBST {
private:
    StudentNode* root;

public:
    StudentBST() : root(nullptr) {}

    void addStudent(const Student& student) {
        root = insertRec(root, student);
    }

    void removeStudent(const string& last_name) {
        root = deleteRec(root, last_name);
    }

    void editStudent(const string& last_name, const Student& new_student) {
        removeStudent(last_name);
        addStudent(new_student);
    }

    void listAllStudents() const {
        inOrder(root);
    }

    void listExcellentStudents() const {
        inOrderCondition(root, [](const Student& student) {
            return student.grades.all_of(5);
        });
    }

    void listStudentsWithThrees() const {
        inOrderCondition(root, [](const Student& student) {
            return student.grades.any_of(3);
        });
    }

    void listStudentsWithTwos() const {
        inOrderCondition(root, [](const Student& student) {
            return student.grades.count(2) == 1;
        });
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            saveToFileRec(root, file);
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            root = nullptr;
            string last_name, initials;
            while (file >> last_name >> initials) {
                GradeBST grades;
                for (int i = 0; i < 5; ++i) {
                    int grade;
                    file >> grade;
                    grades.insert(grade);
                }
                addStudent(Student(last_name, initials, grades));
            }
        }
    }

private:
    StudentNode* insertRec(StudentNode* node, const Student& student) {
        if (!node) return new StudentNode(student);
        if (student.last_name < node->data.last_name)
            node->left = insertRec(node->left, student);
        else
            node->right = insertRec(node->right, student);
        return node;
    }

    StudentNode* deleteRec(StudentNode* node, const string& last_name) {
        if (!node) return node;
        if (last_name < node->data.last_name)
            node->left = deleteRec(node->left, last_name);
        else if (last_name > node->data.last_name)
            node->right = deleteRec(node->right, last_name);
        else {
            if (!node->left) {
                StudentNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                StudentNode* temp = node->left;
                delete node;
                return temp;
            }
            StudentNode* temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteRec(node->right, temp->data.last_name);
        }
        return node;
    }

    StudentNode* minValueNode(StudentNode* node) {
        StudentNode* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

    void inOrder(StudentNode* node) const {
        if (!node) return;
        inOrder(node->left);
        cout << node->data.toString() << "\n";
        inOrder(node->right);
    }

    void inOrderCondition(StudentNode* node, bool(*condition)(const Student&)) const {
        if (!node) return;
        inOrderCondition(node->left, condition);
        if (condition(node->data))
            cout << node->data.toString() << "\n";
        inOrderCondition(node->right, condition);
    }

    void saveToFileRec(StudentNode* node, ofstream& file) const {
        if (!node) return;
        file << node->data.last_name << " " << node->data.initials << " ";
        saveGrades(node->data.grades.root, file);
        file << "\n";
        saveToFileRec(node->left, file);
        saveToFileRec(node->right, file);
    }

    void saveGrades(GradeNode* node, ofstream& file) const {
        if (!node) return;
        saveGrades(node->left, file);
        file << node->data << " ";
        saveGrades(node->right, file);
    }
};

void printMenu() {
    cout << "1. Додати студента\n";
    cout << "2. Видалити студента\n";
    cout << "3. Редагувати студента\n";
    cout << "4. Показати всіх студентів\n";
    cout << "5. Показати студентів, які склали іспити тільки на 5\n";
    cout << "6. Показати студентів, які мають трійки\n";
    cout << "7. Показати студентів, які мають двійки\n";
    cout << "8. Зберегти дані у файл\n";
    cout << "9. Завантажити дані з файлу\n";
    cout << "0. Вийти\n";
}

bool inputGrade(int& grade) {
    cin >> grade;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

void addStudent(StudentBST& list) {
    string last_name, initials;
    GradeBST grades;
    cout << "Введіть прізвище: ";
    cin >> last_name;
    cout << "Введіть ініціали: ";
    cin >> initials;
    cout << "Введіть оцінки з 5 предметів: ";
    for (int i = 0; i < 5; ++i) {
        int grade;
        while (!inputGrade(grade)) {
            cout << "Помилка! Введіть числове значення для оцінки: ";
        }
        grades.insert(grade);
    }
    list.addStudent(Student(last_name, initials, grades));
}

void removeStudent(StudentBST& list) {
    string last_name;
    cout << "Введіть прізвище студента, якого потрібно видалити: ";
    cin >> last_name;
    list.removeStudent(last_name);
}

void editStudent(StudentBST& list) {
    string last_name, new_last_name, initials;
    GradeBST grades;
    cout << "Введіть прізвище студента, якого потрібно редагувати: ";
    cin >> last_name;
    cout << "Введіть нове прізвище: ";
    cin >> new_last_name;
    cout << "Введіть нові ініціали: ";
    cin >> initials;
    cout << "Введіть нові оцінки з 5 предметів: ";
    for (int i = 0; i < 5; ++i) {
        int grade;
        while (!inputGrade(grade)) {
            cout << "Помилка! Введіть числове значення для оцінки: ";
        }
        grades.insert(grade);
    }
    list.editStudent(last_name, Student(new_last_name, initials, grades));
}

void listAllStudents(const StudentBST& list) {
    list.listAllStudents();
}

void listExcellentStudents(const StudentBST& list) {
    list.listExcellentStudents();
}

void listStudentsWithThrees(const StudentBST& list) {
    list.listStudentsWithThrees();
}

void listStudentsWithTwos(const StudentBST& list) {
    list.listStudentsWithTwos();
}

void saveToFile(const StudentBST& list) {
    string filename;
    cout << "Введіть ім'я файлу для збереження: ";
    cin >> filename;
    list.saveToFile(filename);
}

void loadFromFile(StudentBST& list) {
    string filename;
    cout << "Введіть ім'я файлу для завантаження: ";
    cin >> filename;
    list.loadFromFile(filename);
}

int main() {
    StudentBST student_list;
    int choice;
    do {
        printMenu();
        cin >> choice;
        switch (choice) {
            case 1: addStudent(student_list); break;
            case 2: removeStudent(student_list); break;
            case 3: editStudent(student_list); break;
            case 4: listAllStudents(student_list); break;
            case 5: listExcellentStudents(student_list); break;
            case 6: listStudentsWithThrees(student_list); break;
            case 7: listStudentsWithTwos(student_list); break;
            case 8: saveToFile(student_list); break;
            case 9: loadFromFile(student_list); break;
        }
    } while (choice != 0);

    return 0;
}
