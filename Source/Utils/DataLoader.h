#pragma once

#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "../Components/Graph.h"
#include "../Components/Architecture.h"

namespace parser{
    static Graph *LoadGraph(const std::string&DotFile){

        std::ifstream file(DotFile.c_str());
        if(!file.is_open()){
            std::cerr << "Could not open " << DotFile << ".\n";
            return nullptr;
        }

        Graph *G = new Graph;
        std::map<std::string,Node*> NodeMap;
        int NumNodes = 0;

        std::regex pattern("(\\w+)\\s*->\\s*(\\w+)");
        std::string row;

        while(std::getline(file, row)){

            std::smatch rmatch;
            if(regex_search(row, rmatch, pattern)){
                
                std::string from = rmatch[1].str();
                std::string to   = rmatch[2].str();
            
                Node *source, *target;
                auto srcIt = NodeMap.find(from);
                auto tarIt = NodeMap.find(to);

                if(srcIt == NodeMap.end()){
                    source = new Node(NumNodes++);
                    NodeMap[from] = source;
                    G->AddNode(source);
                } else{
                    source = srcIt->second;
                }

                if(tarIt == NodeMap.end()){
                    target = new Node(NumNodes++);
                    NodeMap[to] = target;
                    G->AddNode(target);
                } else{
                    target = tarIt->second;
                }
                G->AddEdge(source, target);
            }
        }
        file.close();

        return G;
    }   

    static Config *LoadConfig(const std::string&JsonFile){
        
        std::ifstream file(JsonFile);

        if(!file.is_open()){
            std::cerr << "Could not open " << JsonFile << ".\n";
            return nullptr;
        }

        Json::Value root;
        file >> root;

        int num_pes = root["num_pes"].asInt();

        std::vector<std::vector<int>> input(num_pes);
        std::vector<std::vector<int>> output(num_pes);
        std::vector<int> PeDeg(num_pes);
        
        for (int i = 0; i < num_pes; i++) {
            std::string peName  = "PE"+std::to_string(i);
            Json::Value pe = root[peName];

            int insize  = pe["input"].size();
            int outsize = pe["output"].size();
            
            for(int j=0; j<insize; j++){
                input[i].push_back( pe["input"][j].asInt() );
                PeDeg[i]++;
            }

            for(int j=0; j<outsize; j++){
                output[i].push_back( pe["output"][j].asInt() );
                PeDeg[i]++;
            }
        }

        Config *port = new Config;
        port->input = input;
        port->output = output;
        port->degree = PeDeg;
        port->pes = num_pes;

        file.close();

        return port;
    }
}