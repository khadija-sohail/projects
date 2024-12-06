#include <iostream>
#include <fstream>
#include <string>
#include <climits> // For INT_MAX
using namespace std;

class Graph {
    struct Road {
        char intersection;
        int length;
        Road* next;
    };

    struct Intersection {
        char intersection;
        Road* head;
    };

    Intersection* adjLists;
    int intersectionCount;
    int capacity;

    int findIndex(char intersection) const {
        for (int i = 0; i < intersectionCount; ++i) {
            if (adjLists[i].intersection == intersection)
                return i;
        }
        return -1;
    }

    Road* createRoad(char intersection, int length) {
        Road* newRoad = new Road;
        newRoad->intersection = intersection;
        newRoad->length = length;
        newRoad->next = nullptr;
        return newRoad;
    }

    void resizeIfNeeded() {
        if (intersectionCount == capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            Intersection* newAdjLists = new Intersection[capacity];
            for (int i = 0; i < intersectionCount; ++i) {
                newAdjLists[i] = adjLists[i];
            }
            delete[] adjLists;
            adjLists = newAdjLists;
        }
    }

public:
    Graph() : adjLists(nullptr), intersectionCount(0), capacity(0) {}

    ~Graph() {
        for (int i = 0; i < intersectionCount; ++i) {
            Road* current = adjLists[i].head;
            while (current) {
                Road* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] adjLists;
    }

    void addIntersection(char intersection) {
        if (findIndex(intersection) == -1) {
            resizeIfNeeded();
            adjLists[intersectionCount].intersection = intersection;
            adjLists[intersectionCount].head = nullptr;
            intersectionCount++;
        }
    }

    void addRoad(char from, char to, int length) {
        int fromIndex = findIndex(from);
        if (fromIndex == -1) {
            addIntersection(from);
            fromIndex = findIndex(from);
        }

        Road* newRoad = createRoad(to, length);
        newRoad->next = adjLists[fromIndex].head;
        adjLists[fromIndex].head = newRoad;
    }

    void displayGraph() const {
        for (int i = 0; i < intersectionCount; ++i) {
            cout << "Intersection " << adjLists[i].intersection << ": ";
            Road* temp = adjLists[i].head;
            while (temp) {
                cout << "-> (To: " << temp->intersection << ", Travel Time: " << temp->length << " mins) ";
                temp = temp->next;
            }
            cout << endl;
        }
    }

    int getIntersectionCount() const {
        return intersectionCount;
    }

    const Intersection* getAdjLists() const {
        return adjLists;
    }

    int getIndex(char intersection) const {
        return findIndex(intersection);
    }
};

class Vehicle {
private:
    string id;
    char start;
    char end;

public:
    Vehicle() : id(""), start('\0'), end('\0') {}
    Vehicle(const string& vehicleId, char startIntersection, char endIntersection)
        : id(vehicleId), start(startIntersection), end(endIntersection) {}

    string getId() const { return id; }
    char getStart() const { return start; }
    char getEnd() const { return end; }

    void print() const {
        cout << "Vehicle ID: " << id
             << ", Start: " << start
             << ", End: " << end << endl;
    }
};

Vehicle* readVehicles(const string& filename, int& count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        count = 0;
        return nullptr;
    }

    string line;
    getline(file, line);

    int capacity = 10;
    Vehicle* vehicles = new Vehicle[capacity];
    count = 0;

    while (getline(file, line)) {
        if (count == capacity) {
            capacity *= 2;
            Vehicle* temp = new Vehicle[capacity];
            for (int i = 0; i < count; i++) {
                temp[i] = vehicles[i];
            }
            delete[] vehicles;
            vehicles = temp;
        }

        size_t firstComma = line.find(',');
        size_t lastComma = line.rfind(',');

        string vehicleId = line.substr(0, firstComma);
        char startIntersection = line[firstComma + 1];
        char endIntersection = line[lastComma + 1];

        vehicles[count++] = Vehicle(vehicleId, startIntersection, endIntersection);
    }

    file.close();
    return vehicles;
}

void printVehicles(const Vehicle* vehicles, int count) {
    for (int i = 0; i < count; i++) {
        vehicles[i].print();
    }
}

void readCSVAndBuildGraph(const string& filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip the header

    while (getline(file, line)) {
        char from = line[0];
        char to = line[2];
        int travelTime = 0;

        for (int i = 4; line[i] != '\0'; ++i) {
            travelTime = travelTime * 10 + (line[i] - '0');
        }

        graph.addIntersection(from);
        graph.addIntersection(to);
        graph.addRoad(from, to, travelTime);
    }

    file.close();
}

int main() {
    Graph roadMap;
    string filename = "C:\\Users\\HP\\Documents\\DS_PROJECT\\road_network.csv"; // Replace with your CSV file path
    readCSVAndBuildGraph(filename, roadMap);

    cout << "Road Map:" << endl;
    roadMap.displayGraph();

    int vehicleCount = 0;
    Vehicle* vehicles = readVehicles("C:\\Users\\HP\\Documents\\DS_PROJECT\\vehicles.csv", vehicleCount);

    if (vehicles) {
        cout << "\nVehicles List:" << endl;
        printVehicles(vehicles, vehicleCount);

        delete[] vehicles;
    }

    return 0;
}
