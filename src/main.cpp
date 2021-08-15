#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

#define NUM_OF_TRAINS (3)

class BinaryFlag {
    mutex dataAccess;
public:
    void occupy() {
        dataAccess.lock();
    }

    void release() {
        dataAccess.unlock();
    }
};

static string GetStringFromStream(const string &message) {
    std::string str;
    while (true) {
        cout << message << endl;
        while (str.empty()) {
            getline(cin, str);
        }
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return str;
}

static int GetIntFromStream(const string &message, int min, int max) {
    int num;
    bool isCorrect = false;
    while (!isCorrect) {
        cout << message << ":";
        isCorrect = (!!(cin >> num)) && num >= min && num <= max;
        if (!isCorrect) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error! Please enter correct number" << endl;
        }
    }
    return num;
}

static void train(const string &name, int timeInWay, BinaryFlag *railwayPlace) {
    assert(nullptr != railwayPlace);

    cout << "Train " << name << " has started the movement" << endl;
    this_thread::sleep_for(chrono::seconds(timeInWay));
    cout << "Train " << name << " is waiting for the place..." << endl;
    railwayPlace->occupy();
    cout << "Train " << name << " has arrived to the railway station" << endl;

    string cmd;
    while (cmd != "depart") {
        cmd = GetStringFromStream("Enter 'depart' to leave the station: ");
    }
    cout << "Train " << name << " has leaved the railway station" << endl;
    railwayPlace->release();
}

int main() {
    const string trainNames[NUM_OF_TRAINS] = {"Train A", "Train B", "Train C"};
    thread *calls[NUM_OF_TRAINS];
    BinaryFlag railwayPlace;

    int timesInWay[NUM_OF_TRAINS];

    for (int i = 0; i < NUM_OF_TRAINS; ++i) {
        timesInWay[i] = GetIntFromStream(string(trainNames[i] + ". Enter the time in the way in s (1 - 20)"),
                                         1,
                                         20);

    }

    for (int i = 0; i < NUM_OF_TRAINS; ++i) {
        calls[i] = new thread(train, trainNames[i], timesInWay[i], &railwayPlace);
    }

    for (auto call : calls) {
        call->join();
        delete call;
    }

    return 0;
}
