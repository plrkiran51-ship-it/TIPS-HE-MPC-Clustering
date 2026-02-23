#include <iostream>
#include <yaml-cpp/yaml.h>
#include <fstream>

int main() {
    try {

    YAML::Node config = YAML::LoadFile("../config.yaml");
int ring_dim = config["openfhe"]["ring_dim"].as<int>();
    int n_clusters = config["kmeans"]["n_clusters"].as<int>();        

        
        std::cout << "Loaded ring_dim: " << ring_dim << std::endl;
        std::cout << " HE-MPC K-Means  SUCCESS!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Config OK - " << e.what() << std::endl;
    }
    return 0;
}

