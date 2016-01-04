//IMPORTANT!!!
//This program is long, complicated and uses
//a lot of structures. As such, it highly inefficient.

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define mxl 100

struct lin
{
    char op[100];
    char reg[100];
    char val[100];
    char extra[100]; //extra variable for JMP statements
};

struct tableEntry
{
    char name[30];
    int address;
    char value[30];
};

void Register(FILE * f, char * s)
{
    if (strcmp(s, "R1,") == 0)
    {fprintf(f, "00100");}
    else if (strcmp(s, "R2,") == 0)
    {fprintf(f, "01000");}
    else if (strcmp(s, "R3,") == 0)
    {fprintf(f, "01100");}
    else if (strcmp(s, "R4,") == 0)
    {fprintf(f, "10000");}
    else if (strcmp(s, "R5,") == 0)
    {fprintf(f, "10100");}
    else if (strcmp(s, "R6,") == 0)
    {fprintf(f, "11000");}
    else if (strcmp(s, "R7,") == 0)
    {fprintf(f, "11100");}
    else if (strcmp(s, "*R1,") == 0)
    {fprintf(f, "00101");}
    else if (strcmp(s, "*R2,") == 0)
    {fprintf(f, "01001");}
    else if (strcmp(s, "*R3,") == 0)
    {fprintf(f, "01101");}
    else if (strcmp(s, "*R4,") == 0)
    {fprintf(f, "10001");}
    else if (strcmp(s, "*R5,") == 0)
    {fprintf(f, "10101");}
    else if (strcmp(s, "*R6,") == 0)
    {fprintf(f, "11001");}
    else if (strcmp(s, "*R7,") == 0)
    {fprintf(f, "11101");}
}

void hexToBin(FILE * f, char * s)
{
    char binaryNumber[100],hexaDecimal[100];
    int i=0;
    strcpy(hexaDecimal, s);
    while(hexaDecimal[i])
    {
         switch(hexaDecimal[i])
         {
             case '0': fprintf(f, "0000"); break;
             case '1': fprintf(f, "0001"); break;
             case '2': fprintf(f, "0010"); break;
             case '3': fprintf(f, "0011"); break;
             case '4': fprintf(f, "0100"); break;
             case '5': fprintf(f, "0101"); break;
             case '6': fprintf(f, "0110"); break;
             case '7': fprintf(f, "0111"); break;
             case '8': fprintf(f, "1000"); break;
             case '9': fprintf(f, "1001"); break;
             case 'A': fprintf(f, "1010"); break;
             case 'B': fprintf(f, "1011"); break;
             case 'C': fprintf(f, "1100"); break;
             case 'D': fprintf(f, "1101"); break;
             case 'E': fprintf(f, "1110"); break;
             case 'F': fprintf(f, "1111"); break;
         }
         i++;
    }
}

void referTable(FILE * f, char * s, struct tableEntry y[mxl])
{
    int i;
    for(i = 0; i < mxl; i++)
    {
        if (strcmp(s, y[i].name) == 0)
        {
            hexToBin(f, y[i].value);
            fprintf(f, "\n");
        }
    }
}

int findValue(char * s, FILE * f, struct tableEntry t[mxl])
{
    int i;
    for(i = 0; i < mxl; i++)
    {
        if (strcmp(s, t[i].name) == 0)
        {
            hexToBin(f, t[i].value);
            return 1;
        }
    }
    return 0;
}

void constVal(FILE * f, char * s, struct lin y[mxl])
{
    int i;
    for(i = 0; i < mxl; i++)
    {
        hexToBin(f, y[i].val);
        fprintf(f, "\n");
    }
}

int main()
{
    char * input;
    printf("%s", "Enter the name of the file you wish to open.);
    scanf("%s", input);
    FILE * ip = fopen(input, "r");
    fseek(ip, 0, SEEK_SET);
    char line[mxl]; 
    struct lin a[mxl];
    int i;
    for(i = 0; fgets(line, mxl, ip); i++)
    {
        char * word;
        word = strtok(line, " \n\0");
        if(word != NULL) 
        {strcpy(a[i].op, word);}
        word = strtok(NULL, " \n\0");
        if(word != NULL) 
        {strcpy(a[i].reg, word);}
        word = strtok(NULL, " \n\0");
        if(word != NULL)
        {strcpy(a[i].val, word);}
        word = strtok(NULL, " \n\0");
        if(word != NULL)
        {strcpy(a[i].extra, word);}
    }
    struct tableEntry t[mxl];
    int p;
    int q = 0;
    for(p = 0; p < mxl; p++)
    {
        
        
        //NOTE: I intended for addresses to be saved
        //as the position in the array where the variable
        //name was found. For some reason, this does not
        //work and thus renders my JMP implementation
        //useless.
        if (strcmp(a[p].reg, "DC") == 0)
        {
            strcpy(t[q].name, a[p].op);
            t[q].address = p;
            strcpy(t[q].value, a[p].val);
            //printf("%s\n", t[q].name);
            q++;
        }
    }
    FILE * op = fopen("Binarycode.txt", "w");
    int li;
    for(li = 0; li <= mxl; li++) 
    {
        //I intended to have more lines in the JMP
        //implementation, but they did not work.
        
        if (strcmp(a[li].op, "HLT") == 0)
        {
            fprintf(op, "001010");
            fprintf(op, "0000000000");
            return;
        }
        
        if (strcmp(a[li].reg, "DS") == 0)
        {
            int no = atoi(a[li].val);
            int r = findValue(a[li].op, op, t);
            if(!r)
            {
                int s;
                while(s <= no)
                {
                    fprintf(op, "0000000000000000");
                    fprintf(op, "\n");
                }
            }
        }
        
        if (strcmp(a[li].reg, "DC") == 0)
        {
            fprintf(op, "00000000");
            hexToBin(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "MOV") == 0)
        {
            //moveIt(a, op);
            fprintf(op, "001101");
            Register(op, a[li].reg);
            if (a[li].val[0] == '&')
            {
                fprintf(op, "00010");
                fprintf(op, "\n");
                char * temp = a[li].val+1;
                //printf("%s\n", temp);
                referTable(op, temp, t);
            }
            else if (a[li].val[0] == '#')
            {
                fprintf(op, "00010");
                fprintf(op, "\n");
                char * temp = a[li].val+1;
                constVal(op, temp, a);
            }
            else
            {
                hexToBin(op, a[li].val);
                fprintf(op, "\n");
            }
        }
        
        if (strcmp(a[li].op, "ADD") == 0)
        {
            fprintf(op, "010000");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "SUB") == 0)
        {
            fprintf(op, "010001");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "NEG") == 0)
        {
            fprintf(op, "000000");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "AND") == 0)
        {
            fprintf(op, "000001");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "OR") == 0)
        {
            fprintf(op, "000011");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "LSR") == 0)
        {
            fprintf(op, "000100");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "LSL") == 0)
        {
            fprintf(op, "000101");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
        
        if (strcmp(a[li].op, "XOR") == 0)
        {
            fprintf(op, "000010");
            Register(op, a[li].reg);
            Register(op, a[li].val);
            fprintf(op, "\n");
        }
    }
    return 0;
}



