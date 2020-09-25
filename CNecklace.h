#ifndef WHYCON_CNECKLACE_H
#define WHYCON_CNECKLACE_H

#include <cstdio>
#include <cstdlib>
#include <cmath>


struct SNecklace
{
  int id;
  int rotation;
  int hamming;
};


class CNecklace
{
  public:
    CNecklace(int bits, int minimalHamming = 1);
    ~CNecklace();
    SNecklace get(int sequence);
    int printAll(int a[]);
    int verifyHamming(int a[], int bits, int len);

  private:
    bool debug;
    int length;
    int idLength;
    SNecklace *idArray;
    SNecklace unknown;
    int *finalList;

    int getHamming(int a, int b);
    int getMinimalHamming(int a, int len);
};

#endif
