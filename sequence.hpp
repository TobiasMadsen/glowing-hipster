#ifndef __sequence_h
#define __sequence_h

#include <memory>
#include <string>
#include <map>
#include <fstream>

namespace FP{
  class Sequence; //forward declaration

  typedef std::shared_ptr<Sequence> SequencePtr;

  class Sequence{
  public:
    virtual int next(std::string) = 0;
  };

  class SequenceMap : public Sequence{
  public:
    int next(std::string name);
    void addSeq(std::string name, int value);

  private:
    std::map<std::string, int> sequences;
  };

  class SequenceStream{
    // Reads characters from filestream
    // Assuming correct order: That is the same order
    // as the sequences appears in the newick file
    // will skip newline characters
  public:
    SequenceStream(std::string filename);
    int next(std::string);

  private:
    std::fstream file;
    std::istream_iterator<char> fileIterator;
  };

  class SequenceDummy{
    // Returns 0 regardless
    // Convenient for testing
  public:
    int next(std::string){
      return 0;
    }
  };
}

#endif // __sequence_h
