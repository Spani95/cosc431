#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <regex>
#include <algorithm>

using namespace std;

const string doc_file = "./files/docNo.txt";
const string docSizes_file = "./files/docSizes.txt";
const string terms_file = "./files/terms.txt";
const string postings_file = "./files/postings.txt";

unordered_map<long, pair<string, long> > doc_map;
unordered_map<string, long> term_map;
vector<string> query_vector;

void load_data();
void print(vector<string> str_v);
void query();
void search();
string clean_string(string str);

int main() {
    cout << "Loading." << endl;
    load_data();
    cout << "Finished Loading. \nCtrl-C to stop searching." << endl;
    while(true) {
        query();
        // print(query_vector);
        search();
    }
    return 0;
}

// Maps the data from files: docNo, docSizes and terms. 
void load_data() {
    string::size_type sz; // alias of size_t

    ifstream docNo_input(doc_file);
    ifstream docSize_input(docSizes_file);
    ifstream term_input(terms_file);
    if (!docNo_input || !docSize_input || !term_input) {
        printf("unable to find input file\n");
        exit(1);
    }
    long doc_count = 1;
    string docNo;
    string docSize;
    while(getline(docNo_input, docNo) && getline(docSize_input, docSize)) {
        doc_map[doc_count] = make_pair(docNo, stol(docSize, &sz));
        doc_count++;
    }
    long term_location = 0;
    string term;
    while(getline(term_input, term)) {
        term_map[term] = term_location;
        term_location++;
    }
    docNo_input.close();
    docSize_input.close();
    term_input.close();
}

// Takes user input and puts each word into a vector of strings.
void query() {
    string line;
    cout << "Search: ";
    getline(cin, line);
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> temp(beg, end);
    string word;
    for(vector<string>::size_type i = 0; i != temp.size(); i++) {
        word = clean_string(temp[i]);
        if (word.empty()) { temp.erase(temp.begin() + i); --i; continue; }
        temp[i] = word;
    }
    query_vector = temp;
}

string clean_string(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    str = regex_replace(str, regex("[^a-z]"), "");
    return str;
}

/*  This still needs to be implemented but my idea was to take the location
    of the term in the map and use it to jump to a specific line in the 
    postings file. From there have a function to get all the document numbers
    that would map to the wsj DOCNO's. Started looking into the jumping to 
    a line in file and believe I might need to change to byte encodeing the
    postings so that I could use the seekg function.
*/
void search() {
    for (auto& w : query_vector) {
        if (term_map.count(w) > 0) {
            cout << w << " at location: " << term_map[w] << " in term map." << endl;
        } else {
            cout << w << ": cannot be found" << endl;
        }
    }
}

void print(vector<string> str_v) {
    cout << "Search terms:" << endl;
    for(auto &str : str_v) {
        //cout << doc.first << "=> " << doc.second.first << ", " << doc.second.second << endl;
        cout << str << endl;
    }
}
