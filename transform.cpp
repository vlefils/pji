#include <iostream>
#include <seqan/sequence.h>
#include <string>

using namespace seqan;

template<typename T> T pgcd(T a,T b)
{
  return b ?  pgcd(b,a%b) : a;
}

//return the pattern length
Size<CharString>::Type convertPattern(CharString const& pattern, String<bool> & ret){
  unsigned i;
  Size<CharString>::Type patternLength=0;
  for (i=0; i<length(pattern);++i){
    switch(pattern[i]){
    case '#':ret[i]=true;patternLength++;break;
    default:ret[i]=false;
    }
  }
  return patternLength;
}


template <typename T> Size<DnaString>::Type transformSequence(String<bool> const& pattern,Size<DnaString>::Type patternSize,Size<DnaString>::Type patternFinalSize,DnaString & sequence,T & transformee){
  std::cout << "type non géré" << std::endl;
  return -1;
}


template <> Size<DnaString>::Type transformSequence<DnaString >(String<bool> const& pattern,Size<DnaString>::Type patternSize,Size<DnaString>::Type patternFinalSize,DnaString & sequence,DnaString & transformee){
  Size<DnaString>::Type i;
  Size<DnaString>::Type size;
  Size<DnaString>::Type cpt=0;

  size=length(sequence);

  while(pgcd(patternSize,size)!=1){
    appendValue(sequence,'A');
    size++;
  }

  reserve(transformee,size*patternFinalSize);

  for(i=0; i < size*patternSize; ++i){
    if(pattern[i%patternSize]){
      appendValue(transformee,sequence[i%size]);
      cpt++;
    }
  }
  return i;
}



template <> Size<DnaString>::Type transformSequence<String<uint16_t> >(String<bool> const& pattern,Size<DnaString>::Type patternSize,Size<DnaString>::Type patternFinalSize,DnaString & sequence,String<uint16_t> & transformee){
  Size<DnaString>::Type i;
  Size<DnaString>::Type size;


  if(patternFinalSize>8){
    std::cout<<"impossible d'encoder le resultat sur un long int"<<std::endl;
    return -1;
  }
  else{
    size=length(sequence);

    Size<String<uint16_t> >::Type offset=0;
    Value<String<uint16_t> >::Type tmp=0;


    while(pgcd(patternSize,length(sequence)%patternSize)!=1){
      appendValue(sequence,'A');
      size++;
    }
    size=length(sequence);
    
    reserve(transformee,patternFinalSize*size);

    for(i=0; i < size*patternSize; ++i){
      if(pattern[i%patternSize]){
	tmp+=(ordValue(sequence[i%size]) << (2*offset) ) ;
	offset++;
	if(offset==patternFinalSize){
	  offset=0;
	  appendValue(transformee,tmp);
	  tmp=0;
	}
      }
    }
    return i;
  }
}

int getPosition(Size<CharString>::Type patternSize,Size<CharString>::Type patternFinalSize,Size<DnaString>::Type textSize,int position){
  return (position*patternSize) % textSize;
}
