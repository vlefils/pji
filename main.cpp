#include <string>
#include <sys/time.h>

#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/file.h>
#include <seqan/index.h>
#include <seqan/seeds.h>
#include "transform.cpp"
#include "fileReader.cpp"
#include "chrono.cpp"


using namespace seqan;

int main(int n, char *params[])
{

  /********************** initialization ***************************/
  /********* usage : ./main genome1.fas genome2.fas pattern ********/
  String<bool> pattern;
  Size<CharString>::Type patternSize,patternFinalSize;

  // Build reads and genomes
  String<Dna> genome = readFasFile(params[1]/*"meningitidis_M01-240149.fas"*/);
    
  String<uint16_t> transformee;

  String<Dna> genome2 = readFasFile(params[2]/*"dna.chromosome.IV.fa"*/);

  if(n==4){
    CharString patternString=params[3];
    
    resize(pattern,length(patternString));

    /******************* Pattern convertion ***********************/
    setStartMark();
    patternFinalSize=convertPattern(patternString,pattern);
    patternSize = length(patternString);
    setEndMark();
    std::cout << "pattern converti : "<< patternSize << " (final size : "<< patternFinalSize << ")"<< std::endl;
    displayDuration(); 


    /******************* Genome 1 transformation ******************/
    setStartMark();
    transformSequence<String<uint16_t> >(pattern,patternSize,patternFinalSize,genome, transformee);
    setEndMark();
    std::cout << "sequence transformee : "<< length(transformee) << std::endl;
    displayDuration();
    

    /******************* Index Creation ****************************/
    setStartMark();
    Index<String<uint16_t> ,FMIndex<> > index(transformee);
    indexCreate(index);
    setEndMark();
    std::cout << "sequence indexee" << std::endl;
    displayDuration();
    

    /****************** Genome 2 transformation ********************/
    int repeat=4;
    String<uint16_t> intGenome2;

    setStartMark();
    intGenome2=applyPattern<String<uint16_t> >(genome2,pattern,patternFinalSize);
    setEndMark();
    std::cout<<"pattern appliqué"<<std::endl;
    displayDuration();


    /****************** Main loop **********************************/
    int cpt=0;
    int maxSeedLength=0;
    Size<String<uint16_t> >::Type offset = 0;
    Infix<String<uint16_t> >::Type part;
    Finder<Index<String<uint16_t>,FMIndex<> > > finder(index);
    Score<int, Simple> scoringScheme(1, -1, -1);

    std::cout << "pattern trouve aux position : " << std::endl; 
    setStartMark();
    for(offset=0;(offset+(patternSize*repeat))<length(genome2);++offset){

      //reset the finder
      clear(finder);
      
      part=infix(intGenome2,offset,offset+repeat);

      while (find(finder,part)){
	int partLength = patternSize*repeat;
	Position<Seed<Simple> >::Type hitH = getPosition(patternSize,length(genome),position(finder));
	Position<Seed<Simple> >::Type hitV = getPosition(patternSize,length(genome2),offset);
	Seed<Simple> seed(hitH,hitV,partLength);
	cpt++;

	/*std::cout << "hit : \n"
		  << "genome 1: " << infix(genome, hitH,
					   hitH+partLength) << std::endl
		  << "genome 2: " << infix(genome2, hitV,
		  hitV+partLength) << std::endl;*/
	
	extendSeed(seed,genome , genome2, seqan::EXTEND_BOTH, scoringScheme, 3,
		   seqan::UnGappedXDrop());

	if(seedSize(seed)>maxSeedLength){
	  maxSeedLength=seedSize(seed);
	  std::cout << "allignement maximum : "<<maxSeedLength << std::endl;

	}
	/*std::cout << "extend : \n"
		  << "genome 1: " << infix(genome, beginPositionH(seed),
					   endPositionH(seed)) << std::endl
		  << "genome 2: " << infix(genome2, beginPositionV(seed),
		  endPositionV(seed)) << std::endl;*/

      }
    }
    setEndMark();
    std::cout<<"matchs : "<<cpt<<std::endl;
    displayDuration();
    
  }
  else{
    std::cout << " usage : ./main genome1.fas genome2.fas pattern" << std::endl;
  }

  return 0;
}


// bioinfo.fr Yass
// debian package : blast

// formats : blast / blast tabular


