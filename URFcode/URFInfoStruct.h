/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef URFINFODATASTRUCT
#define URFINFODATASTRUCT

typedef struct{
  int nofWeights; /*number of different weights of cycle families occuring*/
  int *nofProtos; /*array storing the number of prototypes for each weight*/
  char ***URFrel; /*array of 2D-matrices. For each weight that a CF can have
                  there is a matrix that stores 1 at position [i,j] if RCFs i
                  and j (of this particular weight) are URF-related and 0
                  otherwise.*/
  int nofURFs; /*number of URFs*/
  cfam ***URFs; /*array of tuples of RCFs that belong to the same URF*/
  int *nofCFsPerURF; /*array that stores the number of RCFs that belong to each
                     URF*/
} URFinfo;

#endif
