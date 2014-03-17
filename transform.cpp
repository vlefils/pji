#include <iostream>
#include <seqan/sequence.h>
#include <string>

using namespace seqan;

unsigned int pgcd(unsigned int a,unsigned int b)
{
    return b ?  pgcd(b,a%b) : a;
}

//return the pattern length
int convertPattern(std::string pattern,bool *ret){
  unsigned i;

  for (i=0; i<pattern.length();++i){
    switch(pattern[i]){
    case '#':ret[i]=true;break;
    default:ret[i]=false;		
    }
  }
  return i;
}

int transformSequence(bool *pattern,int patternLength, String<Dna5> sequence,String<Dna5> *transformee){
  unsigned i,size;

  size=length(sequence);

  for(i = 0; i < size; ++i){
    if(pattern[i%patternLength])*transformee+=sequence[i%size];
  }

  //std::cout << "taille tranformee 1er passage : " << length(*transformee) << std::endl; 
  while(pgcd((unsigned int)patternLength,(unsigned int)i%patternLength)!=1){
    sequence+='N';
    size++;
    if(pattern[i%patternLength])*transformee+=sequence[i%size];
    i++;
  }
  //std::cout << "taille transformee avec ajout N : " << length(*transformee) << std::endl; 
  size=length(sequence);

  for(; i < size*patternLength; ++i){
    if(pattern[i%patternLength])*transformee+=sequence[i%size];
  } 
  //std::cout << "taille finale transformee : " << length(*transformee) << std::endl; 
  return i;
}


/* exemple d'util */
/*
int main(int, char const **)
{
  bool *pattern;
  int patternLength;

  // Build reads and genomes
  String<Dna> genome = "ATTAATTAATTAATTAATTAATTAATTA";
    
  String<Dna> transformee;
    
  pattern=(bool *)malloc(4*sizeof(bool));

  patternLength=convertPattern("_##_",pattern);
    
  transformSequence(pattern,genome,transformee);

  std::cout << genome << std::endl;

    
  std::cout << transformee << std::endl;  
    
  return 1;
}

*/
