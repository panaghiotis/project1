#include <iostream>
#include <fstream>
#include "../header/InputParser.h"
#include "../header/clustering.h"
#include "../header/Dataset.h"

using namespace std;

// globals
unsigned int num_clusters = 0;         // no default
unsigned int L = 3;       // L for LSH
unsigned int k = 4;       // num of hash funcs hi
unsigned int M = 10;      //M for cube
unsigned int Dim = 3;     //dimension d' for cube
unsigned int probes = 2;  //probes for cube

bool complete = false;

#define DIVIDE_DATASET_FOR_HASHTABLE_SIZE 8


void read_conf_file(ifstream &conf_stream) {
    string var;
    int value;
    // Read configuration file
    while(conf_stream >> var >> value) {
        if(var == "number_of_clusters:") {
            num_clusters = value;
        }
        else if(var == "number_of_vector_hash_tables:") {
            L = value;
        }
        else if(var == "number_of_vector_hash_functions:") {
            k = value;
        }
        else if(var == "max_number_M_hypercube:") {
            M = value;
        }
        else if(var == "number_of_hypercube_dimensions:") {
            Dim = value;
        }
        else if(var == "number_of_probes:") {
            probes = value;
        }
        else {
            cout << "Invalid conguration file" << std::endl;
        }
    }
    conf_stream.close();
}


int main(int argc, char **argv) {
    // read input
    InputParser input(argc, argv);
    const string &input_file = input.getCmdOption("-i");
    const string &conf_file = input.getCmdOption("-c");
    const string &output_file = input.getCmdOption("-o");
    const string &method = input.getCmdOption("-m");
    complete = input.cmdOptionExists("-complete");

    cout << "Input files from command line:" << endl;
    cout << "input_file:" << input_file << endl;
    cout << "configuration_file:" << conf_file << endl;
    cout << "output_file:" << output_file << endl;

    int method_for_kmeans;
    if (method == "Classic" || method == "classic") {
        method_for_kmeans = CLASSIC;
    } else if (method == "LSH" || method == "lsh") {
        method_for_kmeans = USE_RANGE_LSH;
    } else if (method == "Hypercube" || method == "hypercube" || method == "HYPERCUBE" || method == "CUBE" || method == "cube") {
        method_for_kmeans = USE_HYPERCUBE;
    } else {
        cerr << "No correct method specified for kMeans. Default is Classic" << endl;
        method_for_kmeans = CLASSIC;
    }

    // check input files from cdl
    ifstream input_stream, conf_stream;
    if (!input_file.empty()) {
        if (!input_stream.good()){
            cerr << "Error: input file specified does not exist" << endl;
            return -1;
        } else {
            input_stream.open(input_file, std::ifstream::in);
        }
    }
    if (!conf_file.empty()) {
        if (!conf_stream.good()){
            cerr << "Error: configuration file specified does not exist" << endl;
            return -1;
        } else {
            conf_stream.open(conf_file);
            read_conf_file(conf_stream);
        }
    }

    // read numeric parameters (globals)
    if (num_clusters <= 0)
        num_clusters = atoi(input.getCmdOption("-k").c_str());

    // get input file if not given
    if (input_file.empty()) {
        do {
            cout << "Please specify input file path:" << endl;
            string input_file_in;
            cin >> input_file_in;
            // check
            input_stream.open(input_file_in);
            if (!input_stream.good()) {
                cout << "Invalid file path. Try again." << endl;
            } else{
                break;
            }
        } while (true);
    }

    // load input dataset
    Dataset input_dataset(input_stream);
    if(method_for_kmeans == USE_RANGE_LSH)
        input_dataset.index_LSH(input_dataset.get_size() / DIVIDE_DATASET_FOR_HASHTABLE_SIZE);
    if(method_for_kmeans == USE_HYPERCUBE)
        input_dataset.index_HyperCube(Dim);

    // get configuration file if not given
    if (conf_file.empty()) {
        do {
            cout << "Please specify query file path:" << endl;
            string conf_file_in;
            cin >> conf_file_in;
            // check
            conf_stream.open(conf_file_in);
            if (!conf_stream.good()) {
                cout << "Invalid file path. Try again." << endl;
            } else{
                break;
            }
        } while (true);
        read_conf_file(conf_stream);
    }

    // output stream
    ofstream output_stream;
    if (!output_file.empty()) {
        output_stream.open(output_file.c_str());
    }
    // get output file if not given
    else {
        do {
            cout << "Please specify output file path:" << endl;
            string output_file_in;
            cin >> output_file_in;
            // check
            output_stream.open(output_file_in);
            if (!output_stream.good()) {
                cout << "Invalid file path. Try again." << endl;
            } else{
                break;
            }
        } while (true);
    }

    /**
     * Dataset is ready and conf parameters read
     */

    Clustering clustering(&input_dataset, method_for_kmeans);
    cout << "Performing kMeans..." << endl;
    double dt = clustering.perform_kMeans(num_clusters, M, probes);
    cout << "Finished kMeans" << endl;
    const vector<Cluster *> clusters = clustering.get_clusters();
    cout << "Found " << clusters.size() << " clusters" << endl;
    cout << "Calculate Silhouette..." << endl;
    clustering.calculate_silhouette();
    cout << "Finished calculating Silhouette" << endl;

    // write output
    if(method == "LSH" || method == "lsh")
        output_stream << "Algorithm: Range Search LSH"<< endl;
    else if (method == "Hypercube" || method == "hypercube" || method == "HYPERCUBE" || method == "CUBE" || method == "cube")
        output_stream << "Algorithm: Range Search Hypercube"<< endl;
    else
        output_stream << "Algorithm: Lloyds"<< endl;
    for (int i = 0 ; i < clusters.size() ; i++) {
        output_stream << "CLUSTER-" << (i+1) << " {size: " << clusters[i]->points.size()
                      << ", centroid: " << *((Point *) clusters[i]->centroid);
        if (complete) {
            output_stream << ", points: ";
            for (int j = 0 ; j < clusters[i]->points.size() ; j++) {
                output_stream << ((j == 0) ? "" : ", ") << clusters[i]->points[j]->id;
            }
        }
        output_stream << "}" << endl;
    }
    output_stream << "clustering_time: " << dt << " seconds" << endl;
    output_stream << "Silhouette: [";
    for (int i = 0 ; i < clustering.get_silhouette().size() ; i++) {
        output_stream << ((i == 0) ? "" : ", ")  << clustering.get_silhouette()[i];
    }
    output_stream << "]" << endl;

    // close output stream
    output_stream.close();

    return 0;
}
