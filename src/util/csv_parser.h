#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


namespace util {
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

    template <typename T>
    void CSV_writer::write_line(std::vector<T> line_data){
        for (int i = 0; i < line_data.size(); i++){
            file << line_data[i];
            if (i != line_data.size()-1){
                file << ",";
            }
        }
        file << std::endl;
    }
}