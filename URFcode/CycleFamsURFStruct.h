/* Niek Andresen 2015 - Bachelor Thesis */

#ifndef CFDATASTRUCT
#define CFDATASTRUCT

/** a cycle family */
typedef struct{
    int weight; /*number of edges/vertices in the cf (length of one cycle)*/
    int r, p, q, x; /*vertices defining a cf as in Vismara's definition*/
    char *prototype; /*prototype vector {0,1}^m*/
    int mark; /*mark CF as relevant*/
}cfam;

/** contains the Cycle Families */
typedef struct{
    cfam **fams; /*array of CFs sorted by weight*/
    int nofFams; /*number of cycle families*/
}cfURF;

#endif
