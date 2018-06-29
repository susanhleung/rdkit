#include "AcidBaseCatalogUtils.h"
#include <RDGeneral/BadFileException.h>
#include <boost/tokenizer.hpp>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
#include <fstream>
#include <string>

namespace RDKit {
namespace {
std::pair<ROMol*, ROMol*>* getPair(const std::string &tmpStr) {
	std::pair<ROMol*, ROMol*>* mol_pair = nullptr;
	if (tmpStr.length() == 0) {
    // empty line
    return mol_pair;
  }
  if (tmpStr.substr(0, 2) == "//") {
    // comment line
    return mol_pair;
  }

  boost::char_separator<char> tabSep("\t");
  tokenizer tokens(tmpStr, tabSep);
  tokenizer::iterator token = tokens.begin();

  // name of the functional groups
  std::string name = *token;
  boost::erase_all(name, " ");
  ++token;

  // grab the acid :
  std::string acid_smarts = *token;
  boost::erase_all(acid_smarts, " ");
  ++token;

	// grab the base:
  std::string base_smarts = *token;
  boost::erase_all(base_smarts, " ");
  ++token;

	ROMol* acid = SmartsToMol(acid_smarts);
	ROMol* base = SmartsToMol(base_smarts);
  CHECK_INVARIANT(acid, acid_smarts);
  CHECK_INVARIANT(base, base_smarts);
  acid->setProp(common_properties::_Name, name);
  base->setProp(common_properties::_Name, name);
//  transformation->setProp(common_properties::_SMIRKS, smirks); // TODO
//  RDGeneral/types.h does not have a common property to use?...
	mol_pair = new std::pair<ROMol*, ROMol*>(acid, base) ;
  return mol_pair;
}
} // end of local utility namespace
	


namespace MolStandardize {

std::vector<std::pair<ROMol*, ROMol*>> readPairs(std::string fileName) {
	std::ifstream inStream(fileName.c_str());
	if ((!inStream) || (inStream.bad())) {
		std::ostringstream errout;
		errout << "Bad input file " << fileName;
		throw BadFileException(errout.str());
	}
	std::vector<std::pair<ROMol*, ROMol*>> mol_pairs = readPairs(inStream);
	return mol_pairs;
}

std::vector<std::pair<ROMol*, ROMol*>> readPairs(std::istream &inStream, int nToRead) {
	std::vector<std::pair<ROMol*, ROMol*>> pairs;
	pairs.clear();
	if (inStream.bad()) {
		throw BadFileException("Bad stream contents.");
	}
	const int MAX_LINE_LEN = 512;
	char inLine[MAX_LINE_LEN];
	std::string tmpstr;
	int nRead = 0;
	while (!inStream.eof() && (nToRead < 0 || nRead < nToRead)) {
		inStream.getline(inLine, MAX_LINE_LEN, '\n');
		tmpstr = inLine;
		// parse the molpair on this line (if there is one)
		std::pair<ROMol*, ROMol*>* mol_pair = getPair(tmpstr) ;
		if (mol_pair != nullptr) {
			pairs.push_back(*mol_pair);
			nRead++;
		}
	}
	return pairs;
}

} // namespace MolStandardize
} // namespace RDKit
