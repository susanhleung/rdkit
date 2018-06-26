#include <GraphMol/MolStandardize/TransformCatalog/TransformCatalogParams.h>
#include <GraphMol/MolStandardize/TransformCatalog/TransformCatalogUtils.h>
#include "Normalize.h"
#include <RDGeneral/Invariant.h>
#include <GraphMol/RDKitBase.h>
#include <GraphMol/ROMol.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <RDGeneral/FileParseException.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <GraphMol/MolStandardize/MolStandardize.h>

#include <iostream>
#include <fstream>

using namespace RDKit;
using namespace MolStandardize;

void test1() {

	std::string smi1, smi2, smi3, smi4, smi5, smi6, smi7;

	// testing parsing of transform catalog
	std::string rdbase = getenv("RDBASE");
	std::string transformFile = 
		rdbase + "/Code/GraphMol/MolStandardize/TransformCatalog/test_data/normalizations.txt";
	auto *tparams = new TransformCatalogParams(transformFile);
	unsigned int ntransforms = tparams->getNumTransformations();
	TEST_ASSERT(ntransforms == 22);

	TransformCatalog tcat(tparams);
	Normalizer normalizer;

	// Test sulfoxide normalization.
	smi1 = "CS(C)=O";
	std::shared_ptr<ROMol> m1( SmilesToMol(smi1) );
	std::cout << "Before normalizing: " << MolToSmiles(*m1) << std::endl;
	ROMol* normalized = normalizer.normalize(*m1, &tcat);
	std::cout << MolToSmiles(*normalized) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized) == "C[S+](C)[O-]");

	// Test sulfone
	smi2 = "C[S+2]([O-])([O-])O";
	std::shared_ptr<ROMol> m2( SmilesToMol(smi2) );
	std::cout << "Before normalizing: " << MolToSmiles(*m2) << std::endl;
	ROMol* normalized2 = normalizer.normalize(*m2, &tcat);
	std::cout << MolToSmiles(*normalized2) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized2) == "CS(=O)(=O)O");

	// Test 1,3-separated charges are recombined.
	smi3 = "CC([O-])=[N+](C)C";
	std::shared_ptr<ROMol> m3( SmilesToMol(smi3) );
	std::cout << "Before normalizing: " << MolToSmiles(*m3) << std::endl;
	ROMol* normalized3 = normalizer.normalize(*m3, &tcat);
	std::cout << MolToSmiles(*normalized3) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized3) == "CC(=O)N(C)C");

	// Test 1,3-separated charges are recombined.
	smi4 = "C[n+]1ccccc1[O-]";
	std::shared_ptr<ROMol> m4( SmilesToMol(smi4) );
	std::cout << "Before normalizing: " << MolToSmiles(*m4) << std::endl;
	ROMol* normalized4 = normalizer.normalize(*m4, &tcat);
	std::cout << MolToSmiles(*normalized4) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized4) == "Cn1ccccc1=O");

	// Test a case where 1,3-separated charges should not be recombined.
	smi5 = "CC12CCCCC1(Cl)[N+]([O-])=[N+]2[O-]";
	std::shared_ptr<ROMol> m5( SmilesToMol(smi5) );
	std::cout << "Before normalizing: " << MolToSmiles(*m5) << std::endl;
	ROMol* normalized5 = normalizer.normalize(*m5, &tcat);
	std::cout << MolToSmiles(*normalized5) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized5) == "CC12CCCCC1(Cl)[N+]([O-])=[N+]2[O-]");

	// Test 1,5-separated charges are recombined.
	smi6 = "C[N+](C)=C\C=C\[O-]";
	std::shared_ptr<ROMol> m6( SmilesToMol(smi6) );
	std::cout << "Before normalizing: " << MolToSmiles(*m6) << std::endl;
	ROMol* normalized6 = normalizer.normalize(*m6, &tcat);
	std::cout << MolToSmiles(*normalized6) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized6) == "CN(C)C=CC=O");

	// Test a case where 1,5-separated charges should not be recombined.
	smi7 = "C[N+]1=C2C=[N+]([O-])C=CN2CCC1";
	std::shared_ptr<ROMol> m7( SmilesToMol(smi7) );
	std::cout << "Before normalizing: " << MolToSmiles(*m7) << std::endl;
	ROMol* normalized7 = normalizer.normalize(*m7, &tcat);
	std::cout << MolToSmiles(*normalized7) << std::endl;
	TEST_ASSERT(MolToSmiles(*normalized7) == "C[N+]1=C2C=[N+]([O-])C=CN2CCC1");



}

int main() {
	test1();
	return 0;
}
