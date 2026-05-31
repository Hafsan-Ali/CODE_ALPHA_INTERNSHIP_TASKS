#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

class Task {
public:
    int         id;
    string      title;
    string      category;
    bool        completed;

    Task() : id(0), completed(false) {}

    Task(int id, const string& title, const string& category)
        : id(id), title(title), category(category), completed(false) {}

    // Serialize to file line:  id|title|category|completed
    string serialize() const {
        return to_string(id) + "|" + title + "|" + category + "|" + (completed ? "1" : "0");
    }

    // Deserialize from file line
    static Task deserialize(const string& line) {
        Task t;
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        t.id        = stoi(line.substr(0, p1));
        t.title     = line.substr(p1 + 1, p2 - p1 - 1);
        t.category  = line.substr(p2 + 1, p3 - p2 - 1);
        t.completed = (line.substr(p3 + 1) == "1");
        return t;
    }
};


class TaskManager {
private:
    vector<Task> tasks;
    string       filename;
    int          nextId;

    void recalcNextId() {
        nextId = 1;
        for (const auto& t : tasks)
            if (t.id >= nextId) nextId = t.id + 1;
    }

public:
    TaskManager(const string& file = "tasks.txt")
        : filename(file), nextId(1) {
        loadFromFile();
    }

   
    void saveToFile() const {
        ofstream ofs(filename);
        if (!ofs) { cerr << "  [Error] Cannot open file for writing.\n"; return; }
        for (const auto& t : tasks)
            ofs << t.serialize() << "\n";
        cout << "  Tasks saved to \"" << filename << "\".\n";
    }

    void loadFromFile() {
        ifstream ifs(filename);
        if (!ifs) return;          
        tasks.clear();
        string line;
        while (getline(ifs, line))
            if (!line.empty()) tasks.push_back(Task::deserialize(line));
        recalcNextId();
        cout << "  Loaded " << tasks.size() << " task(s) from \"" << filename << "\".\n";
    }


    void addTask(const string& title, const string& category) {
        tasks.emplace_back(nextId++, title, category);
        cout << "  Task #" << (nextId - 1) << " added successfully.\n";
    }

    void markCompleted(int id) {
        for (auto& t : tasks) {
            if (t.id == id) {
                if (t.completed)
                    cout << "  Task #" << id << " is already completed.\n";
                else {
                    t.completed = true;
                    cout << "  Task #" << id << " marked as completed.\n";
                }
                return;
            }
        }
        cout << "  Task #" << id << " not found.\n";
    }

   
    void deleteTask(int id) {
        auto it = remove_if(tasks.begin(), tasks.end(),
                            [id](const Task& t){ return t.id == id; });
        if (it == tasks.end())
            cout << "  Task #" << id << " not found.\n";
        else {
            tasks.erase(it, tasks.end());
            cout << "  Task #" << id << " deleted.\n";
        }
    }

    
    void printHeader() const {
        cout << "\n  " << string(60, '-') << "\n";
        cout << "  " << left
             << setw(5)  << "ID"
             << setw(25) << "Title"
             << setw(15) << "Category"
             << setw(10) << "Status"
             << "\n";
        cout << "  " << string(60, '-') << "\n";
    }

    void printTask(const Task& t) const {
        cout << "  " << left
             << setw(5)  << t.id
             << setw(25) << t.title
             << setw(15) << t.category
             << setw(10) << (t.completed ? "Done" : "Pending")
             << "\n";
    }

    void viewAll() const {
        if (tasks.empty()) { cout << "  No tasks found.\n"; return; }
        printHeader();
        for (const auto& t : tasks) printTask(t);
        cout << "  " << string(60, '-') << "\n";
    }

    void viewPending() const {
        printHeader();
        bool any = false;
        for (const auto& t : tasks)
            if (!t.completed) { printTask(t); any = true; }
        if (!any) cout << "  No pending tasks.\n";
        cout << "  " << string(60, '-') << "\n";
    }

    void viewCompleted() const {
        printHeader();
        bool any = false;
        for (const auto& t : tasks)
            if (t.completed) { printTask(t); any = true; }
        if (!any) cout << "  No completed tasks.\n";
        cout << "  " << string(60, '-') << "\n";
    }

    void viewByCategory(const string& cat) const {
        printHeader();
        bool any = false;
        for (const auto& t : tasks)
            if (t.category == cat) { printTask(t); any = true; }
        if (!any) cout << "  No tasks in category \"" << cat << "\".\n";
        cout << "  " << string(60, '-') << "\n";
    }

   
    void showSummary() const {
        int total = tasks.size();
        int done  = count_if(tasks.begin(), tasks.end(),
                             [](const Task& t){ return t.completed; });
        cout << "\n  Total: " << total
             << "  |  Completed: " << done
             << "  |  Pending: "   << (total - done) << "\n";
    }
};


void printMenu() {
    cout << "\n  ╔══════════════════════════════╗\n";
    cout << "  ║      TO-DO LIST MANAGER      ║\n";
    cout << "  ╠══════════════════════════════╣\n";
    cout << "  ║  1. Add Task                 ║\n";
    cout << "  ║  2. Mark Task as Completed   ║\n";
    cout << "  ║  3. View All Tasks           ║\n";
    cout << "  ║  4. View Pending Tasks       ║\n";
    cout << "  ║  5. View Completed Tasks     ║\n";
    cout << "  ║  6. View Tasks by Category   ║\n";
    cout << "  ║  7. Delete a Task            ║\n";
    cout << "  ║  8. Save Tasks               ║\n";
    cout << "  ║  9. Summary                  ║\n";
    cout << "  ║  0. Exit                     ║\n";
    cout << "  ╚══════════════════════════════╝\n";
    cout << "  Choice: ";
}

string getInput(const string& prompt) {
    string s;
    cout << "  " << prompt;
    cin.ignore();
    getline(cin, s);
    return s;
}

int main() {
    TaskManager manager("tasks.txt");
    int choice;

    do {
        printMenu();
        cin >> choice;

        switch (choice) {

        case 1: {
            string title    = getInput("Task title    : ");
            string category = getInput("Category      : ");
            manager.addTask(title, category);
            break;
        }
        case 2: {
            int id;
            cout << "  Task ID to mark complete: ";
            cin >> id;
            manager.markCompleted(id);
            break;
        }
        case 3:
            manager.viewAll();
            break;
        case 4:
            manager.viewPending();
            break;
        case 5:
            manager.viewCompleted();
            break;
        case 6: {
            string cat = getInput("Enter category: ");
            manager.viewByCategory(cat);
            break;
        }
        case 7: {
            int id;
            cout << "  Task ID to delete: ";
            cin >> id;
            manager.deleteTask(id);
            break;
        }
        case 8:
            manager.saveToFile();
            break;
        case 9:
            manager.showSummary();
            break;
        case 0:
            manager.saveToFile();
            cout << "\n  Goodbye!\n\n";
            break;
        default:
            cout << "  Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}