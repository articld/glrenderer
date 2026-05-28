#pragma once
#include <string>
#include <filesystem>
#include <deque>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

class Directory {
    private:
        std::string directory;

        std::deque<std::string> files;
        std::deque<std::string>::iterator iterator;

        std::fstream rendered_elements_file;
        std::unordered_set<std::string> rendered_elements;

        bool SetDirectory(std::string path);
        void LoadRenderedFiles();

    public:
        Directory(std::string path);
        ~Directory() {
            if (!rendered_elements_file.is_open())
                rendered_elements_file.close();
        }

        std::string GetItem();
        inline unsigned int GetTotalElements() const { return files.size(); }
};