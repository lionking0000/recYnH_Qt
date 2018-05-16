#ifndef DWAG_H
#define DWAG_H


#include <string> // for string
#include <vector> // for vector
#include <map> // for map

#include "creapairmatrix.h"

using namespace std;

struct dwag {
    string name; // temporary info
    int level; // temporary info
    int index; // -2: default, -1: overlapped, >=0: gene index
    dwag* A;
    dwag* C;
    dwag* G;
    dwag* T;
    dwag* parent;

    public:
        dwag(): A(NULL), C(NULL), G(NULL), T(NULL), parent(NULL), name(""), level(0), index(-2) {}
};

struct Record
{
   Record(string h) : header(h), field() {}
   string header;
   string field;
};

int fusion_pair_search(int argc, char *argv[]);
dwag* build_tree_from_records(dwag* start, vector<Record>& records );
int pair_search_with_tree(dwag* start, map< pair<int, int>, int >& output, char *fastq1, char *fastq2, bool bShortRun, int readlength );
int pair_search_with_tree_normal(dwag* start, map< pair<int, int>, int >& output, char *fastq1, char *fastq2, bool bShortRun, int readlength );
vector<Record> read_fasta( char* fa_filename );

void print_map_pair_values( map<pair<int, int>,int>& output );
void print_map_pair_map( map<pair<int, int>,int>& output, vector<Record>& records );
void write_map_pair_map( map<pair<int, int>,int>& output, vector<Record>& records, char * outfilename );
vector<vector<string>>& ReadTSV(const char* filename);

#endif // DWAG_H
