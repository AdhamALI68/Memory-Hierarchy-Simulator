/*-----------------------------------------------------------------------------------------------
 ASSEMBLY: PROJECT 2
 Implementers: 1- Adham Ali / 900223243
               2- Ebram Raafat / 900214496
               3- Khadiga Iraky / 900222731
 -----------------------------------------------------------------------------------------------
 */
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

struct CacheLine {
    bool valid;       // Valid bit indicating if the cache line is valid or not
    unsigned int tag; // Tag to identify the memory block stored in the cache line
};

void parsing_addresses(const string& filename, vector<int>& sequence_of_addresses) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }
    int address;
    while (in >> address) {
        sequence_of_addresses.push_back(address);
    }
    in.close();
}

void cache_simulation(const string& filename, unsigned int S, unsigned int L, unsigned int Time) {
    unsigned int cache_lines = S / L;
    vector<CacheLine> cache(cache_lines, {false, 0}); // Vector representing the cache

    unsigned int accesses = 0;
    unsigned int hits = 0;
    unsigned int misses = 0;

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return;
    }

    unsigned int memory_address;
    while (inputFile >> memory_address) {
        if (memory_address > (pow(2, 30)))
        { break;}
        ++accesses;
        // Calculate the cache index, tag, and offset for the current memory address
        unsigned int index = (memory_address / L) % cache_lines;
        unsigned int tag = memory_address / (cache_lines * L);

        if (cache[index].valid && cache[index].tag == tag) {
            // Cache hit
            ++hits;
        } else {
            // Cache miss
            ++misses;
            // Update the cache line with the new memory block
            cache[index].valid = true;
            cache[index].tag = tag;
        }


    }

    // Output the valid bits and tags of all cache entries
    outputFile << "Valid bits and tags: " << endl;
    for (unsigned int i = 0; i < cache_lines; i++) {
        outputFile << "Index " << i << " - Valid: " << cache[i].valid << ", Tag: " << cache[i].tag << endl;
    }
    outputFile << endl;
    double amat = ((hits * Time) + (misses * (Time + 150))) / static_cast<double>(accesses);
    // Output the total number of accesses, hit and miss ratios, and AMAT
    outputFile << "Total number of accesses: " << accesses << endl;
    outputFile << "Hit ratio: " << static_cast<double>(hits) / accesses << endl;
    outputFile << "Miss ratio: " << static_cast<double>(misses) / accesses << endl;
    outputFile << "AMAT: " << amat << " cycles" << endl;
    outputFile << endl;

    inputFile.close();
    outputFile.close();
}


void inst_cache_simulation(const string& filename, unsigned int S, unsigned int L, unsigned int Time) {
    unsigned int cache_lines = S / L;
    vector<CacheLine> cache(cache_lines, {false, 0}); // Vector representing the cache

    unsigned int accesses = 0;
    unsigned int hits = 0;
    unsigned int misses = 0;

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return;
    }

    unsigned int memory_address;
    while (inputFile >> memory_address) {
        if (memory_address > (pow(2, 30)))
        { break;}
        ++accesses;
        // Calculate the cache index, tag, and offset for the current memory address
        unsigned int tag = memory_address / (cache_lines * L);
        unsigned int index = (memory_address / L) % cache_lines;

        if (cache[index].valid && cache[index].tag == tag) {
            // Cache hit
            ++hits;
        } else {
            // Cache miss
            ++misses;
            // Update the cache line with the new memory block
            cache[index].valid = true;
            cache[index].tag = tag;
        }


    }

    // Output the valid bits and tags of all cache entries
    outputFile << "Instruction cache: " << endl;
    outputFile << "Valid bits and tags: " << endl;
    for (unsigned int i = 0; i < cache_lines; i++) {
        outputFile << "Index " << i << " - Valid: " << cache[i].valid << ", Tag: " << cache[i].tag << endl;
    }
    outputFile << endl;
    double amat = ((hits * Time) + (misses * (Time + 150))) / static_cast<double>(accesses);
    // Output the total number of accesses, hit and miss ratios, and AMAT
    outputFile << "Total number of accesses: " << accesses << endl;
    outputFile << "Hit ratio: " << static_cast<double>(hits) / accesses << endl;
    outputFile << "Miss ratio: " << static_cast<double>(misses) / accesses << endl;
    outputFile << "AMAT: " << amat << " cycles" << endl;
    outputFile << endl;

    inputFile.close();
    outputFile.close();
}


void n_cache_simulation(const string& filename, unsigned int numCacheLev, vector <unsigned int> S, vector <unsigned int> L, vector <unsigned int> Time)
{
    vector <unsigned int> cache_lines;
    vector <vector<CacheLine>> cache (numCacheLev); // Vector representing the cache

    for(int i = 0; i < numCacheLev; i++)
    {
        unsigned int temp = S[i] / L[i];
        cache_lines.push_back(temp);
        vector <CacheLine> oneCache (cache_lines[i], {false, 0});
        cache[i] = oneCache;
    }

    unsigned int totalAccesses = 0;
    unsigned int totalMisses = 0;

    vector<unsigned int> accesses (numCacheLev, 0);
    vector<unsigned int> hits (numCacheLev, 0);
    vector<unsigned int> misses (numCacheLev, 0);

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return;
    }

    unsigned int memory_address;
    while (inputFile >> memory_address) {
        if (memory_address > (pow(2, 30)))
        { break;}
        ++totalAccesses;
        // Calculate the cache index, tag, and offset for the current memory address

        vector<unsigned int> index(numCacheLev);
        vector<unsigned int> tag(numCacheLev);
        unsigned int found = 0;

        for (int i = 0; i < numCacheLev; i++) {
            unsigned int tag_temp = memory_address / (L[i] * cache_lines[i]);
            unsigned int index_temp = (memory_address / L[i]) % cache_lines[i];
            index[i] = index_temp;
            tag[i] = tag_temp;

        }

        for (int i = 0; i < numCacheLev; i++) {

            if (cache[i][index[i]].valid && cache[i][index[i]].tag == tag[i]) {
                // Cache hit
                ++hits[i];
                ++accesses[i];
                found = 1;
                break;
            }
            else
                ++misses[i];
                ++accesses[i];

        }

        if (found == 0)
        {
            // Cache miss
                // Cache miss
                ++totalMisses;
                // Update the cache line with the new memory block

            for (int i = 0; i < numCacheLev; i++) {
                cache[i][index[i]].valid = true;
                cache[i][index[i]].tag = tag[i];
            }
        }
    }

    Time.push_back(150);

    for (int i = 0; i < numCacheLev; i++) {        // Output the valid bits and tags of all cache entries
        outputFile << "For Level " << i + 1 << " valid bits and tags: " << endl;

        for (unsigned int j = 0; j < cache_lines[i]; j++) {
            outputFile << "Index " << j << " - Valid: " << cache[i][j].valid << ", Tag: " << cache[i][j].tag << endl;
        }
        outputFile << endl;

        // Output the total number of accesses, hit and miss ratios, and AMAT
        outputFile << "Total number of accesses: " << accesses[i] << endl;
        outputFile << "Hit ratio: " << static_cast<double>(hits[i]) / accesses[i] << endl;
        outputFile << "Miss ratio: " << static_cast<double>(misses[i]) / accesses[i] << endl;
        outputFile << "AMAT: " << (hits[i] * Time[i] + misses[i] * (Time[i] + Time[i+1])) / static_cast<double>(totalAccesses)
                   << " cycles" << endl;
        outputFile << endl;
    }

    outputFile << "Total Miss ratio: " << static_cast<double>(totalMisses) / totalAccesses << endl;


    inputFile.close();
    outputFile.close();
}


int main() {
    bool answer = 0;
    vector <unsigned int> S; // Cache size in bytes
    vector <unsigned int> L; // Cache line size in bytes
    vector <unsigned int> Time; // Cache access time in cycles
    unsigned int numCacheLev = 1;

    unsigned int S_temp;
    unsigned int L_temp;
    unsigned int Time_temp;

    unsigned int S_inst;
    unsigned int L_inst;
    unsigned int Time_inst;

    cout<<"Enter the number of Cache levels you want: ";
    cin>> numCacheLev;

    for(int i = 1; i <= numCacheLev; i++)
    {
         cout<<endl<<"For Level "<<i<<":"<<endl;
         cout << "Enter cache size (S): ";
         cin >> S_temp;
         cout << endl <<"Enter cache line size (L): ";
         cin >> L_temp;
         cout << endl <<"Enter access time (Time): ";
         cin >> Time_temp;

         S.push_back(S_temp);
         L.push_back(L_temp);
         Time.push_back(Time_temp);
    }

    if (numCacheLev == 1)
    {
        cout<<"Do you want instruction and data cache? (1 yes, 0 no) ";
        cin>>answer;
        if (answer)
        {
            cout<<endl<<"For instruction cache:"<<endl;
            cout << "Enter cache size (S): ";
            cin >> S_inst;
            cout << endl <<"Enter cache line size (L): ";
            cin >> L_inst;
            cout << endl <<"Enter access time (Time): ";
            cin >> Time_inst;

            cache_simulation("/Users/Yusuf Iraky/CLionProjects/assembly2/DataAccesssSeq.txt", S[0], L[0], Time[0]);
            inst_cache_simulation("/Users/Yusuf Iraky/CLionProjects/assembly2/InstructionsAccesssSeq.txt", S_inst, L_inst, Time_inst);


        }

        else
        cache_simulation("/Users/Yusuf Iraky/CLionProjects/assembly2/AccesssSeq.txt", S[0], L[0], Time[0]);
    }
    else
        n_cache_simulation("/Users/Yusuf Iraky/CLionProjects/assembly2/AccesssSeq.txt", numCacheLev, S, L, Time);

    return 0;
}




