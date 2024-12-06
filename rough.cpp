#include <iostream>
#include <fstream>
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

    Intersection* adjLists; // Pointer to dynamically allocated array of intersections
    int intersectionCount;  // Current count of intersections
    int capacity;           // Current capacity of the array

    int findIndex(char intersection) {
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
        // Double the capacity when the array is full
        if (intersectionCount == capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2; // Start with 1 if capacity is 0
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
        // Clean up dynamically allocated memory
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

    void displayGraph() {
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
};

void readCSVAndBuildGraph(const string& filename, Graph& graph) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip the header

    while (getline(file, line)) {
        char from = line[0];  // Intersection1 (First character)
        char to = line[2];    // Intersection2 (Third character)
        int travelTime = 0;

        // Read the travel time
        for (int i = 4; line[i] != '\0'; ++i) {
            travelTime = travelTime * 10 + (line[i] - '0'); // Convert char to int
        }

        // Add intersections to the graph
        graph.addIntersection(from);
        graph.addIntersection(to);

        // Add the road between the intersections
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

    return 0;
}
