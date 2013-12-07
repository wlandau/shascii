/*
SHASCII: Show Hidden ASCII
@version 1.01
@license: GNU General Public License: Version 3, 29 June 2007
@file shascii.c
Depends: 
  stdio.h
  stdlib.h
  string.h
  unistd.h
@author Will Landau
@email will.landau@gmail.com
Date created: March 22, 2013
Web: http://will-landau.com
     http://github.com/wlandau
Compile and run with:     
  make
  make install
  shascii -a
  shascii -h
  shascii -f [input_file] 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 8192

void about(){
  printf("SHASCII (Show Invisible ASCII) reads a text file and prints\n");
  printf("out the same text withall the original ASCII control characters\n");
  printf("shown explicitly. The outputshows control characters in C language\n");
  printf("character escape code notation when possible:for example, \n"); 
  printf("the line feed character is shown, literally, as \\n. For control\n");
  printf("characters with no C language escape notation, caret notation\n");
  printf("is used. If the -c flag is set, caret notation is used for all ASCII\n");
  printf("control characters. All non-control and non-ASCII characters are left alone.\n\n");
  printf("See http://http://en.wikipedia.org/wiki/Ascii for an explanation of ASCII\n");
  printf("control characters, ASCII decimal and hexidecimal codes, and ASCII iteslf.\n\n");
  printf("SHASCII project page: https://github.com/wlandau/shascii.\n\n");
  exit(EXIT_SUCCESS);
}

void usage(){
  printf("\nUsage:\n");
  printf("  shascii [flag_1] [arg_1] ... [flag_n] [arg_n] \n\n");
  printf("Options:\n");
  printf("  Flag \tArg       \t Description\n");
  printf("  -a               \t About.\n");
  printf("  -c               \t Print out all ASCII control characters in caret notation.\n");
  printf("  -d \t[dec_index] \t Break up output into lines by the ASCII character with \n");
  printf("                 \t decimal code [dec_index].\n");
  printf("  -f \t[file]  \t Use contents of [file] as input.\n");
  printf("  -h               \t Usage.\n");
  printf("  -p         \t\t Pipe input from another program through stdin.\n");
  printf("  -t \t[text]     \t Use [text] as input.\n");
  printf("  -x \t[hex_index] \t Break up output into lines by the ASCII character with \n");
  printf("                 \t hexidecimal code [hex_index].\n\n");
  exit(EXIT_SUCCESS);
}

void handle_error(char* message){
  printf("OH NO! %s\nFor usage, enter: shascii -h\n", message);
  exit(EXIT_FAILURE);
}

void read_file(char **input, char* filename){
  int n;
  FILE *in;
  
  if((in = fopen(filename, "r")) == NULL)
    handle_error("Input file %s does not exist.\n");
  
  fseek(in, 0L, SEEK_END);
  n = ftell(in);
  rewind(in);
  
  *input = (char*) calloc(n + 1, sizeof(char));
  fread(*input, sizeof(char), n, in);
  
  fclose(in);
}

void one_sub(char **buf, char c, int cflag){
  if(c == 0x0)       /* null character */
    strcpy(*buf, cflag ? "^@" : "\\0");
  else if(c == 0x1)  /* start of header */
    strcpy(*buf, "^A");
  else if(c == 0x2)  /* start of text */
    strcpy(*buf, "^B");
  else if(c == 0x3)  /* end of text */
    strcpy(*buf, "^C");
  else if(c == 0x4)  /* end of transmission */
    strcpy(*buf, "^D");
  else if(c == 0x5)  /* enquiry */
    strcpy(*buf, "^E");
  else if(c == 0x6)  /* acknowledgement */
    strcpy(*buf, "^F");
  else if(c == 0x7)  /* bell */
    strcpy(*buf, cflag ? "^G" : "\\a");
  else if(c == 0x8)  /* backspace */
    strcpy(*buf, cflag ? "^H" : "\\b");
  else if(c == 0x9)  /* horizontal tab */
    strcpy(*buf, cflag ? "^I" : "\\t");
  else if(c == 0xa)  /* line feed */
    strcpy(*buf, cflag ? "^J" : "\\n");   
  else if(c == 0xb)  /* vertical tab */
    strcpy(*buf, cflag ? "^K" : "\\v");
  else if(c == 0xc)  /* form feed */
    strcpy(*buf, cflag ? "^L" : "\\f");
  else if(c == 0xd)  /* carriage return */
    strcpy(*buf, cflag ? "^M" : "\\r");
  else if(c == 0xe)  /* shift out */
    strcpy(*buf, "^N");
  else if(c == 0xf)  /* shift in */
    strcpy(*buf, "^O");
  else if(c == 0x10) /* data link escape */
    strcpy(*buf, "^P");
  else if(c == 0x11) /* device control 1 (oft. XON) */
    strcpy(*buf, "^Q");
  else if(c == 0x12) /* device control 2 */
    strcpy(*buf, "^R");
  else if(c == 0x13) /* device control 3 (oft. XOFF) */
    strcpy(*buf, "^S");
  else if(c == 0x14) /* device control 4 */
    strcpy(*buf, "^T");
  else if(c == 0x15) /* negative acknowledgement  */
    strcpy(*buf, "^U");
  else if(c == 0x16) /* synchronous idle */
    strcpy(*buf, "^V");
  else if(c == 0x17) /* end of transmission block */
    strcpy(*buf, "^W");
  else if(c == 0x18) /* cancel */
    strcpy(*buf, "^X");
  else if(c == 0x19) /* end of medium */
    strcpy(*buf, "^Y");
  else if(c == 0x1a) /* substitute */
    strcpy(*buf, "^Z");
  else if(c == 0x1b) /* escape */
    strcpy(*buf, cflag ? "^]" : "\\e");
  else if(c == 0x1c) /* file separator */
    strcpy(*buf, "^\\");
  else if(c == 0x1d) /* group separator */
    strcpy(*buf, "^]");
  else if(c == 0x1e) /* record separator */
    strcpy(*buf, "^^");
  else if(c == 0x1f) /* unit separator */
    strcpy(*buf, "^_");
  else if(c == 0x7f) /* delete */
    strcpy(*buf, "^?");
  else
    sprintf(*buf, "%c", c);
}


void compute_output(char **output, char *input, unsigned long int delim,
                 int delimflag, int cflag){
  int i, j, n, buflen = 3;
  char *buf = NULL;
  
  n = strlen(input);
  *output = (char*) calloc((2*n + 1), sizeof(char));
  buf = (char*) calloc(buflen, sizeof(char));
  
  for(i = 0; i < n; ++i){
    one_sub(&buf, input[i], cflag);
    strcat(*output, buf);

    for(j = 0; j < buflen; ++j)
      buf[j] = '\0';
      
    if(delimflag)
      if(delim == input[i])
        strcat(*output, "\n");
  }  
  free(buf);
}

int main(int argc, char **argv){
  char c, opt, filename[256], *buf = NULL, *input = NULL, *output = NULL;
  int i, count, cflag = 0, delimflag = 0, pflag = 0, fflag = 0, tflag = 0;
  unsigned long int delim = 0;
  
  /* parse command line options */
  while((opt = getopt(argc, argv, "acd:f:hpt:x:")) != -1){
    if(opt == 'a')
      about();
    else if(opt == 'c')
      cflag = 1;
    else if(opt == 'd')
    {
      delimflag = 1;
      delim = strtoul(optarg, NULL, 10);
    } 
    else if(opt == 'f')
    {
      fflag = 1;
      strcpy(filename, optarg);
      if(pflag || tflag)
        handle_error("Flags -f, -p, and -t are mutually exclusive.");
    } 
    else if(opt == 'h'){
      usage();
    } 
    else if(opt == 'p')
    {
      pflag = 1;
      if(fflag || tflag)
        handle_error("Flags -f, -p, and -t are mutually exclusive.");
    } 
    else if(opt == 't')
    {
      tflag = 1;
      if(fflag || pflag)
        handle_error("Flags -f, -p, and -t are mutually exclusive.");
      input = (char*) calloc(strlen(optarg), sizeof(char));
      strcpy(input, optarg);
    } 
    else if(opt == 'x')
    {
      delimflag = 1;
      delim = strtoul(optarg, NULL, 16);
    } 
  }
  
  /* read input */
  if(fflag)
    read_file(&input, filename);
  else if(pflag)
  {
    i = 0;
    count = 0;
    buf = (char*) calloc(BUFSIZE + 1, sizeof(char));
    input = calloc(1, sizeof(char));
    
    while((c = fgetc(stdin)) != EOF)
    {
      buf[i] = c;
      i = (i + 1) % BUFSIZE;
      
      if(!i)
      {
        input = (char*) realloc(input, ((++count) * BUFSIZE + 1) * sizeof(char));
        strcat(input, buf);
      }
    }
    
    buf[i] = '\0';
    input = (char*) realloc(input, (count * BUFSIZE + strlen(buf) + 1) * sizeof(char));
    strcat(input, buf);
    input[strlen(input) - 1] = '\0'; /* remove artificial \n from stdin */    
  } 
  else if(!tflag)
    usage();

  /* compute and print output */
  compute_output(&output, input, delim, delimflag, cflag);
  printf("%s\n", output);
  
  /* clean up */
  if(buf != NULL)
    free(buf);
    
  if(input != NULL)  
    free(input);

  if(output != NULL)
    free(output);

  exit(EXIT_SUCCESS);
}
