#include <iostream>
#include <ncurses.h>
using namespace std;

// Konfiguracja ncurses
void config() {
    keypad(stdscr, TRUE); //wykrywa strzalki
    cbreak();
    noecho(); //nie wypisuja sie znaki byle jakie z klawiatury
    curs_set(0);
}

// Tworzy figurę
void shape(int size, int y, int x, char ch, char w) {
    clear();
    size *= 2;
    for(int i = 0; i < 2*size; i++) {
        for(int j = 0; j < 2*size; j++) {
            if((j == 2 * i) && (j<(size))) {
                // Dolna część figury
                mvaddch(y - i - 1, x - j - 1, ch);
            }
            else if((j - size == 2 * (i - size - 1)) && (j<size)) {
                // Górna część figury
                mvaddch(y - i + 1, x - size + j + 1, ch);
            }
            if((j>size-1) && (i == size/2) && (j%2==1)) {
                // Linia boczna
                mvaddch(y - i, x - j + 2, ch);
            }
        }
    }
    mvaddch(y-1,x-1 , w);// jest przesuniety bo spacje
}

// Umożliwia poruszanie figurą i zmianę rozmiaru
void move_shape(int& size, int& y, int& x, int key) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); //max wymiary termimnala

    switch(key) {
        case KEY_UP:
            if(y >= size * 2)
                --y;
            break;
        case KEY_DOWN:
            if(y < yMax)
                ++y;
            break;
        case KEY_LEFT:
            if(x >= (size * 4)-2)
                --x;
            break;
        case KEY_RIGHT:
            if(x < xMax)
                ++x;
            break;
        case '-':
            if( size >= 3)
                --size;
            break;
        case '+':
        case '=':
            if(x >= size * 2 && y > size * 2)
                ++size;
            break;
        case KEY_RESIZE:
            if(size * 2 >= yMax || size * 2 >= xMax)
                --size;
            if(y >= yMax || x >= xMax) {
                y = yMax - 1;
                x = xMax - 1;
            }
            break;
    }
}

// Wyświetla kształt na ekranie
void draw(int& size, int& y, int& x, int ch) {
    while(true) {
        shape(size, y, x, ch,'.');  //rysowanie kształtu
        refresh();

        int key = getch();
        move_shape(size, y, x, key);
        refresh();

        if(key == 'q')
            break;
    }
}

int main() {
    cout << "Wciśnij Q, aby zakończyć działanie programu" << endl;

    // Określenie rozmiaru i znaku
    int size;
    cout << "Wprowadź rozmiar figury: ";
    cin >> size;

    char ch;
    cout << "Wprowadź znak: ";
    cin >> ch;

    // Start ncurses
    initscr();
    config();

    // Początkowe wartości y i x
    int y, x;
    getmaxyx(stdscr, y, x);

    draw(size, y, x, ch);

    endwin();
    return 0;
}