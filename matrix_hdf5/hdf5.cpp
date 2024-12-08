// Include HDF5 library headers
#include <H5Cpp.h>
#include <iostream>
#include <string>
#include <vector>

// Constants for the dataset
const std::string FILE_NAME = "genomic_data.h5";
const std::string DATASET_NAME = "DNA_sequences";
const hsize_t NUM_SEQUENCES = 3;
const hsize_t SEQUENCE_LENGTH = 5;

int main() {
  try {
    // Create a new HDF5 file
    H5::H5File file(FILE_NAME, H5F_ACC_TRUNC);

    // Define the dimensions of the array
    hsize_t dims[2] = {NUM_SEQUENCES, SEQUENCE_LENGTH};
    H5::DataSpace dataspace(2, dims);

    // Create a dataset with string data type
    H5::StrType strType(H5::PredType::C_S1, SEQUENCE_LENGTH);
    H5::DataSet dataset = file.createDataSet(DATASET_NAME, strType, dataspace);

    // Example DNA sequences
    std::vector<std::string> sequences = {"ATCGN", "GGTCA", "TACGA"};

    // Write data to the dataset
    for (hsize_t i = 0; i < NUM_SEQUENCES; ++i)
        dataset.write(sequences[i].c_str(), strType, dataspace, dataspace);

    std::cout << "DNA data successfully written to " << FILE_NAME << std::endl;

  } catch (H5::Exception& error) {
    error.printErrorStack();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}