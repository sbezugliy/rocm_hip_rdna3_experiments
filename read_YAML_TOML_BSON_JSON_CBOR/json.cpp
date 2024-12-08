#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    // Open and read JSON data from a file
    std::ifstream input_file("input_dna_data.json");
    json dna_data;

    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return EXIT_FAILURE;
    }

    input_file >> dna_data;
    input_file.close();

    // Display the read data
    std::cout << "Read JSON data:\n" << dna_data.dump(4) << std::endl;

    // Modify the data or use it as needed
    // For demonstration, let's just add a new entry
    dna_data["sequences"].push_back({
        {"id", "seq3"}, {"sequence", "TTAGG"}, {"quality", "*****"}
    });

    // Write modified JSON data to a new file
    std::ofstream output_file("output_dna_data.json");
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return EXIT_FAILURE;
    }

    output_file << std::setw(4) << dna_data << std::endl;
    output_file.close();

    std::cout << "Data written to output_dna_data.json" << std::endl;

    return EXIT_SUCCESS;
}