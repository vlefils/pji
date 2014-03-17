#include <fstream>
#include <iostream>

#include <seqan/seq_io.h>
#include <seqan/sequence.h>

String<Dna5> readFasFile(char *file)
{
    // Open file and create RecordReader.
    std::fstream in(file, std::ios::binary | std::ios::in);
    seqan::RecordReader<std::fstream, seqan::SinglePass<> > reader(in);

    // Read file record-wise.
    seqan::CharString id;
    seqan::Dna5String seq;
    while (!atEnd(reader))
    {
        if (readRecord(id, seq, reader, seqan::Fasta()) != 0)
            return 1;  // Could not record from file.


    }
    return seq;
}
