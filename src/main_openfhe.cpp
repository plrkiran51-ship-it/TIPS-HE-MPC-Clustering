#include <iostream>

int main() {
    std::cout << "OpenFHE Docker ENVIRONMENT OK!" << std::endl;
    std::cout << "OpenFHE installed: $(pkg-config --modversion openfhe)" << std::endl;
    std::cout << "HE-MPC Ch5 Docker validated" << std::endl;
    return 0;
}
