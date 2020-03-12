//
// Created by LEI XU on 3/6/20.
//

#include "hikari.h"
#include "parser.h"
#include "parser.cpp"

using namespace hikari;

int main(int argc, char *argv[]){

    std::vector<std::string> filenames;
    // Process command-line arguments
    for (int i = 1; i < argc; ++i) {
            filenames.push_back(argv[i]);
    }
    if (filenames.empty()) {
        // Parse scene from standard input
        hikariParseFile("-");
    } else {
        // Parse scene from input files
        for (const std::string &f : filenames)
            hikariParseFile(f);
    }
    return 0;
}