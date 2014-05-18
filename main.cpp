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
  String<Dna> genome1 = readFasFile(params[1]/*"meningitidis_M01-240149.fas"*/);
    
  String<uint16_t> genome1Transfo;

  String<Dna> genome2 = readFasFile(params[2]/*"dna.chromosome.IV.fa"*/);

  String<uint16_t> genome2Transfo;

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
    transformSequence<String<uint16_t> >(pattern,patternSize,patternFinalSize,genome1,genome1Transfo);
    setEndMark();
    std::cout << "génome 1 transformé : "<< length(genome1Transfo) << std::endl;
    displayDuration();
    

    /******************* Index Creation ****************************/
    setStartMark();
    Index<String<uint16_t> ,FMIndex<> > index(genome1Transfo);
    indexCreate(index);
    setEndMark();
    std::cout << "genome 1 indexé" << std::endl;
    displayDuration();
    

    /****************** Genome 2 transformation ********************/
    int repeat=4;
    String<uint16_t> genome2transfo;

    setStartMark();
    transformSequence<String<uint16_t> >(pattern,patternSize,patternFinalSize,genome2,genome2Transfo);
    setEndMark();
    std::cout<<"genome 2 transformé : "<< length(genome2Transfo) <<std::endl;
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

    std::cout << "n° \t\t" 
	      << "corresp %\t\t" 
	      << "posH(deb)\t"
	      << "posH(fin)\t"
	      << "posV(deb)\t"
	      << "posV(fin)\t"
	      << "score\t"
	      << std::endl;

    for(offset=0;offset<length(genome2);++offset){

      //reset the finder
      clear(finder);
      
      part=infix(genome2Transfo,offset,offset+repeat);

      while (find(finder,part)){
	int partLength = patternSize*repeat;
	Position<Seed<Simple> >::Type hitH = getPosition(patternSize,patternFinalSize,length(genome1),position(finder));
	Position<Seed<Simple> >::Type hitV = getPosition(patternSize,patternFinalSize,length(genome2),offset);
	Seed<Simple> seed(hitH,hitV,partLength);
	cpt++;

       
	extendSeed(seed,genome1 , genome2, seqan::EXTEND_BOTH,scoringScheme,1,seqan::GappedXDrop());

	if(seedSize(seed)>maxSeedLength){
	  maxSeedLength=seedSize(seed);
	  std::cout << cpt << "\t\t" 
		    << (double)seedSize(seed)/(double)length(genome1)*100 << "%\t\t" 
		    << getPosition(patternSize,patternFinalSize,length(genome1),beginPositionH(seed)) << "\t\t"
		    << getPosition(patternSize,patternFinalSize,length(genome1),endPositionH(seed)) << "\t\t"
		    << getPosition(patternSize,patternFinalSize,length(genome2),beginPositionV(seed)) << "\t\t"
		    << getPosition(patternSize,patternFinalSize,length(genome2),endPositionV(seed)) << "\t\t"
		     /*TODO find score*/ << "\t\t"
		    << std::endl;
	}
	

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


