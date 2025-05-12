#include <iostream>
#include <vector>
#define pb push_back
using namespace std;

struct Course {
    string name;
    int unit;
    vector<string> pre;
};

struct Passed {
    string name;
    int unit;
    double grade;
};

int m, n, k;
vector<Course> courses;
vector<Passed> passed;
vector<string> want;

vector<string> split(string str)
{
    vector<string> ans;
    string word = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ',') {
            ans.pb(word);
            word = "";
        } else {
            word += str[i];
        }
    }
    ans.pb(word);
    return ans;
}

bool is_presenting(string name)
{
    for (auto present : courses) {
        if (name == present.name) {
            return true;
        }
    }
    return false;
}

bool is_passed(string name)
{
    for (auto already : passed) {
        if (name == already.name and already.grade >= 10) {
            return true;
        }
    }
    return false;
}

int get_unit_sum()
{
    int ans = 0;
    for (auto course : want) {
        for (auto present : courses) {
            if (course == present.name) {
                ans += present.unit;
                break;
            }
        }
    }
    return ans;
}

double get_avg()
{
    double sum = 0;
    int units = 0;
    for (auto course : passed) {

        sum += course.grade * course.unit;
        units += course.unit;
    }
    return sum / units;
}

int main()
{
    cin >> m;
    courses.resize(m);
    for (int i = 0; i < m; i++) {
        string tmp;
        cin >> courses[i].name >> courses[i].unit >> tmp;
        courses[i].pre = split(tmp);
    }
    cin >> n;
    passed.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> passed[i].name >> passed[i].unit >> passed[i].grade;
    }
    cin >> k;
    want.resize(k);
    for (int i = 0; i < k; i++) {
        cin >> want[i];
    }
    for (auto course : want) {
        if (!is_presenting(course)) {
            cout << "Course Not Offered!";
            return 0;
        }
    }
    int unit_sum = get_unit_sum();
    double avg = get_avg();
    if (unit_sum < 12) {
        cout << "Minimum Units Validation Failed!";
        return 0;
    }
    if (avg >= 17) {
        if (unit_sum > 24) {
            cout << "Maximum Units Validation Failed!";
            return 0;
        }

    } else if (avg >= 12) {
        if (unit_sum > 20) {
            cout << "Maximum Units Validation Failed!";
            return 0;
        }

    } else {
        if (unit_sum > 14) {
            cout << "Maximum Units Validation Failed!";
            return 0;
        }
    }
    for (auto course : want) {
        for (auto already : passed) {
            if (course == already.name and already.grade >= 10) {
                cout << "Course Already Taken!";
                return 0;
            }
        }
    }
    for (auto course : want) {
        for (auto present : courses) {
            if (course == present.name) {
                for (auto p : present.pre) {
                    if (!is_passed(p)) {
                        cout << "Prerequisites Not Met!";
                        return 0;
                    }
                }
            }
        }
    }
    cout << "OK!";
}