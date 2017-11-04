Search program is not fully finished however shows the direction I was heading in.

Compiling:
if wanting to remake files {
   1. make director called "file".
   2. run bash script that will compile all the programs.
   3. run ./parser <xml document> (parse xml document outputting to a file)
   4. run ./indexer (makes the inverted index)
   5. run ./search (at this point only outputs the line number(s) in the postings file that belongs to the search term(s))

}
else {
   1. run bash script that will compile all the programs.
   2. run ./search (at this point only outputs the line number(s) in the postings file that belongs to the search term(s))
}
