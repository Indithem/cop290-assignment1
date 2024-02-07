#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class CSV_reader {
private:
    std::ifstream file;
    std::string line;

public:
    CSV_reader(std::string filename);
    ~CSV_reader();
    /*Gets all the `,` seperated values as strings from the file
    Returns an empty vector if there are no lines.*/
    std::vector<std::string> get_next_line();
};

class CSV_writer{
private:
    std::ofstream file;

public:
    CSV_writer(std::string filename);
    ~CSV_writer();
    /*Writes a `,` seperated line to the file*/
    template <typename T>
    void write_line(std::vector<T> line_data);
};

CSV_reader::CSV_reader(std::string filename){
    file.open(filename);
    if (!file.is_open()){
        std::cerr << "Error: file "<<filename<<" not found" << std::endl;
    }
}

CSV_reader::~CSV_reader(){
    file.close();
}

std::vector<std::string> CSV_reader::get_next_line(){
    std::vector<std::string> line_data;
    if (std::getline(file, line)){
        std::string cell;
        for (int i = 0; i < line.size(); i++){
            if (line[i] == ','){
                line_data.push_back(cell);
                cell = "";
            } else {
                cell += line[i];
            }
        }
        line_data.push_back(cell);
    }
    return line_data;
}


CSV_writer::CSV_writer(std::string filename){
    file.open(filename);
    if (!file.is_open()){
        std::cerr << "Error: file "<<filename<<" not found" << std::endl;
    }
}

CSV_writer::~CSV_writer(){
    file.close();
}

template <typename T>
void CSV_writer::write_line(std::vector<T> line_data){
    for (int i = 0; i < line_data.size(); i++){
        file << line_data[i];
        if (i != line_data.size()-1){
            file << ", ";
        }
    }
    file << std::endl;
}