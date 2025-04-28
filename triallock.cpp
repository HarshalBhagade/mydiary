#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/stat.h>
#include <filesystem>
using namespace std;

int lock(){
  
    const char* foldername = "/home/impulse/cmpt130/ggg";
    
    // Use chmod to set permissions to 000 (no permissions)
    int result = chmod(foldername, 0000);
    
    if (result == 0) {
        std::cout << "Permissions changed successfully." << endl;
    } else {
        std::cerr << "Error changing permissions: " << strerror(errno) << endl;
    }

    // Rename the folder by adding a dot (.) at the beginning of its name
    const char* newFoldername = "/home/impulse/cmpt130/.ggg";

    if (std::rename(foldername, newFoldername) == 0) {
        std::cout << "Folder hidden successfully." << endl;
    } else {
        std::cerr << "Error hiding folder: " << strerror(errno) << endl;
    }

    return 0;
}

int unlock(){

    const char* foldername = "/home/impulse/cmpt130/.ggg";
    
    // Use chmod to set permissions to 000 (no permissions)
    int result = chmod(foldername, 7000);
    
    if (result == 0) {
        std::cout << "Permissions changed successfully." << endl;
    } else {
        std::cerr << "Error changing permissions: " << strerror(errno) << endl;
    }

    return 0;
}

int main(){

    lock();

    return 0;
}