#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <cstdio> // For sscanf
#include <limits>

using namespace std;

struct Patient {
    string name;
    int age;
    int money;
    int membership;
    string joinDate;
    int inputOrder;

    //Constructor
    Patient() : name(""), age(0), money(0), membership(0), joinDate("NaN"), inputOrder(-1) {}
    
    // Parameterized constructor
    Patient(string n, int a, int m, int mb, string jd, int io)
        : name(n), age(a), money(m), membership(mb), joinDate(jd), inputOrder(io) {}
};

tuple<int, int, int> parseDate(const string& date) {
    if (date == "NaN") {
        //cout << "Parsing date: " << date << " -> {max, 12, 31}" << endl;
        return {std::numeric_limits<int>::max(), 12, 31};
    }
    int y, m, d;
    sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d);
    //cout << "Parsing date: " << date << " -> {" << y << ", " << m << ", " << d << "}" << endl;
    return {y, m, d};
}
// Earlier dates are better
/*
1. People with hospital membership
2. Longest to shortest membership
3. Name in alphabetical order
4. Input order
*/
bool customNameCompare(const string& a, const string& b) {
    // Iterators for both strings
    auto it_a = a.begin();
    auto it_b = b.begin();

    while (it_a != a.end() && it_b != b.end()) {
        char char_a = *it_a;
        char char_b = *it_b;

        // Check for space before any character
        if (char_a == ' ' && char_b != ' ') return true;
        if (char_a != ' ' && char_b == ' ') return false;

        // If no spaces, check for uppercase and lowercase
        if (isupper(char_a) && islower(char_b)) return true;
        if (islower(char_a) && isupper(char_b)) return false;

        // If same type, compare lexicographically
        if (char_a != char_b) return char_a < char_b;

        // Move to the next characters
        ++it_a;
        ++it_b;
    }

    // If all characters matched, the shorter string comes first
    return a.size() < b.size();
}

bool comparePanda(const Patient& a, const Patient& b) {
    if (a.membership != b.membership) return a.membership > b.membership;
    if (a.membership && b.membership) {
        auto adate = parseDate(a.joinDate);
        auto bdate = parseDate(b.joinDate);
        if (adate != bdate) return adate < bdate;
    }
    // Use custom name comparison
    if (a.name != b.name) return customNameCompare(a.name, b.name);
    return a.inputOrder < b.inputOrder;
}


bool compareBear(const Patient& a, const Patient& b) {
    if (a.money > b.money) {
        return true; // If 'a' has more money than 'b', 'a' should come first.
    } else if (a.money < b.money) {
        return false; // If 'b' has more money than 'a', 'b' should come first.
    } else {
        // If 'a' and 'b' have the same amount of money, compare by input order.
        return a.inputOrder < b.inputOrder;
    }
}


//Decide merge on Panda or Bear
void merge(vector<Patient>& vec, int l, int m, int r, bool panda) {
    vector<Patient> temp(r - l + 1);
    int i = l, j = m + 1, k = 0;

    while (i <= m && j <= r) {
        if (panda ? comparePanda(vec[i], vec[j]) : compareBear(vec[i], vec[j])) {
            temp[k++] = vec[i++];
        } else {
            temp[k++] = vec[j++];
        }
    }

    while (i <= m) temp[k++] = vec[i++];
    while (j <= r) temp[k++] = vec[j++];
    for (i = l, k = 0; i <= r; ++i, ++k) vec[i] = temp[k];
}

//Recursive vector merge
void mergeSort(vector<Patient>& vec, int l, int r, bool panda) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(vec, l, m, panda);
        mergeSort(vec, m + 1, r, panda);
        merge(vec, l, m, r, panda);
    }
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<Patient> patients;
    int currentInputOrder = 0; 

    for (int i = 0; i < N; i++) {
        string name, joinDate;
        int age, money, membership;
        cin.ignore();
        getline(cin, name);
        cin >> age >> money >> membership >> joinDate;
        patients.emplace_back(name, age, money, membership, joinDate, i);
        //cout << "Added: " << name << " " << age << " " << money << " " << membership << " " << joinDate << " Input Order: " << i << endl;
    }

    currentInputOrder = N;

    for (int day = 0; day < M; day++) {
        int P, X, K;
        cin >> P >> X >> K;
        for (int i = 0; i < P; i++) {
            string name, joinDate;
            int age, money, membership;
            cin.ignore();
            getline(cin, name);
            cin >> age >> money >> membership >> joinDate;
            patients.emplace_back(name, age, money, membership, joinDate, currentInputOrder);
            //cout << "Added: " << name << " " << age << " " << money << " " << membership << " " << joinDate << " Input Order: " << currentInputOrder << endl;
            currentInputOrder++;
        }
        //Debuggin
            /* 
            cout << "Initial list of patients:" << endl;
            for (auto& p : patients) {
                cout << p << endl;
            }
        */

        mergeSort(patients, 0, patients.size() - 1, true); // Sort for Panda Hospital
        
        //Debuggin
            /* 
            cout << "Initial list of patients:" << endl;
            for (auto& p : patients) {
                cout << p << endl;
            }
        */
       
        cout << "DAY #" << (day + 1) << "\n";
        long long income = 0;
        int processCount = min(X, (int)patients.size());
        for (int i = 0; i < processCount; i++) {
            cout << patients[i].name << " " << patients[i].age << "\n";
            income += (long long)patients[i].money * 0.9;
        }
        cout << "INCOME TODAY: " << income << "\n";

        patients.erase(patients.begin(), patients.begin() + processCount);

        // Handle remaining patients for Bear Hospital if necessary
        if (!patients.empty()) {
            if (patients.size() > K) {
                mergeSort(patients, 0, patients.size() - 1, false); // Sort for Bear Hospital
                patients.erase(patients.begin(), patients.begin() + K);
            } else {
                // If the remaining patients are fewer than K, all are processed
                patients.clear();
            }
        }
    }

    return 0;
}
