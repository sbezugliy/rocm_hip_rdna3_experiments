#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

int main() {
    // Read CBOR data from a file
    std::ifstream input_file("input_dna_data.cbor", std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::uint8_t> input_buffer(std::istreambuf_iterator<char>(input_file), {});
    json dna_data = json::from_cbor(input_buffer);
    input_file.close();

    // Display the read data
    std::cout << "Read CBOR data:\n" << dna_data.dump(4) << std::endl;

    // Modify the data or use it as needed
    // For demonstration, let's add a new entry
    dna_data["sequences"].push_back({
        {"id", "seq3"}, {"sequence", "TTAGG"}, {"quality", "*****"}
    });

    // Write modified CBOR data to a new file
    std::ofstream output_file("output_dna_data.cbor", std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::uint8_t> output_buffer = json::to_cbor(dna_data);
    output_file.write(reinterpret_cast<const char*>(output_buffer.data()), output_buffer.size());
    output_file.close();

    std::cout << "Data written to output_dna_data.cbor" << std::endl;

    return EXIT_SUCCESS;
}