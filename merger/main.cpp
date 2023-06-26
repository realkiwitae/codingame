#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <vector>
#include <algorithm>
#include <regex>

namespace fs = std::experimental::filesystem;

std::regex classRegex(R"(class\s+(\w+))");

std::string workDir = ".";
std::string outputFile = "merged";
std::string excludeDir = "generated";
std::string mainFile = "";
bool mainIsProcessed = false;
std::vector<std::string> processOnce;

void processDir(const std::string& dir);
void processFile(const std::string& file, bool include);

void wipeFile(const std::string& filePath) {
    std::ofstream file(filePath);
    if (file)file.close();
}

void processDir(const std::string& dir) {
    for (const auto& entry : fs::directory_iterator(dir)) {
        const std::string& node = entry.path().filename().string();
        const std::string& fullPath = entry.path().string();
        if (fs::is_directory(node) && node != excludeDir) {
            processDir(fullPath);
        } else{
            processFile(fullPath, false);
        }
    }
}

void processFile(const std::string& file, bool include) {
    if (file == mainFile && mainIsProcessed)return;
    if (fs::path(file).extension() != ".cpp" && !include)return;
    
    if (fs::path(file).extension() == ".cpp") {
        std::cout << "Processing " << file << std::endl;
        std::ofstream output(outputFile, std::ios::app);
        if (output) {
            output << "/*-- File: " << file << " start --*/\n";
            output.close();
        }
    } else if (fs::path(file).extension() == ".hpp" || fs::path(file).extension() == ".h") {
        std::cout << "Including: " << file << std::endl;
        std::ofstream output(outputFile, std::ios::app);
        if (output) {
            output << "/*-- #include \"" << file << "\" start --*/\n";
            output.close();
        }
    } else {
        return;
    }

    bool processedOnce = false;
    for (const std::string& processedFile : processOnce) {
        if (processedFile == file) {
            processedOnce = true;
            break;
        }
    }

    if (!processedOnce) {
        std::ifstream inputFile(file);
        if (inputFile) {
            std::string line;
            std::ofstream output(outputFile, std::ios::app);
            if (output) {
                while (std::getline(inputFile, line)) {
                    if (line.find("#include \"") != std::string::npos) {
                        std::string includedFile = line.substr(line.find("\"") + 1, line.rfind("\"") - line.find("\"") - 1);
                        std::string includedFilePath = fs::path(file).parent_path().string() + "/" + includedFile;
                        processFile(includedFilePath, true);
                    } else if (line.find("#pragma once") != std::string::npos) {
                        processOnce.push_back(file);
                    } else {
                        output << line << "\n";
                    }
                }
                output.close();
            }
            inputFile.close();
        }
    }

    std::ofstream output(outputFile, std::ios::app);
    if (output) {
        if (fs::path(file).extension() == ".cpp") {
            output << "/*-- File: " << file << " end --*/\n";
        } else if (fs::path(file).extension() == ".hpp") {
            output << "/*-- #include \"" << file << "\" end --*/\n";
        }
        output.close();
    }
}

// use arguments to set workDir, outputFile, excludeDir, mainFile

int main( int argc, char* argv[]) {
    


    // give usage info if not enough arguments to set mainFile
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " workDir outputFile excludeDir mainFile" << std::endl;
        return 1;
    }

    workDir = argv[1];
    outputFile = argv[2];
    excludeDir = argv[3];
    mainFile = argv[4];
    
    fs::create_directories(fs::path(outputFile).parent_path());

    wipeFile(outputFile);


    if (!mainFile.empty()) {
        processFile(mainFile, false);
        mainIsProcessed = true;
    }

    processDir(workDir);

    // store all the includes in outputFile at the top of the file remove duplicates
    std::ifstream inputFile(outputFile);
    if (inputFile) {
        std::string line;
        std::vector<std::string> includes;
        std::vector<std::string> namespaces;
        std::vector<std::string> classes;
        std::vector<std::string> others;
        while (std::getline(inputFile, line)) {
            if (line.find("#include") != std::string::npos) {
                includes.push_back(line);
            }else if(line.find("namespace") != std::string::npos){
                namespaces.push_back(line);
            }else{
                others.push_back(line);
                std::smatch match;
                if (std::regex_search(line, match, classRegex)) {
                    classes.push_back(match.str(1));
                }
            }
        }
        inputFile.close();

        std::sort(includes.begin(), includes.end());
        includes.erase(std::unique(includes.begin(), includes.end()), includes.end());
        std::sort(namespaces.begin(), namespaces.end());
        namespaces.erase(std::unique(namespaces.begin(), namespaces.end()), namespaces.end());
        
        std::ofstream output(outputFile);
        if (output) {
            for (const std::string& include : includes) {
                output << include << std::endl;
            }
            output << std::endl;
            for (const std::string& n : namespaces) {
                output << n << std::endl;
            }
            output << std::endl;
            for (const std::string& c : classes) {
                output << "class " << c << ";" << std::endl;
            }
            output << std::endl;
            for (const std::string& other : others) {
                output << other << std::endl;
            }
            output.close();
        }
    }  

    return 0;
}

