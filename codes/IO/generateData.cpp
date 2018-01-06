#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
using namespace std;

string genFileName(const int i)
{
    return std::to_string(i) + ".dat";
}

void writeHeader(std::ofstream &f, const size_t num, const int firstVecId)
{
    static unsigned int header[5] = {0};
    header[0] = 512;
    header[1] = num;
    header[2] = firstVecId;
    f.write((char *)header, sizeof(header));
}

int main(int argc, char **argv)
{
    unsigned total = 5e7;
    unsigned block = 250e4;
    int fileNum = (total + block - 1) / block;
    string baseFeature("0.fmem");
    string baseList("id_name_list");
    const int dim = 512;
    fstream is_feat(baseFeature, std::fstream::in | std::fstream::binary);
    fstream is_name(baseList, std::fstream::in);
    unsigned int header[5] = {0};
    is_feat.read((char *)header, sizeof(header));
    unsigned num = header[1];
    unsigned id = header[2];
    vector<float> f(dim, 0);
    vector<float> base;
    vector<string> names(num, "");
    base.reserve(num * dim * sizeof(float));
    stringstream ss;
    for (size_t i = 0; i < num; ++i)
    {
        is_feat.read((char *)f.data(), dim * sizeof(float));
        base.insert(base.end(), f.begin(), f.end());
        getline(is_name, names[i]);
        ss.str("");
        ss.clear();
        ss<<names[i];
        unsigned n=0;
        ss>>n;
        ss>>names[i];
    }
    is_feat.close();
    is_name.close();
    std::ofstream id_name_list("bmap.txt", std::fstream::out);
    ofstream featureStream;
    unsigned processed = 0;
    int times = block / num;
    int featPerFile = times * num;
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < fileNum; ++i)
    {
        featureStream.open(genFileName(i + 1), std::fstream::out | std::fstream::binary | std::fstream::trunc);
        writeHeader(featureStream, featPerFile, processed);
        for (int j = 0; j < times; ++j)
        {
            featureStream.write((char *)base.data(), base.size() * sizeof(float));
            for (const auto &name : names)
                id_name_list << id++ << " " << name << endl;
        }
        featureStream.close();
        processed += featPerFile;
        auto tmpTP = chrono::steady_clock::now();
        auto t = chrono::duration_cast<chrono::milliseconds>(tmpTP - start);
        cout << "Processed: " << processed << " TimeUsed: " << t.count() << "ms"
             << "\r";
    }
    cout << endl;
    id_name_list.close();
    return 0;
}
