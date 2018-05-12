#include "robot.h"
#include <map>
#include <iostream> // for cout
#include <fstream>  // for ifstream
#include "zlib.h"  // for gzFile; presumes zstd library is installed

#define MIS_MATCH_CUT   5
#define MIN_LEVEL_CUT   30
#define MATCH(a,b,level) ((a) == (b) ? (level) : (level+1))
#define LENS    2048

#define DEBUG   0   // 0

// use this to read part of the line
#define SHORT_RUN


// For visualization (MainWindow and R)
#include "qtdensity.h"
#include "mainwindow.h"
#include <QApplication>


vector<Record> read_fasta( char* fa_filename )
{
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
            if ( bShortRun == true ) break;
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

class COptionFusionPairSearch:public COption{
    public:
        COptionFusionPairSearch() { _command = "FusionPairSearch"; }
        int work(int argc, char** argv, RuntimeInfo* run){
            cout << _command << endl;
            //return fusion_pair_search( argc, argv );
            run->_pfp = new FusionPair( argc, argv );
            run->_pfp->bShortRun = run->_config.fp_bShortRun;
            run->_pfp->Search();
            return 1;
        }
};

class COptionReadPairResults:public COption{
    public:
        COptionReadPairResults() { _command = "ReadPairResults"; }
        int work(int argc, char** argv, RuntimeInfo* run){
            cout << _command << endl;
            run->_pfp = new FusionPair( argc, argv );
            run->_pfp->ReadPairResults( run );
            return 1;
        }
};

CRobot::CRobot( int argc, char* argv[] ): _argc(argc), _argv(argv){
    _option_list.push_back( new COptionFusionPairSearch() );

    _option_list.push_back( new COptionReadPairResults() );
    /*
    // related with fusion search
    _option_list.push_back( new COptionFakeTreeTest() );
    _option_list.push_back( new COptionPerformanceTest() );
    _option_list.push_back( new COptionSearchTest() );
    _option_list.push_back( new COptionFusionPairSearch() );

    // related with compression
    _option_list.push_back( new COptionSplitFastq() );
    _option_list.push_back( new COptionSplitFastqToZst() );
    _option_list.push_back( new COptionReadZst() );

    // related with kmer - compression
    _option_list.push_back( new COptionKmer() );
    _option_list.push_back( new COptionKmerDeCompress() );

    // related with integer packing
    _option_list.push_back( new COptionIntPacking() );

    // related with dwag
    _option_list.push_back( new COptionDWAG() );

    // related with nt + qual
    _option_list.push_back( new COptionCombined() );
    */
}

int CRobot::work(){
    if (_argc <= 1) return 0;
    for( int i = 0; i < _option_list.size(); i ++ ){
        COption* opt = _option_list[i];
        if ( opt->_command == _argv[1] )
            return opt->work( _argc, _argv, &_run );
    }
    return 0;
}

int CRobot::visualize(){
#ifdef USE_R
    RInside R(_argc, _argv);  		// create an embedded R instance

    QApplication app(_argc, _argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window(R,&_run);
    window.show();

    return app.exec();
#else
    QApplication app(_argc, _argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window(&_run, 0);
    //MainWindow window(&_run);
    window.show();

    return app.exec();
#endif
}

CRobot::~CRobot(){
    for( int i = 0; i < _option_list.size(); i ++ ){
        COption* opt = _option_list[i];
        delete opt;
    }
}
