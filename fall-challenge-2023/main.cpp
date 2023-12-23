#include <algorithm>
#include <climits>
#include <cmath>
#include <float.h>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>


#include "utils/debug.h"

using namespace std;

const static int WIDTH = 10000;
const static int MAX_TURN = 200;

const static int SCAN_RAD = 800;
const static int BIGSCAN_RAD = 2000;

const static int SCAN_COST = -1;
const static int BIGSCAN_COST = 5;

const static int SCAN_CAPACITY = 30;
const int CREATURES_COUNT = 12;
int MIN_CREATURES_ID = INT_MAX;

const int TYPE_0_Y = 7500/2;
const int TYPE_1_Y = 12500/2;
const int TYPE_2_Y = 17500/2;
const int HABITAT_WIDTH = 2500;

int TARGETS_YT[3] = {TYPE_0_Y, TYPE_1_Y, TYPE_2_Y};

using point = pair<int, int>;


unordered_map<string,point> dirs = {{"TL",{-1,-1}}, {"TR",{1,-1}}, {"BL",{-1,1}}, {"BR",{-1,0}}};

bool zones[3] = {false, false, false};


int distance(const point& a,const point& b){
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}
int score[5] = {1,2,3,3,4};


// overide operator - for point
point operator-(const point& a, const point& b){
    return {a.first - b.first, a.second - b.second};
}
point operator+(const point& a, const point& b){
    return {a.first + b.first, a.second + b.second};
}
point operator*(const point& a, const int& b){
    return {a.first * b, a.second * b};
}

int nbDrones = 2;
vector<vector<string>> radar;

set<int> myDrones;
set<int> foeDrones;

struct creature{
    int id;
    int color;
    int type;
    point pos;
    point vel;
    bool dead = false;
    bool visible = false;
    creature(int id, int color, int type, point pos, point vel):id(id), color(color), type(type), pos(pos), vel(vel){}
};

struct Drone{
    int id;
    point pos;
    int emergency;
    int battery;
    Drone(int id, point pos, int emergency, int battery):id(id), pos(pos), emergency(emergency), battery(battery){}
    
    int scanCount = 0;
    set<int> scannedCreatures;

    int targetZone = -1;
    int target = -1;
    int cooldown = 0;

    void tick();

};

vector<creature*> creatures;
Drone* drones[10] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
int visible_creature_count;
vector<int> scanningCreatures; 

int main()
{
    if(INPUT_ON) freopen("./utils/logs/input.txt", "r", stdin);

    //////////////////
/**
 * Score points by scanning valuable fish faster than your opponent.
 **/

    

 

    // divide the map into tiles to explore tiles of 300x300
    int tile_size = 300;
    int tile_count = WIDTH / tile_size ;
    vector<vector<bool>> tiles(tile_count, vector<bool>(tile_count, 0));



    int creature_count;
    cin >> creature_count; cin.ignore();
    for (int i = 0; i < creature_count; i++) {
        int creature_id;
        int color;
        int type;
        cin >> creature_id >> color >> type; cin.ignore();
        if(creatures.empty()){
            MIN_CREATURES_ID = creature_id;
            creatures = vector<creature*>(MIN_CREATURES_ID, nullptr);
            scanningCreatures = vector<int>(CREATURES_COUNT + MIN_CREATURES_ID, 0);
            radar = vector<vector<string>>(nbDrones*2, vector<string>((CREATURES_COUNT+MIN_CREATURES_ID)*nbDrones, "BR"));
        }
        creatures.push_back(new creature(creature_id, color, type, {0,0}, {0,0}));
    }

    
    int turn = 0;
    cerr << "game begin"<<endl;
   // game loop
    while (1) {
        // time loop in millisecond
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    

        turn++;
        int my_score;
        cin >> my_score; cin.ignore();
        int foe_score;
        cin >> foe_score; cin.ignore();
        int my_scan_count;
        cin >> my_scan_count; cin.ignore();

        for(auto& creature : creatures){
            if(creature == nullptr) continue;
            creature->dead = true;
            creature->visible = false;
        }


        for (int i = 0; i < my_scan_count; i++) {
            int creature_id;
            cin >> creature_id; cin.ignore();
        }
        int foe_scan_count;
        cin >> foe_scan_count; cin.ignore();
        for (int i = 0; i < foe_scan_count; i++) {
            int creature_id;
            cin >> creature_id; cin.ignore();
        }
        int my_drone_count;
        cin >> my_drone_count; cin.ignore();

        for (int i = 0; i < my_drone_count; i++) {
            int drone_id;
            int drone_x;
            int drone_y;
            int emergency;
            int battery;

            cin >> drone_id >> drone_x >> drone_y >> emergency >> battery; cin.ignore();
            myDrones.insert(drone_id);
            if(drones[drone_id] == nullptr){
                drones[drone_id] = new Drone(drone_id, {drone_x, drone_y}, emergency, battery);
            }
            else{
                drones[drone_id]->pos = {drone_x, drone_y};
                drones[drone_id]->emergency = emergency;
                drones[drone_id]->battery = battery;
            }
        }
        int foe_drone_count;
        cin >> foe_drone_count; cin.ignore();
        for (int i = 0; i < foe_drone_count; i++) {
            int drone_id;
            int drone_x;
            int drone_y;
            int emergency;
            int battery;
            cin >> drone_id >> drone_x >> drone_y >> emergency >> battery; cin.ignore();
            foeDrones.insert(drone_id);
            if(drones[drone_id] == nullptr){
                drones[drone_id] = new Drone(drone_id, {drone_x, drone_y}, emergency, battery);
            }
            else{
                drones[drone_id]->pos = {drone_x, drone_y};
                drones[drone_id]->emergency = emergency;
                drones[drone_id]->battery = battery;
            }
        }

        int drone_scan_count;
        cin >> drone_scan_count; cin.ignore();
        for (int i = 0; i < drone_scan_count; i++) {
            int drone_id;
            int creature_id;
            cin >> drone_id >> creature_id; cin.ignore();
            drones[drone_id]->scannedCreatures.insert(creature_id);
            scanningCreatures[creature_id] = 2;
        }

        cin >> visible_creature_count; cin.ignore();
        cerr << "visible creatures " << visible_creature_count <<endl;
        for (int i = 0; i < visible_creature_count; i++) {
            int creature_id;
            int creature_x;
            int creature_y;
            int creature_vx;
            int creature_vy;
            cin >> creature_id >> creature_x >> creature_y >> creature_vx >> creature_vy; cin.ignore();

            creatures[creature_id]->pos = {creature_x, creature_y};
            creatures[creature_id]->vel = {creature_vx, creature_vy};
            creatures[creature_id]->visible = true;
            creatures[creature_id]->dead = false;
        }
        int radar_blip_count;
        cin >> radar_blip_count; cin.ignore();
        cerr << "blips " << radar_blip_count << " " << radar.size() << " " << radar[0].size() << " " << creatures.size() << endl;

        for (int i = 0; i < radar_blip_count; i++) {
            int drone_id;
            int creature_id;
            string loc;
            cin >> drone_id >> creature_id >> loc; cin.ignore();
            radar[drone_id][creature_id] = loc;
            creatures[creature_id]->dead = false;
        }
        cerr << "clear creat " <<endl;

        for(int i = 0; i < (int) creatures.size();i++){
            if(!creatures[i])continue;
            if(!creatures[i]->dead)continue;
            scanningCreatures[creatures[i]->id] = -1;
            delete creatures[i];
            creatures[i] = nullptr;
        }
        cerr << "tickin " <<endl;

        for (auto& i : myDrones){
            cerr << "tick -- " << drones[i]->id << endl;
            drones[i]->tick();
            cerr << "tick done..."<<endl;
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cerr << "turn " << turn << " took " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;
    }
}


void Drone::tick(){

    // Behavior tree
    // Do we have a target ? 
    //      Yes -> Is it in range ?
    //          Yes -> Scan
    //          No -> Move
    //      No -> Target next creature in list type 0 first then 1 then 2
    //          Is it in range ?
    //              Yes -> Scan
    //              No -> Move

    // ove means get in range of the target > 800units or it get scared 

    // if no target left
       // do i have to surface?
            // yes -> surface
            // no -> scare the fishes
    cerr << "tick " << id << endl;
    cooldown--;

    if(target>-1 && (scanningCreatures[target] == 2 || !creatures[target]) ){
        target = -1;
    }
    if(target < 0){
        // find a target

        for(auto creature : creatures){
            if(!creature)continue;
            if(creature->dead)continue;
            if(scanningCreatures[creature->id] == 0){
                // target found 
                cerr << "target found for drone id : " << id << " " << creature->id << endl;
                target = creature->id;
                scanningCreatures[target] = 1;
                break;
            }
        }
    }

    // no target left
    if(target <0){
        // do i have to surface?
        if(scannedCreatures.empty()){
            // do nothing
            cout << "WAIT 0 😴" << endl;
            return;
        }

        // surface
        // move towards the surface y < 500
        bool shouldBigScan = pos.second > TARGETS_YT[0] && (battery >= BIGSCAN_COST) && (cooldown-- <= 0);
        cout << "MOVE" << " " << pos.first << " " << 499;
        if(shouldBigScan){
            cooldown = 3;
            cout << " 1 | ☀️ ";
        }else{
            cout << " 0 | ☀️ ";
        }
        cout << endl;
        return;
    }

    // target in range
    // move toward target but stay within BISCAN_RAD and SCAN_RAD
    // so the target pos should be target + norm(current - pos)*scan_rad
    
    // check if any visible fishes visible and not tageted become the target
    if(!creatures[target]->visible && visible_creature_count>0){
        for(auto creature : creatures){
            if(!creature)continue;
            if(creature->dead)continue;

            if(creature->visible && scanningCreatures[creature->id] > -1 && scanningCreatures[creature->id] < 2){
                if(scanningCreatures[target] == 1){
                    for(auto& i : myDrones){
                        if(drones[i]->id == id)continue; 
                        if(drones[i]->target == target){
                            drones[i]->target = -1;
                        }
                    }
                }
                scanningCreatures[target] = 0;
                target = creature->id;
                scanningCreatures[target] = 1;
                break;
            }
        }

    }

    if(target < 0) cerr << "ERROOR" <<endl;
    if(creatures[target]->visible){
        // scan
        point targetPos = creatures[target]->pos + (creatures[target]->pos - pos)* (SCAN_RAD / distance(pos, creatures[target]->pos));
        cout << "MOVE" << " " << targetPos.first << " " << targetPos.second;

        bool shouldBigScan = distance(pos, creatures[target]->pos) < BIGSCAN_RAD && (battery >= BIGSCAN_COST) && (cooldown-- <= 0);
        if(shouldBigScan){
            cooldown = 3;
            cout << " 1 | 📡 " << creatures[target]->id;
        }else{
            cout << " 0 | 🕵️ " << creatures[target]->id;
        }
        cout << endl;
        return;
    }
    // target not in range move towards it

    string loc = radar[id][target];
    targetZone = creatures[target]->type;
    bool inYRange = abs(pos.second - TARGETS_YT[targetZone]) < SCAN_RAD;
    bool shouldBigScan = inYRange && (battery >= BIGSCAN_COST) && (cooldown <= 0);

    cout << "MOVE" << " " << pos.first + (loc.back()=='R'?1:-1)*600 << " " << TARGETS_YT[targetZone];
    if(shouldBigScan){
        cooldown = 3;
        cout << " 1 | 🏃‍♀️ " << creatures[target]->id << " " << creatures[target]->pos.first << " " << creatures[target]->pos.second << " " << loc << " " << cooldown;
    }else{
        cout << " 0 | 🏃‍♀️ " << creatures[target]->id << " " << creatures[target]->pos.first << " " << creatures[target]->pos.second << " " << loc << " " << cooldown;
    }
    cout << endl;
    return;
}
