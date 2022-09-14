/* Copy input to output until '.' or EOF.
   The '.' itself not copied.
*/

#include <stdio.h>
#include <string.h>

#define sz 8
char buf[sz];

main()
{
    char bcnt;
    char *p;

    bcnt = fread(buf, 1, sz, stdin);

    int inOverflow = bcnt > sz - 1;
    if (inOverflow) bcnt = sz - 1;

    buf[bcnt+1] = 0;
    p = strchr(buf, '.');	// p = NULL if not found

    if (inOverflow && !p) {
        fprintf(stderr, "?-input file size > %d and \'.\' char not before there", sz - 1);
        return -1;
      }

    if (p) {
      bcnt = p - buf;
    }
    fwrite(buf, 1, bcnt, stdout);
    return 0;
}
