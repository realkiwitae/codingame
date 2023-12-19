#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <float.h>
#include <unordered_map>
#include <set>

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

vector<vector<string>> radar(5, vector<string>(CREATURES_COUNT, "BR"));


int nbDrones = 2;

set<int> myDrones;
set<int> foeDrones;

int crea2ID(int id){return id - nbDrones;}

enum class EAction{
    SCAN,
    BIGSCAN,
    MOVE,
    WAIT
};

enum class FSM{
    IDLE,
    CLEAR_ZONE,
    SURFACE,
};


struct creature{
    int id;
    int color;
    int type;
    point pos;
    point vel;
    int lastSeen = -1;
    creature(int id, int color, int type, point pos, point vel):id(id), color(color), type(type), pos(pos), vel(vel){}
};

struct Drone{
    int id;
    point pos;
    int emergency;
    int battery;
    Drone(int id, point pos, int emergency, int battery):id(id), pos(pos), emergency(emergency), battery(battery){}
    
    FSM state = FSM::IDLE;
    int scanCount = 0;
    set<int> scannedCreatures;

    int targetZone = -1;
    creature* target = nullptr;
    int cooldown = 0;

    void updateState();
    void turn();

};

vector<creature> creatures;
Drone* drones[10] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

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
        creatures.push_back(creature(creature_id, color, type, {0,0}, {0,0}));
    }

    
    int turn = 0;


    // game loop
    while (1) {
        turn++;
        int my_score;
        cin >> my_score; cin.ignore();
        int foe_score;
        cin >> foe_score; cin.ignore();
        int my_scan_count;
        cin >> my_scan_count; cin.ignore();
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
        }
        int visible_creature_count;
        cin >> visible_creature_count; cin.ignore();
        for (int i = 0; i < visible_creature_count; i++) {
            int creature_id;
            int creature_x;
            int creature_y;
            int creature_vx;
            int creature_vy;
            cin >> creature_id >> creature_x >> creature_y >> creature_vx >> creature_vy; cin.ignore();

            creatures[creature_id].pos = {creature_x, creature_y};
            creatures[creature_id].vel = {creature_vx, creature_vy};
            creatures[creature_id].lastSeen = turn;
        }
        int radar_blip_count;
        cin >> radar_blip_count; cin.ignore();
        for (int i = 0; i < radar_blip_count; i++) {
            int drone_id;
            int creature_id;
            string loc;
            cin >> drone_id >> creature_id >> loc; cin.ignore();
            radar[drone_id][crea2ID(creature_id)] = loc;
        }


        for (auto& i : myDrones){
            drones[i]->updateState();
            drones[i]->turn();
        }
    }
}


void Drone::updateState(){
    logger("updateState of drone", id);
    if(state == FSM::IDLE){
        for(int i = 0; i < 3;i++){
            if(zones[i] == false){
                state = FSM::CLEAR_ZONE;
                targetZone = i;
                logger("change to CLEARZONE", id , targetZone);
                return;
            }
        }
    }
    if(state == FSM::CLEAR_ZONE){
        for(creature& c : creatures){
            if(c.type == targetZone && !scannedCreatures.count(c.id)){
                return;
            }
        }
        zones[targetZone] = true;
        state = FSM::SURFACE;
        logger("change to SURFACE", id , targetZone);

        return;
    }

    if(state == FSM::SURFACE){
        if(pos.second <= 500){
            state = FSM::IDLE;
            logger("change to IDLE", id , targetZone);
            updateState();
            return;
        }
    }
    

}

void Drone::turn(){
    logger("turn of drone");
    if(state == FSM::IDLE){
        cout << "WAIT 0" << endl;
        return;
    }

    if(state == FSM::SURFACE){
        cout << "MOVE" << " " << pos.first << " " << 499 << " " << 0 << " ☀️" << endl;
        return;
    }

    if(state == FSM::CLEAR_ZONE){
        if(target == nullptr){
            for(creature& c : creatures){
                if(c.type == targetZone && !scannedCreatures.count(c.id)){
                    target = &c;
                    logger("found target");
                    break;
                }
            }
            if(!target){
                updateState();
                return;
            }else{
                turn();
            }
        }
        else{
            // use radar info
            if(scannedCreatures.count(target->id)){
                target = nullptr;
                turn();
                return;
            }

            string loc = radar[id][crea2ID(target->id)];
            bool inYRange = abs(pos.second - TARGETS_YT[targetZone]) < SCAN_RAD;
            bool shouldBigScan = inYRange && (battery >= BIGSCAN_COST) && (cooldown-- <= 0);
            if(shouldBigScan){
                cooldown = 3;
            }

            cout << "MOVE" << " " << pos.first + (loc.back()=='R'?1:-1)*600 << " " << TARGETS_YT[targetZone] << " " << (shouldBigScan?1:0) << " 🕵️ "<<target->id << endl;
            return;
        }
    }

}