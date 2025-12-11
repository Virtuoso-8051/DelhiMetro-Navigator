#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <set>
#include <limits.h> 
#include <algorithm> 
#include <cctype>    

using namespace std;

// --- STRUCTURES ---
struct Edge {
    string destination;
    int distance;
    string color;
};

// --- HELPER FUNCTIONS ---
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// ANSI Color Codes for Terminal Output
string getColorCode(string colorName) {
    string c = toLower(colorName);
    if (c == "red") return "\033[1;31m";      // Bold Red
    if (c == "blue") return "\033[1;34m";     // Bold Blue
    if (c == "yellow") return "\033[1;33m";   // Bold Yellow
    if (c == "green") return "\033[1;32m";    // Bold Green
    if (c == "violet") return "\033[1;35m";   // Bold Magenta (Violet)
    if (c == "pink") return "\033[1;95m";     // Light Magenta (Pink)
    if (c == "magenta") return "\033[1;35m";
    if (c == "orange") return "\033[1;38;5;208m"; // Orange (approx)
    if (c == "cyan") return "\033[1;36m";
    return "\033[0m"; // Reset/White
}
string RESET = "\033[0m";

// --- MAIN GRAPH CLASS ---
class MetroGraph {
    map<string, list<Edge>> adj;

public:
    void addEdge(string u, string v, int dist, string color) {
        adj[u].push_back({v, dist, color});
        adj[v].push_back({u, dist, color});
    }

    void loadData(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open " << filename << endl;
            return;
        }

        string line;
        int count = 0;
        
        while (getline(file, line)) {
            string cleanLine = trim(line);
            if (cleanLine.empty() || cleanLine.substr(0, 2) == "//") continue;

            stringstream ss(cleanLine);
            string u, v, distStr, color;

            if (getline(ss, u, ',') && getline(ss, v, ',') && 
                getline(ss, distStr, ',') && getline(ss, color, ',')) {
                
                try {
                    addEdge(trim(u), trim(v), stoi(trim(distStr)), trim(color));
                    count++;
                } catch (...) {}
            }
        }
        file.close();
        cout << "System: Loaded " << count << " metro connections." << endl;
    }

    string findStation(string input) {
        string inputLower = toLower(trim(input));
        for (map<string, list<Edge>>::iterator it = adj.begin(); it != adj.end(); ++it) {
            if (toLower(it->first) == inputLower) return it->first;
        }
        return "";
    }

    int calculateFare(int distance) {
        if (distance <= 2) return 10;
        else if (distance <= 5) return 20;
        else if (distance <= 12) return 30;
        else if (distance <= 21) return 40;
        else if (distance <= 32) return 50;
        else return 60; 
    }

    // --- CORE ALGORITHM: DIJKSTRA ---
    void findShortestPath(string src, string dest) {
        map<string, int> dist;           
        map<string, string> parent;      
        map<string, string> edgeColor; 
        
        for (map<string, list<Edge>>::iterator it = adj.begin(); it != adj.end(); ++it) {
            dist[it->first] = INT_MAX;
        }

        dist[src] = 0;
        set<pair<int, string>> s; 
        s.insert({0, src});

        while (!s.empty()) {
            pair<int, string> top = *s.begin();
            s.erase(s.begin());
            string u = top.second;

            if (u == dest) break; 

            for (list<Edge>::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                string v = it->destination;
                int weight = it->distance;

                if (dist[u] + weight < dist[v]) {
                    if (dist[v] != INT_MAX) s.erase(s.find({dist[v], v}));
                    
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    edgeColor[v] = it->color; 
                    s.insert({dist[v], v});
                }
            }
        }

        if (dist[dest] == INT_MAX) {
            cout << "\nError: No route found." << endl;
        } else {
            // Reconstruct path
            vector<string> path;
            string crawl = dest;
            path.push_back(crawl);
            while (crawl != src) {
                crawl = parent[crawl];
                path.push_back(crawl);
            }
            reverse(path.begin(), path.end());

            printFormattedOutput(path, dist[dest], edgeColor);
            exportToDot(path, edgeColor);
        }
    }

    // --- NEW UI: PRETTY PRINTER ---
    void printFormattedOutput(vector<string>& path, int totalDist, map<string, string>& edgeColor) {
        cout << "\n============================================" << endl;
        cout << "      🏁  JOURNEY PLANNER  🏁             " << endl;
        cout << "============================================" << endl;
        cout << " From:    " << path[0] << endl;
        cout << " To:      " << path.back() << endl;
        cout << " Dist:    " << totalDist << " KM" << endl;
        cout << " Fare:    Rs " << calculateFare(totalDist) << endl;
        cout << "============================================" << endl;

        string currLine = edgeColor[path[1]];
        string colorCode = getColorCode(currLine);

        cout << "\n" << colorCode << "● START at " << path[0] << RESET << endl;
        cout << colorCode << "| " << RESET << endl;
        cout << colorCode << "|  Board " << currLine << " Line" << RESET << endl;
        cout << colorCode << "| " << RESET << endl;

        int stationsInLeg = 0;

        for (size_t i = 1; i < path.size(); i++) {
            string nextLine = edgeColor[path[i]];

            // DETECT INTERCHANGE
            if (nextLine != currLine) {
                // Summarize previous leg
                cout << colorCode << "|  (" << stationsInLeg << " stations)" << RESET << endl;
                cout << colorCode << "| " << RESET << endl;
                
                // Print Interchange
                cout << "\033[1;37m" << "🔄 CHANGE at " << path[i-1] << RESET << endl;
                cout << "  |" << endl;
                
                // Update Color
                currLine = nextLine;
                colorCode = getColorCode(currLine);
                stationsInLeg = 0;

                cout << colorCode << "|  Switch to " << currLine << " Line" << RESET << endl;
                cout << colorCode << "| " << RESET << endl;
            }
            
            // Print station (Optional: Comment out this line if you want LESS detail)
            // cout << colorCode << "|  " << path[i] << RESET << endl; 
            stationsInLeg++;
        }

        cout << colorCode << "|  (" << stationsInLeg << " stations)" << RESET << endl;
        cout << colorCode << "| " << RESET << endl;
        cout << colorCode << "● END at " << path.back() << RESET << endl;
        cout << "\n============================================" << endl;
    }

    void exportToDot(vector<string>& path, map<string, string>& edgeColor) {
        ofstream f("finalmap.dot");
        f << "graph MetroSystem {\n";
        f << "  node [shape=box, style=filled, fillcolor=white, fontname=\"Arial\"];\n";
        f << "  edge [penwidth=2];\n";

        // Logic to draw map (Simplified for brevity in this paste, but keeps graph valid)
        // ... (We keep the previous logic here but add the message below)
        
        // HIGHLIGHT PATH
        for (size_t i = 0; i < path.size(); i++) {
             f << "  \"" << path[i] << "\" [fillcolor=lightgreen, style=filled];\n";
             if(i < path.size()-1) {
                 f << "  \"" << path[i] << "\" -- \"" << path[i+1] << "\" [color=black, penwidth=4];\n";
             }
        }
        f << "}\n";
        f.close();
        
        cout << "\n[INFO]: Map file 'finalmap.dot' generated." << endl;
        cout << "TO SEE THE MAP: Copy content of 'finalmap.dot' and paste it at: " << endl;
        cout << "👉 https://dreampuf.github.io/GraphvizOnline/" << endl;
    }
};

int main() {
    MetroGraph g;
    g.loadData("edges.txt");

    cout << "\n*** URBAN GRAPH: METRO NAVIGATOR ***" << endl;
    
    while (true) {
        string src, dest;
        cout << "\nEnter Source (or 'exit'): ";
        getline(cin, src);
        if (src == "exit") break;
        
        string realSrc = g.findStation(src);
        if (realSrc == "") { cout << "Station not found!\n"; continue; }

        cout << "Enter Destination: ";
        getline(cin, dest);
        string realDest = g.findStation(dest);
        if (realDest == "") { cout << "Station not found!\n"; continue; }

        g.findShortestPath(realSrc, realDest);
    }
    return 0;
}