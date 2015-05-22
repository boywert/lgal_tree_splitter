#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

struct halo_data {
  /* merger tree pointers */
  int Descendant;
  int FirstProgenitor;
  int NextProgenitor;
  int FirstHaloInFOFgroup;
  int NextHaloInFOFgroup;
  /* properties of halo */
  int Len;
  float M_Mean200, M_Crit200, M_TopHat;
  float Pos[3];
  float Vel[3];
  float VelDisp;
  float Vmax;
  float Spin[3];
  long long MostBoundID;
  /* original position in subfind output */
  int SnapNum;
  int FileNr;
  int SubhaloIndex;
  float SubHalfMass;
};
struct  halo_ids_data {
  long long HaloID;
  long long FileTreeNr;
  long long FirstProgenitor;
  long long LastProgenitor;
  long long NextProgenitor;
  long long Descendant;
  long long FirstHaloInFOFgroup;
  long long NextHaloInFOFgroup;
#ifdef MRII
  long long MainLeafID; 
#endif
  double    Redshift;
  int       PeanoKey;
  int       dummy;
};

int main(int argc, char **argv) {
  int n_in,n_out;
  char folder_in[1024],folder_out[1024];
  int lastsnap;
  int i,j;
  char command[1024];
  int this_nTrees;
  int this_nHalos;
  int *this_nTreeHalos;
  int64_t start_nTree, nTrees, nHalos, HalosFile;
  int *nTreeHalos,*cum_TreeHalos, *file_endtree;
  FILE *fp1,*fp2;
  struct halo_data *halos;
  struct halo_ids_data *halo_ids;
  char filename_tree[1024];
  char filename_treeids[1024];
  n_in = 1;
  n_out = 32;
  lastsnap = 107;
  snprintf(folder_in,1024,"treedata/");
  snprintf(folder_out,1024,"treedata/treedata/");
  snprintf(command,1024,"mkdir -p %s",folder_out);
  system(command);
  nTrees = 0;
  nHalos = 0;
  nTreeHalos = malloc(0);
  for(i=0;i<n_in;i++) {
    start_nTree = nTrees;
    snprintf(filename_tree,1024,"%s/trees_%03d.%d",folder_in,lastsnap,i);
    fp = fopen(filename_tree,"rb");
    fread(&this_nTrees,sizeof(int),1,fp);
    nTrees += this_nTrees;
    fread(&this_nHalos,sizeof(int),1,fp);
    nHalos += this_nHalos;
    nTreeHalos = realloc(nTreeHalos,nTrees*sizeof(int));
    fread(&nTreeHalos[start_nTree],this_nTrees,sizeof(int),fp);
    fclose(fp);
  }
  cum_TreeHalos = malloc(nTrees*sizeof(int));
  cum_TreeHalos[0] = 0;
  for(i=0;i<nTrees;i++) 
    cum_TreeHalos[i] = cum_TreeHalos[i-1]+nTreeHalos[i];

  HalosFile = nHalos/n_out;
  file_endtree = malloc(n_out*sizeof(int));
  for(i=0,j=0;j<nTrees;j++) {
    if(cum_TreeHalos[j] >= (i+1)*HalosFile) {
      file_endtree[i] = cum_TreeHalos[j];
      printf("file %d = %d\n",i,file_endtree[i]);
      i++;
    }
  }
  return 0;
}
