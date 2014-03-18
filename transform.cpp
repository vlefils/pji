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

int transformSequence(bool *pattern,int patternSize,int patternFinalSize, String<Dna5> sequence,String<Dna5> *transformee){
  unsigned i,size;

  size=length(sequence);  
  reserve(transformee,patternFinalSize*size);

  for(i = 0; i < size; ++i){
    if(pattern[i%patternSize])*transformee+=sequence[i%size];
  }

  std::cout << "taille tranformee 1er passage : " << length(*transformee) << std::endl; 
  while(pgcd((unsigned int)patternSize,(unsigned int)i%patternSize)!=1){
    sequence+='N';
    size++;
    if(pattern[i%patternSize])*transformee+=sequence[i%size];
    i++;
  }
  std::cout << "taille transformee avec ajout N : " << length(*transformee) << std::endl; 
  size=length(sequence);
  reserve(transformee,patternFinalSize*size);

  for(; i < size*patternFinalSize; ++i){
    if(pattern[i%patternSize])*transformee+=sequence[i%size];
  } 
  std::cout << "taille finale transformee : " << length(*transformee) << std::endl; 
  return i;
}

int transformSequenceToDouble(bool *pattern,int patternSize,int patternFinalSize, String<Dna5> sequence,String<double> *transformee){
  unsigned i,size;

  if(patternFinalSize>21){
    std::cout<<"impossible d'encoder le resultat sur un double"<<std::endl;
    return -1;
  }
  else{
    size=length(sequence);  
    reserve(transformee,patternFinalSize*size);
    int offset=0;
    double tmp=0;

    for(i = 0; i < size; ++i){
      if(pattern[i%patternSize]){
	tmp+=(ordValue(sequence[i%size]) * (3*offset) ) ;
	offset++;
	if(offset==patternFinalSize){
	  offset=0;
	  appendValue(*transformee,tmp);
	}
      }
    }

    std::cout << "taille tranformee 1er passage : " << length(*transformee) << std::endl; 
    while(pgcd((unsigned int)patternSize,(unsigned int)i%patternSize)!=1){
      sequence+='N';
      size++;
      if(pattern[i%patternSize]){
	tmp+=(ordValue(sequence[i%size]) * (3*offset) ) ;
	offset++;
	if(offset==patternFinalSize){
	  offset=0;
	  appendValue(*transformee,tmp);
	}
      }
      i++;
    }
    std::cout << "taille transformee avec ajout N : " << length(*transformee) << std::endl; 
    size=length(sequence);
    reserve(transformee,patternFinalSize*size);

    for(; i < size*patternFinalSize; ++i){
      if(pattern[i%patternSize]){
	tmp+=(ordValue(sequence[i%size]) * (3*offset) ) ;
	offset++;
	if(offset==patternFinalSize){
	  offset=0;
	  appendValue(*transformee,tmp);
	}
      }
    } 
    std::cout << "taille finale transformee : " << length(*transformee) << std::endl; 
    return i;
  }
}

