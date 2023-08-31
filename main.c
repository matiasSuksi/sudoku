#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 9

char solvable_sudoku[1024];
char solution_sudoku[1024];
int solvable_grid[N][N];
int solution_grid[N][N];
int game_over;
int win;
int guesses = 10;

void choose_sudoku();
void print_grid();
void check_answer();
void insert_grid();
void convert_grids();
int game_status();
void game();

int main() 
{
    choose_sudoku();
    convert_grids();
    game();
    return 0;
}
//Valitsee sudokun ja sen ratkaisun databasesta (sudokus.csv)
void choose_sudoku()
{
    FILE* fp = fopen("sudokus.csv", "r");
    if (fp == NULL)
    {
        printf("Sudokun nouto epäonnistui");
        return;
    }
    
    int num_rows = 0;
    char line[1024];
    while (fgets(line, 1024, fp) != NULL)
    {
        num_rows++;
    }
   
    // Satunnainen rivin indeksi
    srand(time(NULL));
    int random_index = rand() % num_rows;

    // FILE-pointer tiedoston alkuun 
    fseek(fp, 0, SEEK_SET);

    // Iteroidaan kunnes saavutetaan valittu satunnainen rivi 
    for (int i = 0; i < random_index; i++)
    {
        fgets(line, 1024, fp);
    }
    if (fgets(line, 1024, fp) != NULL)
    {
        char* token = strtok(line, ",");
        strncpy(solvable_sudoku, token, 1024);
        token = strtok(NULL, ",");
        strncpy(solution_sudoku, token, 1024);
        printf("random rivi:\npöytä=%s\nratkaisu=%s", solvable_sudoku, solution_sudoku);
    }
}

// Tulostetaan sudoku
void print_grid()
{
    printf("\nJäljellä olevat arvaukset: \e[1m%d\e[m\n\n", guesses);
    int colum = 1;
    int grid_calc=0; // tarvitaan 3x3 grid sivujen tummentamiseen
    int horiz_calc=0; // tarvitaan 3x3 grid pohjien tekemiseen
    printf("   ");
    for(int i=1; i<=9; i++)
    {
        if(i==1)
        {
            printf(" ");
        }
        if(i < 4)
        {
            printf("\e[1m%d\e[m ", i);
        }
        if(i==3)
        {
            printf(" ");
        }
        if(i >= 4 && i < 7)
        {
            printf("\e[1m%d\e[m ", i);
        }
        if(i==6)
        {
            printf(" ");
        }
        if(i >= 7 && i < 10)
        {
            printf("\e[1m%d\e[m ", i);
        }

        
    }
    printf("\n");
    //printf("    ____________________\n");
    for (int i = 0; i < N; i++) 
    {
        
        if(horiz_calc % 3 == 0){
            printf("    ___________________\n");
        }
        horiz_calc++;
        printf("\e[1m%d ||\e[m", colum);
        colum++;
        for (int j = 0; j < N; j++)
        {
            grid_calc++;
            printf("\e[4m%d\e[0m", solvable_grid[i][j]);
            if(grid_calc % 3 == 0)
            {
                printf("\e[1m||\e[m");
            }
            else
            {
                printf("|");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Muutetaan yksiulotteiset arrayt ongelma ja ratkaisu kaksiuloitteiseksi 
void convert_grids() 
{
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            solvable_grid[i][j] = solvable_sudoku[i*N+j] - '0';
            solution_grid[i][j] = solution_sudoku[i*N+j] - '0';
        }
    }
}

// Tarkistaa onko käyttäjän syöte oikea
void check_answer(int row, int col, int value)
{
    if(solvable_grid[row][col] != 0)
    {
       printf("Ruudussa on jo numero!\n");
       insert_grid();
    }

    else if(solution_grid[row][col] != value)
    {
       guesses--;
       printf("Väärä numero!\n");
    }

    else
    {
        solvable_grid[row][col] = value;
    }
}

// Käyttäjän syöte
void insert_grid()
{
    int row;
    int col;
    int value;
    
    printf("Syötä rivi ja sarake:\n");
    fflush( stdout );
    if(!scanf("%d%d", &row, &col)) 
    {
        scanf("%*[^\n]");
        printf("Syötä numero!\n");
        insert_grid();
    }
    printf("Syötä luku: ");
    fflush( stdout );
    if(!scanf("%d", &value)) 
    {
        scanf("%*[^\n]");
        printf("Syötä numero!\n");
        insert_grid();
    }
    fflush( stdout );
    check_answer(--row, --col, value);
}

// Pelin tilan tarkistus
int game_status()
{
    //Tarkista onko liikaa vääriä arvauksia --> häviö
    if(guesses > 0)
    {   
    //Jos ratkaisu valmis eli grid täynnä --> voitto
        for(int i = 0; i<N; i++)
        {
            for (int j=0; j<N; j++)
            {
                if(solvable_grid[i][j]!=0) 
                {
                    continue;
                }
                else 
                {
                    return win;
                }
            }
        }
    }
    else
    {
        game_over = 1;
        return game_over;
    }
    win = 1;
    return win;
}

// Peli-loop
void game()
{
    game_over = 0;
    win = 0;   
    while(!game_over && !win)
    {
        game_status();
        print_grid();
        insert_grid();
    }
}



