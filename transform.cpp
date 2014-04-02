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

  std::cout << "taille sequence avant ajout A : " << length(sequence) << std::endl;

  while(pgcd(patternSize,size)!=1){
    appendValue(sequence,'A');
    size++;
  }
  std::cout << "taille sequence avec ajout A : " << length(sequence) << std::endl;

  reserve(transformee,size*patternFinalSize);

  for(i=0; i < size*patternSize; ++i){
    if(pattern[i%patternSize]){
      appendValue(transformee,sequence[i%size]);
      cpt++;
    }
  }
  std::cout << "taille finale transformee : " << length(transformee) << std::endl;
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

    Value<String<uint16_t> >::Type offset=0;
    Value<String<uint16_t> >::Type tmp=0;

    std::cout << "taille sequence avant ajout A : " << length(sequence) << std::endl;

    while(pgcd(patternSize,length(sequence)%patternSize)!=1){
      appendValue(sequence,'A');
      size++;
    }
    std::cout << "taille transformee avec ajout N : " << length(transformee) << std::endl;
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
    std::cout << "taille finale transformee : " << length(transformee) << std::endl;
    return i;
  }
}

/*TODO : ne peux tu pas faire une fonction générique :
  template <typename TAlphabet, typename TWord> Size<String<TAlphabet> >::Type transformSequence(
  String<bool> const& pattern,
  Size<String<TAlphabet> >::Type patternSize,
  Size<String<TAlphabet> >::Type patternFinalSize,
  String<TAlphabet> & sequence,
  String<TWord> & transformee)
  qui avec n'importe quelle alphabet d'entrée(TAlphabet) et d'alphabet de sortie (TWord) est capable de faire ta transformation ?
  Ca te permettra de passer à des mots plus court ou plus long (si seqan change son fonctionnement)sans avoir a réécrire completement ta fonction.
*/
