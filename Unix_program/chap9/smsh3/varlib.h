#ifndef  __VARLIB_H__
#define __VARLIB_H__

int VLstore(char *, char *);
char *new_string();
char *VLlookup();
int VLexport();
void VLlist();
int VLenviron2table();
char **VLtable2environ();



#endif
