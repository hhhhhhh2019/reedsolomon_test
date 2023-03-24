#ifndef REEDSOLOMON_H
#define REEDSOLOMON_H


#include <gpoly.h>


GPoly encode(GPoly, GPoly, int, int);
GPoly decode(GPoly, GPoly, int, int);
GPoly gen_g(int);


#endif // REEDSOLOMON_H
