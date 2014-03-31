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
  int length=0;
  for (i=0; i<pattern.length();++i){
    switch(pattern[i]){
    case '#':ret[i]=true;length++;break;
    default:ret[i]=false;		
    }
  }
  return length;
}

template <typename T> int transformSequence(bool *pattern,int patternSize,int patternFinalSize, String<Dna> sequence,T *transformee){
  std::cout << "type non géré" << std::endl;
  return -1;
}


template <> int transformSequence<String<Dna> >(bool *pattern,int patternSize,int patternFinalSize, String<Dna> sequence,String<Dna> *transformee){
  unsigned i,size;
  int cpt=0;

  size=length(sequence);

  std::cout << "taille sequence avant ajout A : " << length(sequence) << std::endl; 

  while(pgcd((unsigned int)patternSize,(unsigned int)size)!=1){
    sequence+='A';
    size++;
  }
  std::cout << "taille sequence avec ajout A : " << length(sequence) << std::endl; 

  reserve(transformee,size*patternFinalSize);

  for(i=0; i < size*patternSize; ++i){
    if(pattern[i%patternSize]){
      *transformee+=sequence[i%size];
      cpt++;
    }
  } 
  std::cout << "taille finale transformee : " << length(*transformee) << std::endl; 
  return i;
}



template <> int transformSequence<String<uint16_t> >(bool *pattern,int patternSize,int patternFinalSize, String<Dna> sequence,String<uint16_t> *transformee){
  unsigned i,size;

  if(patternFinalSize>8){
    std::cout<<"impossible d'encoder le resultat sur un long int"<<std::endl;
    return -1;
  }
  else{
    size=length(sequence);  

    int offset=0;
    double tmp=0;

    std::cout << "taille sequence avant ajout A : " << length(sequence) << std::endl; 

    while(pgcd((unsigned int)patternSize,(unsigned int)length(sequence)%patternSize)!=1){
      sequence+='A';
      size++;
    }
    std::cout << "taille transformee avec ajout N : " << length(*transformee) << std::endl; 
    size=length(sequence);
    reserve(transformee,patternFinalSize*size);

    for(i=0; i < size*patternSize; ++i){
      if(pattern[i%patternSize]){
	tmp+=(ordValue(sequence[i%size]) << (2*offset) ) ;
	offset++;
	if(offset==patternFinalSize){
	  offset=0;
	  appendValue(*transformee,tmp);
	  tmp=0;
	}
      }
    } 
    std::cout << "taille finale transformee : " << length(*transformee) << std::endl; 
    return i;
  }
}

