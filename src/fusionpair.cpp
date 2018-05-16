#include "fusionpair.h"
#include <stdlib.h>     /* atoi */
#include <iostream> // for cout
#include <fstream>  // for ifstream
#include <math.h>

FusionPair::FusionPair()
{
    this->bShortRun = false; //true;
}

FusionPair::FusionPair(int argc, char *argv[])
{
    this->_argc = argc;
    this->_argv = argv;
    this->bShortRun = false; //true;
}

int FusionPair::Search_Normal(){
    cout << "read_fasta: " << this->_argv[2] << endl;
    this->records = read_fasta( this->_argv[2] );

    for ( vector<Record>::iterator it = this->records.begin(); it != this->records.end(); ++it ){
        Record aRecord = *it;
        this->_colnames.push_back( aRecord.header );
        this->_rownames.push_back( aRecord.header );
    }

    //for( int i = 0){
    //    this->_colnames;
    //}

    cout << "build_tree_from_records" << endl;
    build_tree_from_records( &this->start, records );

    cout << "pair_search_with_tree: " << this->_argv[3] << " , " << this->_argv[4] << endl;
    //pair_search_with_tree( &this->start, this->output, this->_argv[3], this->_argv[4], this->bShortRun, atoi(this->_argv[6]) ); // for separated fastq file
    pair_search_with_tree_normal( &this->start, this->output, this->_argv[3], this->_argv[4], this->bShortRun, atoi(this->_argv[6]) );

    print_map_pair_values( output );
    print_map_pair_map( output, records );
    //write_comment_conditions( this->_argv[5] );
    write_map_pair_map( output, records, this->_argv[5] );
}

int FusionPair::Search(){
    cout << "read_fasta: " << this->_argv[2] << endl;
    this->records = read_fasta( this->_argv[2] );

    for ( vector<Record>::iterator it = this->records.begin(); it != this->records.end(); ++it ){
        Record aRecord = *it;
        this->_colnames.push_back( aRecord.header );
        this->_rownames.push_back( aRecord.header );
    }

    //for( int i = 0){
    //    this->_colnames;
    //}

    cout << "build_tree_from_records" << endl;
    build_tree_from_records( &this->start, records );

    cout << "pair_search_with_tree: " << this->_argv[3] << " , " << this->_argv[4] << endl;
    pair_search_with_tree( &this->start, this->output, this->_argv[3], this->_argv[4], this->bShortRun, atoi(this->_argv[6]) ); // for separated fastq file

    //print_map_pair_values( output );
    //print_map_pair_map( output, records );
    //write_comment_conditions( this->_argv[5] );
    //write_map_pair_map( output, records, this->_argv[5] );
}

vector<string> split(const string &text, char sep) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}


vector<string> read_table( char* table_filename ){
    vector<string> data;
    ifstream fin( table_filename );
    if(!fin)
    {
       cerr << "Couldn't open the input file!";
       return data;
    }
    string line;
    size_t counter = -1;

    getline(fin, line);
    while(fin)
    {
       data.push_back(line);
       getline(fin, line);
    }
    return data;
}

int FusionPair::ReadPairResults(RuntimeInfo* run){
    // READ TABLES
    cout << "read_fasta: " << this->_argv[2] << endl;
    this->records = read_fasta( this->_argv[2] );


    // READ NO SELECTION
    cout << "read_no_selection table: " << this->_argv[3] << endl;
    vector<string> data = read_table( this->_argv[3] );

    int i = 0;
    int j = 0;

    for( i = 0; i < data.size(); i++ ){
        vector<string> fields = split(data[i], '\t');
        if ( i == 0 ){ // names
            for( j = 1; j < fields.size(); j++ ){
                this->_colnames.push_back( fields[j] );
                cout << "_colnames " << j << " " << fields[j] << endl;
            }
        }else{
            // j = 0 ; name
            this->_rownames.push_back( fields[0] );
            cout << "_rownames " << i << " " << fields[0] << endl;
            for( int j = 1; j < fields.size(); j++ ){
                //this->output[ pair<int,int>(i-1,j-1) ] = log2( atoi(fields[j].c_str()) + 1.0 );
                this->_no_selection[ pair<int,int>(i-1,j-1) ] = atoi(fields[j].c_str());

            }
        }
    }

    run->_config.nrow = i - 1;
    run->_config.ncol = j - 1;


    // READ SELECTION
    cout << "read_selection table: " << this->_argv[4] << endl;
    vector<string> data2 = read_table( this->_argv[4] );

    i = 0;
    j = 0;

    for( i = 0; i < data2.size(); i++ ){
        vector<string> fields = split(data2[i], '\t');
        if ( i == 0 ){ // names
            for( j = 1; j < fields.size(); j++ ){
                cout << "_colnames " << j << " " << fields[j] << endl;
            }
        }else{
            // j = 0 ; name
            cout << "_rownames " << i << " " << fields[0] << endl;
            for( int j = 1; j < fields.size(); j++ ){
                this->output[ pair<int,int>(i-1,j-1) ] = atoi(fields[j].c_str()); //log2( atoi(fields[j].c_str()) + 1.0 );
                this->_selection1[ pair<int,int>(i-1,j-1) ] = atoi(fields[j].c_str());
            }
        }
    }


    /*
    this->_config.nrow = nrow;
    this->_config.ncol = ncol;

    //fusion_pair_search( argc, argv );

    _scene1 = new QGraphicsScene(this);

    GradientPalette gp( startColor, endColor );

    // Populate scene
    int yy = 0;
    int nitems = 0;

    Chip::SetWidth( width );
    Chip::SetHeight( height );

    for (int i = 0; i < nrow; i ++) {
        ++yy;
        int xx = 0;
        for (int j = 0; j < ncol; j ++) {
            ++xx;

            int pair_cnt = _fp->output[ pair<int,int>(i,j) ];
            float ratio = min( pair_cnt / 1000.0, 1.0 );
            QColor color = gp.getColor(ratio);

            QGraphicsItem *item = new Chip(color, xx, yy);
            item->setPos(QPointF(j*width, i*height));
            _scene1->addItem(item);

            ++nitems;
        }
    }
    */
    return 1;
}
