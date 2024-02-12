#include "csv_parser.h"

namespace util{
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
        for (long unsigned i = 0; i < line.size(); i++){
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

}