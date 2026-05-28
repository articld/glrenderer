#include "Directory.h"

Directory::Directory(std::string path) {
    if (!SetDirectory(path))
        std::cerr << "Impossibile utilizzare il path " << path;
    rendered_elements_file.open(path + "/rendered_files.txt");

    if (!rendered_elements_file.is_open() && !rendered_elements_file.good())
        std::cerr << "Impossibile aprire i file dei progressi" << std::endl;
    else
        LoadRenderedFiles();

    fs::path fs_directory(directory);
    for (fs::recursive_directory_iterator it(fs_directory), end; it != end; ++it) {
        if (it->is_regular_file()) {
            if (it->path().extension().string() == ".glb") {
                files.push_back(it->path().filename().string());
            }
        }
    }
    iterator = files.begin();
}

void Directory::LoadRenderedFiles() {
    while (!rendered_elements_file.eof()) {
        std::string line;
        std::getline(rendered_elements_file, line);
        rendered_elements.insert(line);
    }
}

bool Directory::SetDirectory(std::string path) {
    if (path.length() >= 0) {
        directory = path;
        return true;
    } else
        return false;
}

//Ritorna il nome del file da renderizzare. Ritorna "EOF" se il vettore è finito
std::string Directory::GetItem() {
    if (iterator == files.end())
        return "EOF";

    while (iterator != files.end() && rendered_elements.find(*iterator) != rendered_elements.end())
        ++iterator;

    if (iterator == files.end())
        return "EOF";

    std::string item = *iterator;
    ++iterator;
    return item;
}