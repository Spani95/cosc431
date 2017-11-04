#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <utility>

using namespace std;

#define BUFFER_SIZE 2024

unordered_map <string, list<pair<long , long> > > term_map;
list<string> docs;
list<long> doc_sizes;

void map_parser();
void write_to_file();

int main() {
	printf("Mapping parser.\n");
	map_parser();
	printf("Finished mapping, writing to disk:\n");
	write_to_file();
	printf("Finished writing.\n");
	return 0;
} 

/**
Reads the parser_output line by line and maps it to a unordered_map
with a string as its key that maps to a list of pairs that represent
all the documents that contain that term, with the pairs being
the documentNo and the frequency of the term in that document.
*/
void map_parser(){
	FILE *file_handle;
	char term[BUFFER_SIZE];
	bool new_doc = false;
	long docNo = 0;

	file_handle = fopen("./files/parser_output.txt", "r");
	if(!file_handle) {
		fprintf(stderr, "Unable to open file!\n");
        exit(1);
	}

	while (fgets(term, BUFFER_SIZE, file_handle) != NULL){
		if (strcmp(term, "\n") == 0) {
			new_doc = true;
			continue;
		}
		if (new_doc) {
			docs.push_back(term);
			doc_sizes.push_back((long)size_t(term));
			docNo++;
			new_doc = false;
			continue;
		}
		// if (term_map.count(term) == 0) {
		// 	term_map[term].push_back(make_pair(docNo, 1));
		// } else if (term_map[term].back().first == docNo) {
		if (term_map[term].back().first == docNo) {
			term_map[term].back().second++;
		} else {
			term_map[term].push_back(make_pair(docNo, 1));
		}
		doc_sizes.back() += (long)size_t(term);
	}

	if (ferror(file_handle)){
        perror("The following error occured");
    }
	fclose(file_handle);
}

/**
Writes all the data to various files.
*/
void write_to_file(){
	// declaring file handlers.
	FILE *term_file_handle;
	FILE *posting_file_handle;
	FILE *docNo_file_handle;
	FILE *docSizes_file_handle;

	// opening files for writing to.
	term_file_handle = fopen("./files/terms.txt", "w");
	posting_file_handle = fopen("./files/postings.txt", "w");
	docNo_file_handle = fopen("./files/docNo.txt", "w");
	docSizes_file_handle = fopen("./files/docSizes.txt", "w");

	long last_doc;

	/** 
	Writing terms to file "terms.txt".
	The list of pairs for each term is being written to file "postings.txt"
	in the format "unsignedLong-unsignedLong unsignedLong-unsignedLong..."
	->unsignedLong(diffence in documentNo from last documentNo)-unsignedLong(frequency of term in document).
	*/
	printf("Writing terms and postings files...\n");
	for (const auto &term : term_map) {
		last_doc = 0;
		fprintf(term_file_handle, "%s", term.first.c_str());
		auto &postings = term.second;
		for (const auto &post : postings) {
			//printf("Document:%lu, frequency:%lu, %s", post.first - last_doc, post.second, term.first.c_str());
			fprintf(posting_file_handle, "%ld-%ld ", post.first-last_doc, post.second);
			last_doc = post.first;
		}
		fprintf(posting_file_handle, "\n");
	}

	// writing docNo (Document Numbers/ID's) to file "docNo.txt".
	printf("Writing docNo file...\n");
	for (const auto &doc : docs) {
		fprintf(docNo_file_handle, "%s", doc.c_str());
	}

	// writing docSizes to file "docSizes.txt".
	printf("Writing docSizes file...\n");
	for (const auto &size : doc_sizes) {
		fprintf(docSizes_file_handle, "%ld\n", size);
	}

	// closing all files being written.
	fclose(term_file_handle);
	fclose(posting_file_handle);
	fclose(docNo_file_handle);
	fclose(docSizes_file_handle);
}