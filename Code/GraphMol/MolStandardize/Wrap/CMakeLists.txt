rdkit_python_extension(rdMolStandardize rdMolStandardize.cpp
                       DEST Chem
                       LINK_LIBRARIES
LINK_LIBRARIES MolStandardize Descriptors PartialCharges Fingerprints FileParsers SmilesParse Subgraphs
SubstructMatch MolTransforms GraphMol EigenSolvers DataStructs RDGeneral RDGeometryLib RDBoost )

add_pytest(pyMolStandardize ${CMAKE_CURRENT_SOURCE_DIR}/testMolStandardize.py)
