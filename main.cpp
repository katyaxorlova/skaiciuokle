#include <iostream>
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

int main() {
    vector<string> names;
    vector<vector<int>> homeworks;
    vector<int> exams;

    // Atsitiktinių skačių generavimas
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> score_distribution(0, 10);

    while (true) {
        string first_name, last_name;
        cout << "Įveskite studento vardą, arba parasšykit 'stop' kad sustabdyti įvedimą): ";
        cin >> first_name;

        if (first_name == "stop") {
            break;
        }

        cout << "Įveskite studento pavardę: ";
        cin >> last_name;

        names.push_back(last_name + ", " + first_name);

        vector<int> homework_scores;
        int exam_score;

        cout << "Ar norite kad programa sugeneruotu atsitiktinius namų darbų rezultatus? (t/n): ";
        char generate_homework;
        cin >> generate_homework;

        if (generate_homework == 't' || generate_homework == 'T') {
            for (size_t i = 0; i < names.size(); ++i) {
                homework_scores.push_back(score_distribution(gen));
            }
        } else {
            cout << "Įveskite " << first_name << " " << last_name << " namų darbų rezultatus (atskirkite juos tarpais): ";
            int score;
            while (cin >> score) {
                if (cin.fail() || score < 0 || score > 10) {
                    cin.clear(); // ištriname blogą įvestį
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Homework score must be a valid number between 0 and 10. Try again: ";
                    continue;
                }
                homework_scores.push_back(score);
                if (homework_scores.size() == names.size()) {
                    break;
                }
            }
            if (homework_scores.size() != names.size()) {
                cout << "Bad input." << endl;
                break;
            }
        }

        cin.clear(); // ištriname blogą įvestį
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ar norite, kad programa sugeneruotu egzamimo rezultatą? (t/n): ";
        char generate_exam;
        cin >> generate_exam;

        if (generate_exam == 't' || generate_exam == 'T') {
            exam_score = score_distribution(gen);
        } else {
            while (true) {
                cout << "Įveskite " << first_name << " " << last_name << "egzamino rezultatą: ";
                cin >> exam_score;
                if (cin.fail() || exam_score < 0 || exam_score > 10) {
                    cin.clear(); //ištrinam blogą įvestį
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Exam score must be a valid number between 0 and 10. Try again: ";
                    continue;
                } else {
                    break;
                }
            }
        }

        homeworks.push_back(homework_scores);
        exams.push_back(exam_score);

        cin.clear(); // ištrinam blogą įvestį
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << setw(20) << left << "Pavardė, Vardas" << setw(15) << "Galutinis vid" << setw(15) << "Galutinė med" << endl;

    for (size_t i = 0; i < names.size(); ++i) {
        double homework_average = 0.0;
        for (size_t j = 0; j < homeworks[i].size(); ++j) {
            homework_average += homeworks[i][j];
        }
        homework_average /= homeworks[i].size();

        double final_score = 0.4 * homework_average + 0.6 * exams[i];
        double median = calculateMedian(homeworks[i]);

        cout << setw(20) << left << names[i] << setw(15) << fixed << setprecision(2) << final_score << setw(15) << median << endl;
    }

    return 0;
}

