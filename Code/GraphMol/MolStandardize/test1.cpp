#include "MolStandardize.h"
#include <RDGeneral/Invariant.h>
#include <GraphMol/RDKitBase.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/ROMol.h>

#include <iostream>

using namespace RDKit;
using namespace std;

void testStandardize(){
	string smi;
	CleanupParameters *params;

	smi = "CCCC(=O)O";
	unique_ptr<RWMol> m( SmilesToMol(smi) );
	unique_ptr<RWMol> m2( SmilesToMol(smi) );
	TEST_ASSERT(m);
	// empty cleanup function
	MolStandardize::cleanup(*m, *params);
	// testing nothing has changed
	TEST_ASSERT(MolToSmiles(*m) == MolToSmiles(*m2));
}

int main() {
	testStandardize();
	return 0;
}
