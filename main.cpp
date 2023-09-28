#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <limits>

using namespace std;

// Funkcija apskaičiuoti medianą
double calculateMedian(vector<int> values) {
    size_t size = values.size();
    sort(values.begin(), values.end());
    if (size % 2 == 0) {
        return (values[size / 2 - 1] + values[size / 2]) / 2.0;
    } else {
        return values[size / 2];
    }
}

// struktura laikyti studento duomenis
struct Student {
    string name;
    vector<int> homework_scores;
    int exam_score;
};

// Funkcija studentu rusiavimui
bool compareStudents(const Student& a, const Student& b) {
    return a.name < b.name;
}

int main() {
    vector<Student> students;

    // Atsitiktinių skačių generavimas
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> score_distribution(0, 10);

    cout << "Kaip norite, kad būtų įvesti duomenys? (1: File, 2: Ranka): ";
    int inputChoice;
    cin >> inputChoice;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (inputChoice == 1) {
        // skaitymas iš file
        ifstream inputFile("kursiokai.txt");
        if (!inputFile) {
            cerr << "Nepavyko atidaryti file, įveskite duomenis ranka" << endl;
        } else {
            string line;
            while (getline(inputFile, line)) {
                istringstream iss(line);
                string first_name, last_name;
                iss >> first_name >> last_name;
                Student student;
                student.name = last_name + ", " + first_name;

                int score;
                while (iss >> score) {
                    student.homework_scores.push_back(score);
                }
                student.exam_score = student.homework_scores.back();
                student.homework_scores.pop_back();

                students.push_back(student);
            }
            inputFile.close();
        }
    }

    if (students.empty()) {
        // Duomenų įvedimas ranka
        while (true) {
            string first_name, last_name;
            cout << "Įveskite studento vardą, arba parasšykit 'stop' kad sustabdyti įvedimą) ";
            cin >> first_name;

            if (first_name == "stop") {
                break;
            }

            cout << "Įveskite studento pavardę: ";
            cin >> last_name;

            Student student;
            student.name = last_name + ", " + first_name;

            cout << "Ar norite kad programa sugeneruotu atsitiktinius namų darbų rezultatus? (t/n): ";
            char generate_homework;
            cin >> generate_homework;

            if (generate_homework == 't' || generate_homework == 'T') {
                for (int i = 0; i < 5; ++i) {
                    student.homework_scores.push_back(score_distribution(gen));
                }
            } else {
                // Įvestis namų darbų
                cout << "Įveskite " << student.name << " namų darbų rezultatus (atskirkite juos tarpais, baigus įveskite 'baigta'): ";
                int score;
                while(cin>>score) {
                    if (score < 0 || score > 10) {
                        cout << "Namų darbų rezultatas turi būti tarp 1 ir 10. Bandykite dar karta: ";
                    } else {
                        student.homework_scores.push_back(score);
                    }
                    string done;
                    cin >> done;
                    if (done == "baigta") {
                        break;
                    }
                }
            }

            // Egzamino rezultato įvedimas
            cout << "Ar norite kad programa sugeneruotų atsitiktinį egzamino rezultatą? (t/n)";
            char generate_exam;
            cin >> generate_exam;

            if (generate_exam == 't' || generate_exam == 'T') {
                student.exam_score = score_distribution(gen);
            } else {
                // Egzamino rezultato įvedimas
                cout << "Įveskite " << student.name << " egzamino rezultatą (tarp 0 ir 10): ";
                while (true) {
                    cin >> student.exam_score;
                    if (student.exam_score < 0 || student.exam_score > 10) {
                        cout << "Egzamino rezultatas turi būti tarp 0 ir 10 bandykite dar kartą: ";
                    } else {
                        break;
                    }
                }
            }

            students.push_back(student);
        }
    }

    // studentų rušiavimas
    sort(students.begin(), students.end(), compareStudents);

    // rez file atidarymas
    ofstream outputFile("rez.txt");
    if (!outputFile) {
        cerr << "Įvyko klaida atidarant file" << endl;
        return 1;
    }

    // Duomenų spausdinimas
    cout << setw(20) << left << "Pavardė, Vardas" << setw(15) << "Galutinis vid." << setw(15) << "Galutinis med." << endl;
    outputFile << setw(20) << left << "Pavardė, Vardas" << setw(15) << "Galutinis vid" << setw(15) << "Galutinis med." << endl;
    for (const Student& student : students) {
        double homework_average = 0.0;
        for (int score : student.homework_scores) {
            homework_average += score;
        }
        homework_average /= student.homework_scores.size();

        double final_score = 0.4 * homework_average + 0.6 * student.exam_score;
        double median = calculateMedian(student.homework_scores);

        cout << setw(20) << left << student.name << setw(15) << fixed << setprecision(2) << final_score << setw(15) << median << endl;
        outputFile << setw(20) << left << student.name << setw(15) << fixed << setprecision(2) << final_score << setw(15) << median << endl;
    }

    outputFile.close();

    cout << "Duomenys išvesti 'rez.txt'." << endl;

    return 0;
}

