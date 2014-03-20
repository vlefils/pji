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

  bool *pattern;
  int patternSize,patternFinalSize;

  // Build reads and genomes
  String<Dna5> genome = readFasFile("meningitidis_M01-240149.fas");
    
  String<long int> transformee;

  if(n==2){
    std::string patternString=params[1];
    
    pattern=(bool *)malloc(patternString.length()*sizeof(bool));

    setStartMark();
    patternFinalSize=convertPattern(patternString,pattern);
    patternSize = patternString.length();
    setEndMark();
    std::cout << "pattern converti : "<< patternSize << " (final size : "<< patternFinalSize << ")"<< std::endl;
    displayDuration(); 

    setStartMark();
    transformSequenceToInt(pattern,patternSize,patternFinalSize,genome,&transformee);
    setEndMark();
    std::cout << "sequence transformee : "<< length(transformee) << std::endl;
    displayDuration();

    String<long int> test;
    
    resize(test,3);

    test[0]=1;
    test[1]=2;
    test[2]=3;

    
    setStartMark();
    Index<String<long int> ,FMIndex<> > index(test);
    indexCreate(index);
    setEndMark();
    std::cout << "sequence indexee" << std::endl;
    displayDuration();
    
  
    /*
    Finder<Index<String<long int>,FMIndex<> > > finder(index);
    std::cout << "pattern trouve aux position :"; 
    while (find(finder, 0))
      std::cout << position(finder) << " ";
    std::cout << std::endl;
    */
  }
  else{
    std::cout << "veuillez passer le pattern en paramètre (exemple '##_')" << std::endl;
  }

  return 0;
}

