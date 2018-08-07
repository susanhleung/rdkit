//
//  Copyright (C) 2018 Susan H. Leung
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#ifndef __RD_CHARGE_H__
#define __RD_CHARGE_H__

#include "MolStandardize.h"
#include <Catalogs/Catalog.h>
#include <GraphMol/MolStandardize/AcidBaseCatalog/AcidBaseCatalogEntry.h>
#include <GraphMol/MolStandardize/AcidBaseCatalog/AcidBaseCatalogParams.h>

namespace RDKit {
class RWMol;
class ROMol;

namespace MolStandardize {

extern const CleanupParameters defaultCleanupParameters;

typedef RDCatalog::HierarchCatalog<AcidBaseCatalogEntry, AcidBaseCatalogParams,
                                   int>
    AcidBaseCatalog;

struct ChargeCorrection {
  std::string Name;
  std::string Smarts;
  int Charge;

  ChargeCorrection(std::string name, std::string smarts, int charge)
      : Name(name), Smarts(smarts), Charge(charge) {}
};

// The default list of ChargeCorrections.
extern std::vector<ChargeCorrection> CHARGE_CORRECTIONS;

class Reionizer {
  // A class to fix charges and reionize a molecule such that the strongest
  // acids
  //  ionize first.

 public:
	Reionizer();
	Reionizer(const std::string acidbaseFile);
	Reionizer(const std::string acidbaseFile, const std::vector<ChargeCorrection> ccs);
	Reionizer(const Reionizer &other);
	~Reionizer();

//	Reionizer(const AcidBaseCatalog *abcat, const std::vector<ChargeCorrection> ccs = CHARGE_CORRECTIONS);

  ROMol *reionize(const ROMol &mol);

 private:
	AcidBaseCatalog *d_abcat;
	std::vector<ChargeCorrection> d_ccs;

  std::pair<unsigned int, std::vector<unsigned int>> *strongestProtonated(
      const ROMol &mol,
      const std::vector<std::pair<ROMOL_SPTR, ROMOL_SPTR>> &abpairs);
  std::pair<unsigned int, std::vector<unsigned int>> *weakestIonized(
      const ROMol &mol,
      const std::vector<std::pair<ROMOL_SPTR, ROMOL_SPTR>> &abpairs);

};  // Reionizer class

class Uncharger {
  // Class for neutralizing ionized acids and bases.

 public:
  Uncharger();
  Uncharger(const Uncharger &other);
  ~Uncharger();

  ROMol *uncharge(const ROMol &mol);

 private:
  std::shared_ptr<ROMol> pos_h;
  std::shared_ptr<ROMol> pos_quat;
  std::shared_ptr<ROMol> neg;
  std::shared_ptr<ROMol> neg_acid;
};  // Uncharger class

}  // namespace MolStandardize
}  // namespace RDKit
#endif
