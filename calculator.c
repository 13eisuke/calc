#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define argbufsize 12
#define maxlinelen 4

double ans = 0, mem = 0;

double getnum();
int get_line (char* line);
void parse_line (char* line);
void exe_cmd (char cmd);
void exe_calc (char op);
void cmd_ansclear ();
void cmd_print ();
void cmd_memadd ();
void cmd_memminus ();
void cmd_memclear ();
void calc_add (double arg);
void calc_minus (double arg);
void calc_mult (double arg);
void calc_div (double arg);

double getnum () {
    char argbuf[argbufsize], tmp[argbufsize];
    int isNUM = 0, count_dot = 0;
    int i, j, k, n = 0;
    double num, base;

    while (!isNUM) {
        num = 0;
        base = 1;
        count_dot = 0;
        
        write (1, "number > ", sizeof("number > ")-1);
        for (i = 0; i < argbufsize; i++) {
            argbuf[i] = '\0';
        }
        if ((i = read(0, argbuf, sizeof(argbuf)-1)) == -1) {
            continue;
        }

        if (argbuf[i-1] != '\n') {
            n = 0;
            while (!n) {
                for (i = 0; i < argbufsize; i++) {
                    tmp[i] = '\0';
                }
                if ((i = read(0, tmp, argbufsize-1)) == -1) {
                    exit(0);
                }
                for (j = 0; j < argbufsize-1; j++) {
                    if (tmp[j] == '\n') n++;
                }
            }
            write (1, "Error : too long number\n", sizeof("Error : too long number\n")-1);
            continue;
        }

        for (j = i-2; j >= 0; j--) {
            if ('0' <= argbuf[j] && argbuf[j] <= '9') {
                num += (argbuf[j] - '0') * base;
                base *= 10;
            }
            else if (argbuf[j] == '.' && count_dot < 1) {
                base = 1;
                count_dot++;
                for (k = 0; k < i-2-j; k++) {
                    num /= 10;
                }
            }
            else if (j == 0 && argbuf[j] == '-') num *= -1; 
            else {
                isNUM = 0;
                write (1, "Error : Invalid input\n", sizeof("Error : Invalid input\n")-1);
                break;
            }
        }
        if (j == -1) {
            isNUM = 1;
        }
    }
    return num;
}


int get_line (char* line) {
    int i, j, n = 0, line_len;
    int isCorrect = 0;

    char tmp[maxlinelen];

    while (!isCorrect) {
        n = 0;
        write (1, "op or cd > ", sizeof("op or cd > ")-1);
        if ((line_len = read(0, line, maxlinelen-1)) == -1) {
            break;
        } else {
            for (i = 0; i < line_len; i++) {
                if (line[i] == '\n') n++;
            }
            if (n == 0) {
                while (!n) {
                    for (i = 0; i < maxlinelen; i++) {
                        tmp[i] = '\0';
                    }
                    if ((i = read(0, tmp, maxlinelen-1)) == -1) {
                        continue;
                    }
                    for (j = 0; j < maxlinelen-1; j++) {
                        if (tmp[j] == '\n') n++;
                    }
                }
                write (1, "Error : Too long command or operator\n", sizeof("Error : Too long command or operator\n")-1);
            } else {
                isCorrect = 1;
                if (*line == '%' && *(line+1) == 'Q') return 0;
            }
        }
    }
    return 1;
}

void parse_line (char* line) {
    if (*line == '%') exe_cmd (*(line+1));
    else if (*line == '$') exe_calc (*(line+1));
    else write (1, "Error : Invalid input\n", sizeof("Error : Invalid input\n")-1);
}

void exe_cmd (char cmd) {
    switch (cmd)
    {
        case 'C':
            cmd_ansclear();
            break;
        case 'P':
            cmd_print();
            break;
        case 'A':
            cmd_memadd();
            break;
        case 'M':
            cmd_memminus();
            break;
        case 'D':
            cmd_memclear();
            break;
        default:
            write (1, "Error : Invalid command\n", sizeof("Error : Invalid command\n")-1);
            break;
    }
}

void exe_calc (char op) {
    double arg;

    switch (op)
    {
        case '+':
            arg = getnum ();
            calc_add (arg);
            break;
        case '-':
            arg = getnum ();
            calc_minus (arg);
            break;
        case '*':
            arg = getnum ();
            calc_mult (arg);
            break;
        case '/':
            arg = getnum ();
            calc_div (arg);
            break;            
        default:
            write (1, "Error : Invalid operator\n", sizeof("Error : Invalid operator\n")-1);
            break;
    }
}

void cmd_ansclear () {
    ans = 0;
}

void cmd_print () {
    printf ("Ans : %lf\nMem : %lf\n", ans, mem);
}

void cmd_memadd () {
    mem += ans;
}
void cmd_memminus () {
    mem -= ans;
}
void cmd_memclear () {
    mem = 0;
}

void calc_add (double arg) {
    ans += arg;
}
void calc_minus (double arg) {
    ans -= arg;
}
void calc_mult (double arg) {
    ans *= arg;
}
void calc_div (double arg) {
    if (arg == 0) {
        write (1, "Error : Zero division\n", sizeof("Error : Zero division\n")-1);
    } else {
        ans /= arg;
    }
}


int main()
{
    char line[maxlinelen] = {'\0'};

    ans = getnum ();

    while (get_line (line)) {
        parse_line (line);
    }
    
    printf ("QUIT\n");

    return 0;
}