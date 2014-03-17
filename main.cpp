#include <string>
#include <sys/time.h>

#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/file.h>
#include <seqan/index.h>
#include "transform.cpp"
#include "fileReader.cpp"
#include "chrono.cpp"


using namespace seqan;

int main(int, char const **)
{

  bool *pattern;
  int patternLength;

  // Build reads and genomes
  String<Dna5> genome = readFasFile("meningitidis_M01-240149.fas");
    
  String<Dna5> transformee;
    
  pattern=(bool *)malloc(4*sizeof(bool));

  setStartMark();
  patternLength=convertPattern("_#_#_#_####",pattern);
  setEndMark();
  std::cout << "pattern converti : "<< patternLength << std::endl;
  displayDuration(); 

  setStartMark();
  transformSequence(pattern,patternLength,genome,&transformee);
  setEndMark();
  std::cout << "sequence transformee : "<< length(transformee) << std::endl;
  displayDuration();


  setStartMark();
  Index<String<Dna5> ,FMIndex<> > index(transformee);
  setEndMark();
  std::cout << "sequence indexee" << std::endl;
  displayDuration();
  
  /*
    Finder<Index<String<Dna5>,FMIndex<> > > finder(index);
    std::cout << "pattern trouve aux position :"; 
    while (find(finder, "TTAATTAATTAA"))
    std::cout << position(finder) << " ";
    std::cout << std::endl;
  */
  return 0;
}

