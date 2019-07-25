#include<curses.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>

void drawMenu(WINDOW **w, char (*s)[12])  //deseneaza meniul cu cele cinci optiuni
{
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	init_pair(1, 255, 237);
	bkgd(COLOR_PAIR(1));
	*w = newwin(10, 15, 2, 2);
	box(*w, 0, 0);
	keypad(*w, TRUE);
	wattron(*w, A_STANDOUT);        //prima optiune va avea highlight pe text
	mvwprintw(*w, 1, 2, "%s", s[0]);  //se afiseaza "New Game"
	wattroff(*w, A_STANDOUT);         //urmatoarele nu vor fi evidentiate
	mvwprintw(*w, 2, 2, "%s", s[1]);  //se afiseaza "Resume"
	mvwprintw(*w, 3, 2, "%s", s[2]);  //se afiseaza "Highscores"
	mvwprintw(*w, 4, 2, "%s", s[3]);  //se afiseaza "Switch user"
	mvwprintw(*w, 5, 2, "%s", s[4]);  //se afiseaza "Quit"
	wrefresh(*w);
}

typedef struct {
	char name[40], time [26];
	int score;
} data;

int compare(const void *a, const void *b)
{
	data *ia = (data *)a;
	data *ib = (data *)b;
	int rez = ib->score - ia->score; //se compara scorurile
	if (rez != 0) 
		return rez;
	else //daca sunt egale,
		return strcmp(ib->name, ia->name); //se compara lexicografic
}

void topScores(WINDOW **w, data *v, int n, int x) //deseneaza fereastra cu scorurile
{
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	init_pair(1, 255, 237);
	bkgd(COLOR_PAIR(1));
	int i;
	*w = newwin(25, 77, 2, 2);
	box(*w, 0, 0);
	keypad(*w, TRUE);
	mvwprintw(*w, 1, 37, "TOP 10");
	while (i <= 9 && i < n) {
		mvwprintw(*w, 3 + i, 4, "%d. %s %d", i + 1, v[i].name, v[i].score);
		i++;
	}
	mvwprintw(*w, 14, 32, "AVERAGE SCORE");
	mvwprintw(*w, 16, 38, "%d", x);
	mvwprintw(*w, 20, 4, "PRESS 'S' TO SEARCH FOR A USERNAME");
	mvwprintw(*w, 23, 4, "PRESS 'Q' TO EXIT");
	wrefresh(*w);
}

void personalHistory(WINDOW **w, data *v, int n) //afiseaza scorurile
{

	char s[80]; 
	int i, lineNumber = 3;
		
	wmove(*w, 21, 4); //apare cursorul pe ecran
	curs_set(1);
	echo();
	wgetstr(*w, s);  //se retine un username in variabila s
	delwin(*w);  //inchide fereastra 
	endwin();

	initscr();
	noecho();
	cbreak();	
	curs_set(0);
	start_color();
	init_pair(1, 255, 237);
	bkgd(COLOR_PAIR(1));
	*w = newwin(25, 77, 2, 2);
	box(*w, 0, 0);
	keypad(*w, TRUE);
	mvwprintw(*w, 1, 19, "TOP SCORES FOR %s", s);
	for (i = 0; i < n; i++)
		if (strcmp(v[i].name, s) == 0) { //cauta informatiile pentru username-ul ales
			mvwprintw(*w, lineNumber, 4, "%d points on %s", v[i].score, v[i].time);
			lineNumber++; //trece pe linia urmatoare
			if (lineNumber == 19)
				break;
		}
	if (lineNumber == 3) //lineNumber creste doar atunci cand se gaseste un match cu username-ul
		mvwprintw(*w, lineNumber, 4, "THIS USER DOES NOT EXIST");
	mvwprintw(*w, 19, 4, "PRESS ANY KEY TO EXIT");
	wgetch(*w); 
}

void askUsername(WINDOW **w, char *s)  //cere un username si il retine in s
{
	initscr();
	echo();
	nocbreak();
	curs_set(1);
	start_color();
	init_pair(1, 255, 237);
	bkgd(COLOR_PAIR(1));
	*w = newwin(25, 77, 2, 2);
	box(*w, 0, 0);
	keypad(*w, TRUE);
	mvwprintw(*w, 10, 4, "%s", "PICK AN USERNAME: ");
	wgetstr(*w, s);
	delwin(*w);
	endwin();
	refresh();
}

char* getTime()   //returneaza data si ora curenta
{
	time_t currentTime;
    char *Time;

   	currentTime = time(NULL);
    Time = ctime(&currentTime);

    return Time;
}

void newBoard(WINDOW **w, WINDOW **u)  //deseneaza tabla de joc cu cele 16 casute si panoul cu informatii
{
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	init_pair(1, 255, 237);
	bkgd(COLOR_PAIR(1));
	*w = newwin(25, 77, 2, 2);
	box(*w, 0, 0);
	keypad(*w, TRUE);
	//traseaza cele 16 celule folosind caracterele '-' si '|'
	int i, j;
	for (i = 2; i <= 18; i++) {
        mvwaddch(*w, i, 2, '|');
        mvwaddch(*w, i, 10, '|');
        mvwaddch(*w, i, 18, '|');
        mvwaddch(*w, i, 26,'|');
       	mvwaddch(*w, i, 34, '|');
        if (i % 4 == 2)               //traseaza linia orizontala care desparte casutele
            for (j = 2; j <= 34; j++) 
                mvwaddch(*w, i, j, '-');
   	}
   	wrefresh(*w);
   	*u = derwin(*w, 20, 28, 2, 38);  //panoul se afla in aceeasi fereastra cu celulele
   	mvwprintw(*u, 2, 2, "%s", getTime()); //afiseaza data
   	mvwprintw(*u, 6, 2, "SCORE: %d", 0);  //afiseaza scorul (initial 0)
   	mvwprintw(*u, 10, 2, "%s", "USE ARROW KEYS TO PLAY");  
   	mvwprintw(*u, 12, 2, "%s", "PRESS 'N' FOR NEW GAME");
   	mvwprintw(*u, 13, 2, "%s", "PRESS 'U' TO UNDO");
   	mvwprintw(*u, 14, 2, "%s", "PRESS 'Q' TO EXIT");
   	box(*u, 0, 0);
   	wrefresh(*u);
}

int val() //genereaza o valoare aleatoare dintre 2 si 4
{
	if (rand() % 2 == 0)
		return 2;
	else
		return 4;
}
 //Urmatoarele doua functii genereaza o pozitie aleatoare pe tabla de joc
int line()  //genereaza o linie aleatoare. Pozitiile posibile sunt: 4, 8, 12 sau 16.
{
	return (4 * (rand() % 4 + 1));  //(rand() % 4 + 1) genereaza un numar intre 1 si 4
}

int col()  //genereaza o coloana aleatore. Pozitiile posibile sunt: 6, 14, 22 sau 30.
{
	return (8 * (rand() % 4 + 1) - 2);  
}

void genRandom(WINDOW **w, int m[17][31]) //foloseste cele trei functii precedente
{
	int a, b, c;
	do {
   		a = line(); 
   		b = col();
   	}
   	while (m[a][b] != 0); //pozitia generata nu trebuie sa fie ocupata pe tabla de joc
   	c = val();
   	m[a][b] = c; 

}

void copyMatrix(int n[17][31], int m[17][31]) //copiaza elementele matricei m in n
{
	int i, j;
	for (i = 4; i <= 16; i += 4)
		for(j = 6; j <= 30; j += 8)
			n[i][j] = m[i][j];
}

void printMatrix(int m[17][31], WINDOW **w)
{
	start_color();
	init_pair(1, 25, 237);
	init_pair(2, 3, 237);
	init_pair(3, 124, 237);
	init_pair(4, 6, 237);
	init_pair(5, 171, 237);
	init_pair(6, 2, 237);
	init_pair(7, 219, 237);
	init_pair(8, 75, 237);
	init_pair(9, 209, 237);
	init_pair(10, 198, 237);
	init_pair(11, 255, 237);
	//Am numerotat perechile de culori in functiile de puterile lui 2 care exista pe tabla (1, 2,... 11).
	//De aici, nevoia de logaritm in baza 2.
	int i, j;
	for (i = 4; i <= 16; i += 4)
		for (j = 6; j <= 30; j += 8) {
			mvwprintw(*w, i, j - 2, "%s", "    ");
			if (m[i][j] != 0) {
				wattron(*w, COLOR_PAIR(log2(m[i][j])));  //coloreaza numarul
				mvwprintw(*w, i, j - 2, "%d", m[i][j]);	
				wattroff(*w, COLOR_PAIR(log2(m[i][j])));
			}
		}
	wrefresh(*w);
}

void north(int m[17][31], int *x)
{
	int i, j, tmp;
	for (j = 6; j <= 30; j += 8) { //ia fiecare coloana pe rand
		for (i = 8; i <= 16; i += 4)    //incepe de la marginea din nord
			if (m[i][j] == 0)           //ignora celulele deja goale
				continue;               
			else {
				tmp = i;    //ia o celula nenula si
				while (m[tmp - 4][j] == 0 && tmp >= 8) {  //pana cand atinge marginea sau o alta celula nenula,
					m[tmp - 4][j] = m[tmp][j];  //o deplaseaza inspre nord,
					m[tmp][j] = 0;   //lasand in urma sa celulele goale
					tmp -= 4;
				}
			}
		for (i = 8; i <= 16; i += 4)     //apoi, se porneste de la marginea din nord a tablei
			if (m[i - 4][j] == m[i][j] && m[i - 4][j] != 0) {  //daca se intalnesc doua celule egale
				m[i - 4][j] = 2 * m[i][j];  //cea mai din nord isi dubleaza valoarea
				m[i][j] = 0; 			//cea de sub ea devine 0
				*x += m[i - 4][j];     //se actualizeaza scorul
			}
		for (i = 8; i <= 16; i += 4)    //se reia primul pas, pentru ca
			if (m[i][j] == 0)          //intre timp pot sa mai fi aparut celule goale 
				continue;
			else {
				tmp = i;
				while (m[tmp - 4][j] == 0 && tmp >= 8) {
					m[tmp - 4][j] = m[tmp][j];
					m[tmp][j] = 0;
					tmp -= 4;
				}
			}
	}
}
//Celelalte trei directii posibile de a muta au un algoritm analog.
void west(int m[17][31], int *x)
{
	int i, j, tmp;
	for (i = 4; i <= 16; i += 4) {
		for (j = 14; j <= 30; j += 8)    
			if (m[i][j] == 0)
				continue;
			else {
				tmp = j;
				while (m[i][tmp - 8] == 0 && tmp >= 14) {
					m[i][tmp - 8] = m[i][tmp];
					m[i][tmp] = 0;
					tmp -= 8;
				}
			}
		for (j = 14; j <= 30; j += 8)
			if (m[i][j - 8] == m[i][j] && m[i][j - 8] != 0) {
				m[i][j - 8] = 2 * m[i][j];
				m[i][j] = 0;
				*x += m[i][j - 8];
			}
		for (j = 14; j <= 30; j += 8)  
			if (m[i][j] == 0)
				continue;
			else {
				tmp = j;
				while (m[i][tmp - 8] == 0 && tmp >= 14) {
					m[i][tmp - 8] = m[i][tmp];
					m[i][tmp] = 0;
					tmp -= 8;
				}
			}
	}
}

void south(int m[17][31], int *x)
{
	int i, j, tmp;
	for (j = 6; j <= 30; j += 8) {
		for (i = 12; i >= 4; i -= 4)
			if (m[i][j] == 0)
				continue;
			else {
				tmp = i;
				while (m[tmp + 4][j] == 0 && tmp <= 12) {
					m[tmp + 4][j] = m[tmp][j];
					m[tmp][j] = 0;
					tmp += 4;
				}
			}
		for (i = 12; i >= 4; i -= 4)
			if (m[i + 4][j] == m[i][j] && m[i + 4][j] != 0) {
				m[i + 4][j] = 2 * m[i][j];
				m[i][j] = 0;
				*x += m[i + 4][j];
			}
		for (i = 12; i >= 4; i -= 4)    
			if (m[i][j] == 0)
				continue;
			else {
				tmp = i;
				while (m[tmp + 4][j] == 0 && tmp <= 12) {
					m[tmp + 4][j] = m[tmp][j];
					m[tmp][j] = 0;
					tmp += 4;
				}
			}
	}
}

void east(int m[17][31], int *x)
{
	int i, j, tmp;
	for (i = 4; i <= 16; i += 4) {
		for (j = 22; j >= 6; j -= 8)    
			if (m[i][j] == 0)
				continue;
			else {
				tmp = j;
				while (m[i][tmp + 8] == 0 && tmp <= 22) {
					m[i][tmp + 8] = m[i][tmp];
					m[i][tmp] = 0;
					tmp += 8;
				}
			}
		for (j = 22; j >= 6; j -= 8)
			if (m[i][j + 8] == m[i][j] && m[i][j + 8] != 0) {
				m[i][j + 8] = 2 * m[i][j];
				m[i][j] = 0;
				*x += m[i][j + 8];
			}
		for (j = 22; j >= 6; j -= 8)    
			if (m[i][j] == 0)
				continue;
			else {
				tmp = j;
				while (m[i][tmp + 8] == 0 && tmp <= 22) {
					m[i][tmp + 8] = m[i][tmp];
					m[i][tmp] = 0;
					tmp += 8;
				}
			}
	}
}

int validMove(int m[17][31], int n[17][31], void (*func)(int m[17][31], int *x))
{
	int i, j, x = 0;
	func(n, &x);  //x reprezinta scorul curent, dar nu este important aici si i-am dat valoarea 0
	for (i = 4; i <= 16; i += 4)
		for (j = 6; j <= 30; j += 8)
			if (m[i][j] != n[i][j]) //daca tabla de joc nu ramane aceeasi dupa efectuarea mutarii,
				return 1;   //aceasta este valida
	return 0;
} 

int checkIfLost(int m[17][31])  //verifica daca jocul e pierdut
{
	int i, j;
	for (i = 4; i <= 16; i += 4)       
		for(j = 6; j <= 30; j += 8)
			if (m[i][j] == 0)       //exista un loc gol pe tabla sau
				return 0;
	for (i = 4; i <= 16; i += 4)  
		for (j = 6; j <= 30; j += 8)
			if (m[i][j] == m[i - 4][j])  //doua celule adiacente pe aceeasi coloana sunt egale (se pot alipi) sau
				return 0;
	for (i = 4; i <= 16; i += 4)
		for (j = 6; j <= 22; j += 8)
			if (m[i][j] == m[i][j + 8])  //doua celule adiacente pe aceeasi linie sunt egale (se pot alipi)
				return 0;
	return 1;
}

int checkIfWon(int m[17][31])  //verifica daca jocul e castigat
{
	int i, j;
	for (i = 4; i <= 16; i += 4)
		for(j = 6; j <= 30; j += 8)
			if (m[i][j] == 2048)   //o celula de pe tabla are valoarea 2048
				return 1;
	return 0;
}

int cntZeros (int m[17][31])  //numara celulele goale de pe tabla
{
	int i, j, cnt = 0;
	for (i = 4; i <= 16; i += 4)
		for(j = 6; j <= 30; j += 8)
			if (m[i][j] == 0) 
				cnt++;
	return cnt;

}

int bestMove(int m[17][31])
{
	int i = 0, aux[17][31], zeros[4] = { 0 }, max, rez = 0;
	copyMatrix(aux, m);
	if (validMove(m, aux, north)) //incearca fiecare mutare, doar daca e valida
		zeros[0] = cntZeros(aux);  //numara celulele eliberate
	copyMatrix(aux, m);
	if (validMove(m, aux, west))
		zeros[1] = cntZeros(aux);
	copyMatrix(aux, m);
	if (validMove(m, aux, south))
		zeros[2] = cntZeros(aux);
	copyMatrix(aux, m);
	if (validMove(m, aux, east))
		zeros[3] = cntZeros(aux);
	max = zeros[0];
	for (i = 1; i < 4; i++)  //determina maximul de celule eliberate
		if (zeros[i] > max) { 
			max = zeros[i];
			rez = i;
		}
	return rez;
}

int main() 
{
	srand(time(NULL));
	FILE *f1, *f2, *f3, *f4;
	WINDOW *menu, *request, *board, *pannel, *info;
	data *v;
	char options[5][12] = { "New Game", "Resume", "Highscores", "Switch user", "Quit" }, user[40], s[40];
	int i, j, k, n = 0, choice, score = 0, avgScore, lastScore, sum = 0, gameOver = 2;
	int cell[17][31] = { 0 }, aux[17][31] = { 0 }, lastMove[17][31] = { 0 };
	bool backToMenu, exitGame = FALSE, resume = TRUE;


	f1 = fopen("users.txt", "a+");
	f2 = fopen("scores.txt", "a+");
	f3 = fopen("dates.txt", "a+");	
	f4 = fopen("last_game.txt", "r+");

    v = (data *)malloc(sizeof(data)); //se iau informatiile despre scoruri
	while (fgets(s, 40, f1) != NULL) {  //se citesc datele din fisier
		strcpy(v[n].name, s);             
		v[n].name[strlen(s) - 1] = '\0';
		fscanf(f2, "%d", &v[n].score);  
		sum += v[n].score;   //se face suma scorurilor, pentru a calcula media lor
		fgets(v[n].time, 26, f3);
		v[n].time[24] = '\0';
		n++;   //se pregateste o noua intrare
		v = (data *)realloc(v, (n + 1) * sizeof(data));
	}
	while (!exitGame) {
		drawMenu(&menu, options);  //se afiseaza meniul
		choice = 0;  //optiunea curenta este prima ("New Game")
		while ((k = wgetch(menu))) {
			mvwprintw(menu, choice + 1, 2, "%s", options[choice]); //se afiseaza optiunea curenta
			switch(k) {  //cazuri in functie de tasta apasata de utilizator
				case KEY_UP:
            	                choice--;  
                	            if (choice < 0)
                    	        	choice = 4;
                        	    break;
            	case KEY_DOWN:
                	            choice++;
                    	        if (choice > 4)
                        	    	choice = 0;
                            	break;
                case 10:		           //utilizatorul a selectat o optiune apasand ENTER
                				if (choice == 4) //utilizatorul a selectat "Quit"
                					exitGame = TRUE;
                				break;
                default:
                				break;
			}
			if (k == 10 && (choice != 1 || resume == TRUE)) //s-a apasat ENTER, si, daca s-a selectat Resume, optiunea trebuie sa fie valabila
				break;
			wattron(menu, A_STANDOUT);  //se pune highlight pe text
			mvwprintw(menu, choice + 1, 2, "%s", options[choice]);
			wattroff(menu, A_STANDOUT);
		}
    	delwin(menu);
    	endwin();
    	backToMenu = FALSE;
    	while (!exitGame && !backToMenu) {
			switch(choice) {  
				case 0:                           //"New Game"
						if (gameOver == 2)   //daca abia s-a deschis apllicatia
							askUsername(&request, user);
						else
							if (!gameOver && score > 0) {                              //daca nu, se retine scorul obtinut
								fprintf(f1, "%s\n", user);
       							fprintf(f2, "%d\n", score);
       							fprintf(f3, "%s", getTime());
       							strcpy(v[n].name, user);  //se actualizeaza vectorul
								v[n].name[strlen(user)] = '\0';
								v[n].score = score;
								sum += score;
								strcpy(v[n].time, getTime());
								v[n].time[24] = '\0';
								n++;
								v = (data *)realloc(v, (n + 1) * sizeof(data));
							}
						for (i = 4; i <= 16; i += 4)
							for (j = 6; j <= 30; j += 8)
								cell[i][j] = 0;  //toate celulele sunt goale
						score = 0;   //scorul initial e 0
						lastScore = 0;
            			newBoard(&board, &pannel); 
  						genRandom(&board, cell); 
  						genRandom(&board, cell); 
            			printMatrix(cell, &board);	
  						gameOver = 0;
  						resume = TRUE;
   						break;
   				case 1:          //"Resume"
   						if (resume == TRUE) {  //daca optiunea e disponibila
   							newBoard(&board, &pannel);  
   							if (gameOver == 2) {   //se reia jocul de la sesiunea anterioara a aplicatiei
   								fscanf(f4, "%s", user);  //se extrag informatiile din fisier
    							fscanf(f4, "%d", &score);  
    							for (i = 4; i <= 16; i += 4)
    								for (j = 6; j <= 30; j +=8)
    									fscanf(f4, "%d", &cell[i][j]); 
    							copyMatrix(lastMove, cell);
            					lastScore = score;
    							gameOver = 0;
   							}
   							printMatrix(cell, &board);	//se afiseaza celulele in starea ramasa
   							wrefresh(board);
							wrefresh(pannel);
						}
   						break;	
   				case 2:                  //"Highscores"
						avgScore = sum / n;           //se calculeaza media punctajelor de-a lungul timpului
						qsort(v, n, sizeof(data), compare); //se ordoneaza scorurile
   						topScores(&info, v, n, avgScore);  
   						switch(wgetch(info)) {
   							case 'S':
   							case 's':
   									personalHistory(&info, v, n); //se afiseaza informatiile pentru
   									break;           //un anumit username
   							default:
   									break;
   						}
   						backToMenu = TRUE;
   						delwin(info);
   						endwin();
   						refresh();
   						break;
   				case 3:            //Switch User
   						if (!gameOver && score > 0) {                   //daca jocul anterior nu se terminase,
							fprintf(f1, "%s\n", user);          //retine scorul
  							fprintf(f2, "%d\n", score);
  							fprintf(f3, "%s", getTime());
  							strcpy(v[n].name, user);           //actualizeaza vectorul de structura
							v[n].name[strlen(user)] = '\0';
							v[n].score = score;
							sum += score;
							strcpy(v[n].time, getTime());
							v[n].time[24] = '\0';
							n++;
							v = (data *)realloc(v, (n + 1) * sizeof(data));
							resume = FALSE;          //resume nu mai e valabila
      					}
   						askUsername(&request, user);
   						for (i = 4; i <= 16; i += 4)
							for (j = 6; j <= 30; j += 8)
								cell[i][j] = 0;  //toate celulele devin goale
						score = 0;   //scorul initial e 0
						lastScore = 0; 	
  						gameOver = 0;
  						backToMenu = TRUE;
   						break;
   			}
   			while (!backToMenu) {
				mvwprintw(pannel, 6, 2, "SCORE: %d   ", score);
				wrefresh(pannel);
				if (halfdelay(150) == OK)     //se asteapta 15 secunde pentru apasarea unei taste
            		switch (wgetch(board)) {
       					case 'N':  //New Game
       					case 'n':
								for (i = 4; i <= 16; i += 4)
									for (j = 6; j <= 30; j += 8)
										cell[i][j] = 0;  //se reseteaza celulele
       							if (!gameOver && score > 0) {
									fprintf(f1, "%s\n", user);  //se retine scorul
       								fprintf(f2, "%d\n", score);
       								fprintf(f3, "%s", getTime());
       								strcpy(v[n].name, user);   //se actualizeaza vectorul
									v[n].name[strlen(user)] = '\0';
									v[n].score = score;
									sum += score;
									strcpy(v[n].time, getTime());
									v[n].time[24] = '\0';
									n++;
									v = (data *)realloc(v, (n + 1) * sizeof(data));
								}
								score = 0;  //se reseteaza scorul
								lastScore = 0; 
								newBoard(&board, &pannel);
								genRandom(&board, cell);
  								genRandom(&board, cell);
  								gameOver = 0;
  								resume = TRUE;
   								break;
   						case 'U':  //Undo
   						case 'u':
   								copyMatrix(cell, lastMove);  //se revine la matricea de la ultima mutare
   								score = lastScore;
   								if (gameOver == 1) {
   									gameOver = 0;
   									mvwprintw(board, 20, 4, "%s", "                       ");  //se sterge mesajul "Game over" de pe ecran
   								}
   								break;
   						case 'Q':  //Quit
   						case 'q':
   								backToMenu = TRUE;
   								break;
   						case KEY_UP:  //Mutare directia nord
            					copyMatrix(aux, cell);  //se verifica daca mutarea e valida
   								if (validMove(cell, aux, north) == 1 && gameOver == 0) {
   									copyMatrix(lastMove, cell);  //se copiaza situatia actuala
           							lastScore = score;    //in eventualitatea selectarii UNDO
   									north(cell, &score);  //apoi se efectueaza mutaea
   									genRandom(&board, cell);	
   								}
   								break;
   						case KEY_DOWN:  //Mutare directia sud
   								copyMatrix(aux, cell);
   								if (validMove(cell, aux, south) == 1 && gameOver == 0) {
   									copyMatrix(lastMove, cell);
            						lastScore = score;	
   									south(cell, &score);
   									genRandom(&board, cell);	
   								}	
   								break;
   						case KEY_RIGHT:  //Mutare directia est
   								copyMatrix(aux, cell);
   								if (validMove(cell, aux, east) == 1 && gameOver == 0) {
   									copyMatrix(lastMove, cell);
            						lastScore = score;
   									east(cell, &score);
   									genRandom(&board, cell);	
   								}
   								break;
   						case KEY_LEFT:  //Mutare directi vest
   								copyMatrix(aux, cell);
   								if (validMove(cell, aux, west) == 1 && gameOver == 0) {
   									copyMatrix(lastMove, cell);
            						lastScore = score;
   									west(cell, &score);
   									genRandom(&board, cell);	
   								}
   								break;
   						case -1:              //miscarea automata a celulelor
   								copyMatrix(lastMove, cell);
   								lastScore = score;
            					switch (bestMove(cell)) {  //se determina mutarea cea mai buna
            						case 0:
   											north(cell, &score);
   											break;
									case 1:
											west(cell, &score);
											break;
									case 2:
											south(cell, &score);
											break;
									case 3:
											east(cell, &score);
											break;
								}
            					genRandom(&board, cell);
            					break;
   						default: 
   								break;
       				}
       			if (backToMenu == TRUE) {   //se intoarce la navigarea meniului
       				if (gameOver == 1) {                   //daca jocul s-a terminat,
						fprintf(f1, "%s\n", user);          //retine scorul
  						fprintf(f2, "%d\n", score);
  						fprintf(f3, "%s", getTime());
  						strcpy(v[n].name, user);           //actualizeaza vectorul de structura
						v[n].name[strlen(user)] = '\0';
						v[n].score = score;
						sum += score;
						strcpy(v[n].time, getTime());
						v[n].time[24] = '\0';
						n++;
						v = (data *)realloc(v, (n + 1) * sizeof(data));
						resume = FALSE;          //resume nu mai e valabila
      				}
      				endwin();
      				refresh();
       				break;
       			}
   				if (checkIfLost(cell)) {
   					mvwprintw(board, 20, 4, "%s", "Game over. You lost!");
   					gameOver = 1;
   				}	
   				if (checkIfWon(cell)){
   					mvwprintw(board, 20, 4, "%s", "Game over. You won!");
   					gameOver = 1;
   				}	
   				printMatrix(cell, &board);
			}
			delwin(board);
   			delwin(pannel);
   		}
	}
    if (!gameOver) {                    //daca jocul nu s-a terminat,
    	fprintf(f4, "%s\n", user);    //copiaza situatia tablei de joc
      	fprintf(f4, "%d\n", score);       //pentru posibilitatea de reluare
    	for (i = 4; i <= 16; i += 4)     
    		for (j = 6; j <= 30; j +=8)
    			fprintf(f4, "%d\n", cell[i][j]);
   	}
	endwin();
	delwin(board);
	delwin(pannel);
	delwin(menu);
	delwin(info);
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
	return 0;
}