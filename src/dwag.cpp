#include "dwag.h"

#include <iostream> // for cout
#include <fstream>  // for ifstream
#include "zlib.h"  // for gzFile; presumes zstd library is installed
#include "gmm.h"

#define MIS_MATCH_CUT   5
#define MIN_LEVEL_CUT   30
#define MATCH(a,b,level) ((a) == (b) ? (level) : (level+1))
#define LENS    200

#define DEBUG   0   // 0

// use this to read part of the line
#define SHORT_RUN


#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>


#include <armadillo>

using namespace arma;

vector<vector<string>>& ReadTSV(const char* filename)
{
    using namespace std;

    ifstream infile(filename);
    if (!infile) {
        cout << "unable to load file" << endl;
    }
    string str;

    vector<vector<string>> vvStr;

    vector<string> vecTitle;
    int pos1, pos2;

    // title

    getline(infile, str);
    pos1 = 0;
    while((pos2 = str.find('\t',pos1))!= string::npos)
    {
        vecTitle.push_back(str.substr(pos1, pos2-pos1));
        pos1 = ++pos2;
    }

    // values
    while (getline(infile, str))
    {
        pos1 = 0;
        vector<string> vStr;
        while((pos2 = str.find('\t',pos1))!= string::npos)
        {
            vStr.push_back(str.substr(pos1, pos2-pos1));
            pos1 = ++pos2;
        }
        vStr.push_back(str.substr(pos1, string::npos));
        vvStr.push_back(vStr);
    }

    return vvStr;
}

int gmm_test()
{
    // create synthetic data containing
    // 2 clusters with normal distribution
    uword d = 1; // dimensionality
    uword N = 10000; // number of samples (vectors)
    mat data(d, N, fill::zeros);
    vec mean1 = linspace<vec>(1,d,d);
    vec mean2 = mean1 + 5;
    uword i = 0;
    while(i < N)
    {
        if(i < N) { data.col(i) = mean1 + randn<vec>(d); ++i; }
        if(i < N) { data.col(i) = mean1 + randn<vec>(d); ++i; }
        if(i < N) { data.col(i) = mean2 + randn<vec>(d); ++i; }
    }
    // model the data as a diagonal GMM with 2 Gaussians
    gmm_diag model;
    bool status = model.learn(data, 2, maha_dist, random_subset,10, 5, 1e-10, true);
    if(status == false) { cout << "learning failed" << endl; }

    model.means.print("means:");

    double overall_likelihood = model.avg_log_p(data);

    rowvec set_likelihood = model.log_p( data.cols(0,9) );
    double scalar_likelihood = model.log_p( data.col(0) );

    uword gaus_id = model.assign( data.col(0), eucl_dist );
    urowvec gaus_ids = model.assign( data.cols(0,9), prob_dist );

    urowvec histogram1 = model.raw_hist (data, prob_dist);
    rowvec histogram2 = model.norm_hist(data, eucl_dist);

    cout << "histogram2\t" << histogram2[0] << "\t" << histogram2[1] << endl;

    model.save("/Users/jyang/my_model.gmm");

    mat modified_dcovs = 2 * model.dcovs;

    model.set_dcovs(modified_dcovs);

    return 0;
}

vector<Record> read_fasta( char* fa_filename )
{
    gmm_test();
    /*
   GaussianMixture aGmm = GaussianMixture(4,4,0.1,0.1);
   //aGmm.setValue();

   Eigen::VectorXf ax;
   ax << 1,2,3,4;

   Eigen::VectorXf bx;
    bx << 2,3,4,5;

    aGmm.setValue(ax,bx);

    //aGmm.value();
    */
   vector<Record> records;

   ifstream fin( fa_filename );
   if(!fin)
   {
      cerr << "Couldn't open the input file!";
      return records;
   }
   string line;
   size_t counter = -1;

   getline(fin, line);
   while(fin)
   {
      if(line[0] == '>')
      {
         records.push_back(Record(&line[1]));
         counter++;
      }
      else
      {
         records[counter].field += line;
      }
      getline(fin, line);
   }

   return records;
}

string reverse_complement( string DNA ){
    string output;
    for( int i = DNA.size()-1; i >= 0; i -- ){
        switch ( DNA[i] ){
            case 'a':
            case 'A':
                output += 'T';
                break;
            case 'c':
            case 'C':
                output += 'G';
                break;
            case 'g':
            case 'G':
                output += 'C';
                break;
            case 't':
            case 'T':
                output += 'A';
                break;
            default:
                output += DNA[i];
                break;
        }
    }
    return output;
}

/*
 *    Building fake tree structure based on the last 60 seq. from fasta (reverse complement).
 *    DWAG starting pointer should be given.
 *
 *    infilename : fasta file
 */
dwag* build_tree_from_records(dwag* start, vector<Record>& records ){
    for ( int i = 0; i < records.size(); i++ ){
        string rc_seq = reverse_complement( records[i].field );
        //cout << records[i].header << endl << records[i].field << endl << rc_seq << endl;

        dwag* next = start;
        //cout << start->name << endl;
        for ( int j = 0; j < 100; j++ ){
            char aChar = rc_seq[j];
            //cout << aChar;

            switch( aChar ){
                case 'A':
                    if ( next->A == NULL ){
                        next->A = new dwag;
                        next->A->name = "A";
                        next->A->level = next->level + 1;
                        next->A->parent = next;
                        next = next->A;
                    }else{
                        next = next->A;
                    }
                break;
                case 'C':
                    if ( next->C == NULL ){
                        next->C = new dwag;
                        next->C->name = "C";
                        next->C->level = next->level + 1;
                        next->C->parent = next;
                        next = next->C;
                    }else{
                        next = next->C;
                    }
                break;
                case 'G':
                    if ( next->G == NULL ){
                        next->G = new dwag;
                        next->G->name = "G";
                        next->G->level = next->level + 1;
                        next->G->parent = next;
                        next = next->G;
                    }else{
                        next = next->G;
                    }
                break;
                case 'T':
                    if ( next->T == NULL ){
                        next->T = new dwag;
                        next->T->name = "T";
                        next->T->level = next->level + 1;
                        next->T->parent = next;
                        next = next->T;
                    }else{
                        next = next->T;
                    }
                break;
            }
            if ( next->index == -2 ) next->index = i; // assign gene index when it is first time
            else next->index = -1; // overlap with other gene
        }
        //cout << endl;
        //cout << get_sequence( next ) << endl;
    }
    //cout << "$$$$$$ " << start->name << endl;
    return start;
}

dwag* mismatch_search_tree_c( dwag* next, char* value, int level, int mismatch, int* global_mismatch, dwag* found ){
    if ( mismatch >= MIS_MATCH_CUT ){
        return found;
    }
    if (level >= MIN_LEVEL_CUT){
        if ( mismatch < *global_mismatch ){
            *global_mismatch = mismatch;
            found = next;
            //cout << "REF: " << get_sequence( found ) << endl;
            //cout << "QUR: " << value << endl;
        }
        return found;
    }

    if ( next->A != NULL ) found = mismatch_search_tree_c( next->A, value, level+1, MATCH(value[level],'A',mismatch), global_mismatch, found );
    if ( next->C != NULL ) found = mismatch_search_tree_c( next->C, value, level+1, MATCH(value[level],'C',mismatch), global_mismatch, found );
    if ( next->G != NULL ) found = mismatch_search_tree_c( next->G, value, level+1, MATCH(value[level],'G',mismatch), global_mismatch, found );
    if ( next->T != NULL ) found = mismatch_search_tree_c( next->T, value, level+1, MATCH(value[level],'T',mismatch), global_mismatch, found );

    return found;
}

/*
 *
 * This used whole fastq files
 *
 */
namespace io = boost::iostreams;

int pair_search_with_tree_normal(dwag* start, map< pair<int, int>, int >& output, char *fastq1, char *fastq2, bool bShortRun = false, int readlength = 151){
    //cout << "### " << start->name << endl;
    char buffer1[LENS];
    for (int xx = 0; xx<LENS; xx++) buffer1[xx] = NULL;
    char buffer2[LENS];
    for (int xx = 0; xx<LENS; xx++) buffer2[xx] = NULL;
    char gz1[LENS];
    char gz2[LENS];
    std::cout << fastq1 << std::endl;
    std::cout << fastq2 << std::endl;

    gzFile infile1 = gzopen(fastq1, "rb");
    if (!infile1) return -1;

    gzFile infile2 = gzopen(fastq2, "rb");
    if (!infile2) return -1;

    char buffer[LENS];
    int count = 0;
    int found_cnt1 = 0;
    int found_cnt2 = 0;
    int global_mismatch = 1000;
    int found_both_cnt = 0;
    dwag* found1 = NULL;
    dwag* found2 = NULL;

    int i = 0;
    while( 0 != gzgets(infile1, buffer1, LENS) ){
        if ( 0 == gzgets(infile2, buffer2, LENS) ){
            break;
        }

        i++;
        if ( i % 100000 == 0 ) {
            cout << count << "\t" << found_cnt1 << "\t" << found_cnt2 << "\t" << found_both_cnt << "\t" << found_both_cnt / (float) count * 100.0 << endl;
            if (  i > 1000000 && bShortRun == true ) break;
        }

        if ( i % 4 != 2 ) continue;
        count += 1;
        global_mismatch = 1000;

        found1 = NULL;
        if ( found1 == NULL ) {
            for ( int xx = 30; xx < 60; xx ++ ){
                found1 = mismatch_search_tree_c( start, &buffer1[xx], 0, 0, &global_mismatch, found1 ); // for read1
                if ( found1 != NULL ) break;
            }
        }
        if ( found1 != NULL ){
            found_cnt1 += 1;
        }

        global_mismatch = 1000;
        found2 = NULL;
        if ( found2 == NULL ) {
            for ( int xx = 30; xx < 60; xx ++ ){
                found2 = mismatch_search_tree_c( start, &buffer2[xx], 0, 0, &global_mismatch, found2 ); // for read1
                 if ( found2 != NULL ) break;
            }
        }
        if ( found2 != NULL ){
            /*if ( global_mismatch == MIS_MATCH_CUT-1 ){
                cout << "READ2: " << i << "\t" << global_mismatch << "\t" << found2->index << endl;
                cout << "REF: " << get_sequence( found2 ) << endl;
                cout << "QUR: " << string( &buffer2[54] ) << endl;
            }*/
            found_cnt2 += 1;
        }
        if ( found1 != NULL && found2 != NULL ){
            ++output[pair<int, int>(found1->index, found2->index)];
            found_both_cnt ++;
        }
    }
    cout << "Total: " << count << endl;
    cout << "Found 1: " << found_cnt1 << "\t" << found_cnt1 / (float) count * 100.0 << endl;
    cout << "Found 2: " << found_cnt2 << "\t" << found_cnt2 / (float) count * 100.0 << endl;
    cout << "Found Both: " << found_both_cnt << "\t" << found_both_cnt / (float) count * 100.0 << endl;
    gzclose( infile1 );
    gzclose( infile2 );

/*
    io::filtering_istream in;
    in.push(io::gzip_decompressor());
    in.push(io::file_source(fastq1));
    // read from in using std::istream interface

    std::string line;
    while (std::getline(in, line, '\n'))
    {
         std::cout << line ; // your code :)
    }
*/

    return 0;
}

/*
 *
 * This used only second line - seq files
 *
 */
int pair_search_with_tree(dwag* start, map< pair<int, int>, int >& output, char *fastq1, char *fastq2, bool bShortRun = false, int readlength = 151){
    //cout << "### " << start->name << endl;
    char buffer1[LENS];
    for (int xx = 0; xx<LENS; xx++) buffer1[xx] = NULL;
    char buffer2[LENS];
    for (int xx = 0; xx<LENS; xx++) buffer2[xx] = NULL;
    char gz1[LENS];
    char gz2[LENS];
    sprintf( gz1, "%s.2", fastq1 );
    sprintf( gz2, "%s.2", fastq2 );
    std::cout << gz1 << std::endl;
    std::cout << gz2 << std::endl;


    int size = 0;
    int i = 0;
    int line = 0;
    int global_mismatch = 1000;
    int found_cnt1 = 0;
    int found_cnt2 = 0;
    int found_both_cnt = 0;
    dwag* found1 = NULL;
    dwag* found2 = NULL;

    gzFile infile1 = gzopen(gz1, "rb");
    if (!infile1) return -1;
    gzFile infile2 = gzopen(gz2, "rb");
    if (!infile2) return -1;

    while( 0 != (size = gzread(infile1, buffer1, readlength+1) ) ){
        (size = gzread(infile2, buffer2, readlength+1) );
        //cout << buffer1 << buffer2 << endl;
        global_mismatch = 1000;
        i++;
        if ( i % 100000 == 0 ) {
            cout << i << "\t" << found_cnt1 << "\t" << found_cnt2 << "\t" << found_both_cnt << "\t" << found_both_cnt / (float) i << endl;
            if (  i > 1000000 && bShortRun == true ) break;
        }
        found1 = NULL;
        //found1 = mismatch_search_tree_c( start, &buffer1[52], 0, 0, &global_mismatch, found1 ); // for read1
        //if ( found1 == NULL ) found1 = mismatch_search_tree_c( start, &buffer1[50], 0, 0, &global_mismatch, found1 ); // for read1
        //if ( found1 == NULL ) found1 = mismatch_search_tree_c( start, &buffer1[51], 0, 0, &global_mismatch, found1 ); // for read1
        //if ( found1 == NULL ) found1 = mismatch_search_tree_c( start, &buffer1[53], 0, 0, &global_mismatch, found1 ); // for read1
        //if ( found1 == NULL ) found1 = mismatch_search_tree_c( start, &buffer1[54], 0, 0, &global_mismatch, found1 ); // for read1
        if ( found1 == NULL ) {
            for ( int xx = 0; xx < 100; xx ++ ){
                found1 = mismatch_search_tree_c( start, &buffer1[xx], 0, 0, &global_mismatch, found1 ); // for read1
                if ( found1 != NULL ) break;
            }
        }
        if ( found1 != NULL ){
            /*if ( global_mismatch == MIS_MATCH_CUT-1 ){
                cout << "READ1: " << i << "\t" << global_mismatch << "\t" << found1->index << endl;
                cout << "REF: " << get_sequence( found1 ) << endl;
                cout << "QUR: " << string( &buffer1[52] ) << endl;
            }*/
            found_cnt1 += 1;
        }
        global_mismatch = 1000;
        found2 = NULL;
        //found2 = mismatch_search_tree_c( start, &buffer2[54], 0, 0, &global_mismatch, found2 ); // for read2
        //if ( found2 == NULL ) found2 = mismatch_search_tree_c( start, &buffer2[52], 0, 0, &global_mismatch, found2 ); // for read2
        //if ( found2 == NULL ) found2 = mismatch_search_tree_c( start, &buffer2[53], 0, 0, &global_mismatch, found2 ); // for read2
        //if ( found2 == NULL ) found2 = mismatch_search_tree_c( start, &buffer2[55], 0, 0, &global_mismatch, found2 ); // for read2
        //if ( found2 == NULL ) found2 = mismatch_search_tree_c( start, &buffer2[56], 0, 0, &global_mismatch, found2 ); // for read2
        if ( found2 == NULL ) {
            for ( int xx = 0; xx < 100; xx ++ ){
                found2 = mismatch_search_tree_c( start, &buffer2[xx], 0, 0, &global_mismatch, found2 ); // for read1
                 if ( found2 != NULL ) break;
            }
        }
        if ( found2 != NULL ){
            /*if ( global_mismatch == MIS_MATCH_CUT-1 ){
                cout << "READ2: " << i << "\t" << global_mismatch << "\t" << found2->index << endl;
                cout << "REF: " << get_sequence( found2 ) << endl;
                cout << "QUR: " << string( &buffer2[54] ) << endl;
            }*/
            found_cnt2 += 1;
        }
        if ( found1 != NULL && found2 != NULL ){
            ++output[pair<int, int>(found1->index, found2->index)];
            found_both_cnt ++;
        }
    }
    cout << "Found 1: " << found_cnt1 << "\t" << found_cnt1 / (float) i << endl;
    cout << "Found 2: " << found_cnt2 << "\t" << found_cnt2 / (float) i << endl;
    cout << "Found Both: " << found_both_cnt << "\t" << found_both_cnt / (float) i << endl;
    gzclose( infile1 );
    gzclose( infile2 );
    return 0;
}

void print_map_pair_values( map<pair<int, int>,int>& output ){
    for ( map< pair<int,int>, int>::iterator it = output.begin(); it != output.end();  it++ ){
        cout << it->first.first << "," << it->first.second << "\t" << it->second << endl;
    }
}

void print_map_pair_map( map<pair<int, int>,int>& output, vector<Record>& records ){
    int total_cnt = 0;
    int zero_cnt = 0;
    for ( int i = 0; i < records.size(); i ++ ){
        for ( int j = 0; j < records.size(); j ++ ){
            int pair_cnt = output[ pair<int,int>(i,j) ];
            cout << i << "," << j << "\t" << records[i].header << "," << records[j].header << "\t" << pair_cnt << endl;
            if ( pair_cnt == 0 ) zero_cnt ++;
            total_cnt ++;
        }
    }
    cout << "Total: " << total_cnt << "\tZeros: " << zero_cnt << "\tFound: " << (total_cnt-zero_cnt) / (float)(total_cnt) * 100.0 << "%" << endl;
    /*for ( map< pair<int,int>, int>::iterator it = output.begin(); it != output.end();  it++ ){
        cout << it->first.first << "," << it->first.second << "\t" << it->second << endl;
    }*/
}

void write_comment_conditions( char * outfilename ){
    ofstream outFile( outfilename );
    outFile << "# ALLOWED_MISMATCH " << MIS_MATCH_CUT - 1 << endl;
    outFile << "# MIN_MATCH_LENGTH " << MIN_LEVEL_CUT << endl;
    outFile.close();
}

void write_map_pair_map( map<pair<int, int>,int>& output, vector<Record>& records, char * outfilename ){
    ofstream outFile;
    outFile.open( outfilename, ofstream::out | ofstream::app );

    int total_cnt = 0;
    int zero_cnt = 0;
    for ( int i = 0; i < records.size(); i ++ )
        outFile << "\t" << records[i].header;
    outFile << endl;
    for ( int i = 0; i < records.size(); i ++ ){
        outFile << records[i].header;
        for ( int j = 0; j < records.size(); j ++ ){
            int pair_cnt = output[ pair<int,int>(i,j) ];
            outFile << "\t" << pair_cnt;
            //outFile << i << "," << j << "\t" << records[i].header << "," << records[j].header << "\t" << pair_cnt << endl;
            if ( pair_cnt == 0 ) zero_cnt ++;
            total_cnt ++;
        }
        outFile << endl;
    }
    //outFile << "Total: " << total_cnt << "\tZeros: " << zero_cnt << "\tFound: " << (total_cnt-zero_cnt) / (float)(total_cnt) * 100.0 << "%" << endl;
    outFile.close();
    /*for ( map< pair<int,int>, int>::iterator it = output.begin(); it != output.end();  it++ ){
 *         cout << it->first.first << "," << it->first.second << "\t" << it->second << endl;
 *             }*/
}

/*
 * argv[1] : fasta
 * argv[2] : fastq1
 * argv[3] : fastq2
 */
int fusion_pair_search(int argc, char *argv[]){
    //std::cout << "fusion_pair_search" << std::endl;

    dwag start;
    map<pair<int, int>,int> output;

    //cout << "read_fasta: " << argv[2] << endl;
    vector<Record> records = read_fasta( argv[2] );

    //cout << "build_tree_from_records" << endl;
    build_tree_from_records( &start, records );

    //cout << "pair_search_with_tree: " << argv[3] << " , " << argv[4] << endl;
    pair_search_with_tree( &start, output, argv[3], argv[4], 151 );

    print_map_pair_values( output );
    print_map_pair_map( output, records );
    write_comment_conditions( argv[5] );
    write_map_pair_map( output, records, argv[5] );
    return 0;
}
