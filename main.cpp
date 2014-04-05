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

int main(int n, char *params[])
{

  String<bool> pattern;
  Size<CharString>::Type patternSize,patternFinalSize;

  // Build reads and genomes
  String<Dna> genome = readFasFile("meningitidis_M01-240149.fas");
    
  String<uint16_t> transformee;

  if(n==2){
    CharString patternString=params[1];
    
    resize(pattern,length(patternString));

    setStartMark();
    patternFinalSize=convertPattern(patternString,pattern);
    patternSize = length(patternString);
    setEndMark();
    std::cout << "pattern converti : "<< patternSize << " (final size : "<< patternFinalSize << ")"<< std::endl;
    displayDuration(); 

    setStartMark();
    transformSequence<String<uint16_t> >(pattern,patternSize,patternFinalSize,genome, transformee);
    setEndMark();
    std::cout << "sequence transformee : "<< length(transformee) << std::endl;
    displayDuration();
    
    setStartMark();
    Index<String<uint16_t> ,FMIndex<> > index(transformee);
    indexCreate(index);
    setEndMark();
    std::cout << "sequence indexee" << std::endl;
    displayDuration();
    
 
    Finder<Index<String<uint16_t>,FMIndex<> > > finder(index);
    String<uint16_t> seed;
    appendValue(seed,8);
    appendValue(seed,12);

    
    std::cout << "pattern trouve aux position :"; 
    while (find(finder,seed))
      std::cout << getPosition(length(genome),patternFinalSize,position(finder)) << " ";
    std::cout << std::endl;
    
  }
  else{
    std::cout << "veuillez passer le pattern en paramètre (exemple '##_')" << std::endl;
  }

  return 0;
}
