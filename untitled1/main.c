/*!
 * \file
 * \brief Implementacja prostej gry Snake z użyciem OpenGL i GLUT.
 * \details Plik zawiera implementację gry Snake z podstawowymi funkcjonalnościami takimi jak ruch, generowanie jedzenia i wykrywanie kolizji.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


#define ROWS 40 ///< Liczba wierszy planszy gry.
#define COLS 40 ///< Liczba kolumn planszy gry.
#define CELL_SIZE 21 ///< Rozmiar komórki w pikselach.
#define BORDER_THICKNESS 2 ///< Grubość granic obszaru gry.
#define UI_HEIGHT 2 ///< Wysokość interfejsu użytkownika w komórkach.

/*!
 * \enum GameState
 * \brief Stan gry
 */

enum GameState{PLAYING, GAME_OVER};
/*!
 * \var gameState
 * \brief Aktualny stan gry
 */
enum GameState gameState = PLAYING;
/*!
 * \var snakeX
 * \brief Tablica przechowująca współrzędne X węża
 */
int snakeX[(ROWS * COLS) + 1]; // +1 for the initial two-block snake



/*!
 * \var snakeY
 * \brief Tablica przechowująca współrzędne Y węża
 */

int snakeY[(ROWS * COLS) + 1];


/*!
* \var snakeLength
* \brief Długość węża
*/
int snakeLength = 2; // Start with two blocks
/*!
 * \var direction
 * \brief Kierunek ruchu węża
 */

int direction = GLUT_KEY_RIGHT;

/*!
 * \var lastDirection
 * \brief Ostatni kierunek ruchu węża
 */

int lastDirection = GLUT_KEY_RIGHT; // Variable to keep track of last direction
/*!
 * \var foodX
 * \brief Współrzędna X jedzenia
 */
int foodX;

/*!
 * \var foodY
 * \brief Współrzędna Y jedzenia
 */

int foodY;


/*!
 * \var score
 * \brief Ustawienie wyniku gracza na 0
 */
int score = 0;

/*!
 * \var randX
 * \brief Losowa współrzędna X do generowania jedzenia
 */

int randX;

/*!
 * \var randY
 * \brief Losowa współrzędna Y do generowania jedzenia
 */
int randY;
/*!
 * \brief Generuje jedzenie na losowej pozycji.
 */
void generateFood(){
    srand(time(NULL));
    randX = rand() % (39+1-0) + 0 ;
    foodX = randX * CELL_SIZE;
    randY = rand() % (37 + 1 - 4)+4;
    foodY = randY * CELL_SIZE;



}



/*!
 * \brief Inicjalizuje stan gry i ustawienia OpenGL.
 */

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, COLS * CELL_SIZE, UI_HEIGHT * CELL_SIZE, (ROWS + 2 )*CELL_SIZE);
    srand(time(NULL));
    randX = rand() % (40+1-0) + 0 ;
    foodX = randX * CELL_SIZE;


    randY = rand() % (37 + 1 - 4)+4;
    foodY = randY * CELL_SIZE;


    // Initialize snake with two blocks
    snakeX[0] = COLS * CELL_SIZE / 2; // Centered horizontally
    snakeY[0] = (ROWS + UI_HEIGHT) * CELL_SIZE / 2; // Centered vertically
    snakeX[1] = snakeX[0] - CELL_SIZE; // One block to the left
    snakeY[1] = snakeY[0]; // Same row as the head
}



/*!
 * \brief Rysuje komórkę na określonych współrzędnych.
 * \param[in] x Współrzędna x komórki.
 * \param[in] y Współrzędna y komórki.
 */

void drawCell(int x, int y) {
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + CELL_SIZE, y);
    glVertex2i(x + CELL_SIZE, y + CELL_SIZE);
    glVertex2i(x, y + CELL_SIZE);
    glEnd();
}

/*!
 * \brief Rysuje węża na ekranie.
 */

void drawSnake() {
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < snakeLength; ++i) {
        drawCell(snakeX[i], snakeY[i]);
    }
}

/*!
 * \brief Rysuje jedzenie na ekranie.
 */

void drawFood() {
    glColor3f(1.0, 0.0, 0.0);
    drawCell(foodX, foodY);
}

/*!
 * \brief Rysuje granice obszaru gry.
 */

void drawBorders() {
    glColor3f(1.0, 1.0, 1.0); // Border color
    glLineWidth(BORDER_THICKNESS);

    // Draw top border
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, (ROWS - UI_HEIGHT) * CELL_SIZE);
    glVertex2i(0, (ROWS - UI_HEIGHT) * CELL_SIZE - BORDER_THICKNESS);
    glVertex2i(COLS * CELL_SIZE, (ROWS - UI_HEIGHT) * CELL_SIZE - BORDER_THICKNESS);
    glVertex2i(COLS * CELL_SIZE, (ROWS - UI_HEIGHT) * CELL_SIZE);
    glEnd();

    // Draw bottom border
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, (ROWS+1)*BORDER_THICKNESS);
    glVertex2i(0,0);
    glVertex2i(COLS * CELL_SIZE, 0);
    glVertex2i(COLS * CELL_SIZE, (ROWS+1)*BORDER_THICKNESS);
    glEnd();

    // Draw left border
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(BORDER_THICKNESS, 0);
    glVertex2i(BORDER_THICKNESS, (ROWS + UI_HEIGHT) * CELL_SIZE);
    glVertex2i(0, (ROWS + UI_HEIGHT) * CELL_SIZE);
    glEnd();

    // Draw right border
    glBegin(GL_LINE_LOOP);
    glVertex2i(COLS * CELL_SIZE - BORDER_THICKNESS, 0);
    glVertex2i(COLS * CELL_SIZE, 0);
    glVertex2i(COLS * CELL_SIZE, (ROWS + UI_HEIGHT) * CELL_SIZE);
    glVertex2i(COLS * CELL_SIZE - BORDER_THICKNESS, (ROWS + UI_HEIGHT) * CELL_SIZE);
    glEnd();
}

/*!
 * \brief Wyświetla wynik na ekranie.
 */

void UI(){
    glColor3f(1.0, 1.0,1.0);
    glRasterPos2i(10,(ROWS + UI_HEIGHT) * CELL_SIZE - 20);
    char scoreText[100];
    sprintf(scoreText,"Score: %d", score);
    int len = strlen(scoreText);
    for(int i = 0; i < len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);

    }


}

/*!
 * \brief Rysuje ekran końca gry z końcowym wynikiem i instrukcjami restartu.
 */

void drawEndgameScreen(){
    glColor3f(1.0,1.0,1.0);
    char endgameText[100];
    glRasterPos2i(COLS * CELL_SIZE / 2 - 80, (ROWS + UI_HEIGHT) * CELL_SIZE / 2 +20);
    sprintf(endgameText, "Game Over! Score: %d", score);
    int len = strlen(endgameText);
    for (int i = 0; i < len; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, endgameText[i]);
    }

    char restartText[] = "Press F1 to Restart";
    glRasterPos2i(COLS * CELL_SIZE / 2 - 80, (ROWS + UI_HEIGHT) * CELL_SIZE / 2 - 20);
    len = strlen(restartText);
    for (int i = 0; i < len; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, restartText[i]);
    }

}





/*!
 * \brief Główna funkcja wyświetlania. Rysuje elementy gry w zależności od aktualnego stanu gry.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameState == PLAYING) {
        drawSnake();
        drawFood();
        UI();
        drawBorders(); // Draw borders
    }else if (gameState == GAME_OVER){
        drawEndgameScreen();
    }
    glutSwapBuffers();
}
/*!
 * \brief Aktualizuje stan gry i ruch węża.
 *
 * Aktualizuje stan gry, sprawdzając kolizje, ruch węża i wywołując ponowne rysowanie ekranu.
 *
 * \param[in] value Wartość przekazywana do funkcji timer'a GLUT (nieużywana).
 */
void update(int value) {

    if (gameState == PLAYING){


        glutPostRedisplay();
        glutTimerFunc(100, update, 0);

        // Move the snake
        for (int i = snakeLength - 1; i > 0; --i) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        switch (direction) {
            case GLUT_KEY_UP:
                if (lastDirection != GLUT_KEY_DOWN && snakeLength > 1)
                    snakeY[0] += CELL_SIZE;
                break;
            case GLUT_KEY_DOWN:
                if (lastDirection != GLUT_KEY_UP && snakeLength > 1)
                    snakeY[0] -= CELL_SIZE;
                break;
            case GLUT_KEY_LEFT:
                if (lastDirection != GLUT_KEY_RIGHT && snakeLength > 1)
                    snakeX[0] -= CELL_SIZE;
                break;
            case GLUT_KEY_RIGHT:
                if (lastDirection != GLUT_KEY_LEFT && snakeLength > 1)
                    snakeX[0] += CELL_SIZE;
                break;
        }

        // Update last direction
        lastDirection = direction;

        // Check if snake eats food
        if (snakeX[0] == foodX && snakeY[0] == foodY) {
            snakeLength++;
            generateFood();
            score++;
        }

        // Check for collisions with tail
        for (int i = 1; i < snakeLength; ++i) {

            if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
                gameState = GAME_OVER;
                break;
            }

        }



        // Check for collision with border
        if(snakeX[0] < 0 || snakeX[0] >= COLS * CELL_SIZE || snakeY[0] >= (ROWS-2) * CELL_SIZE || snakeY[0] < (4*CELL_SIZE)){
            gameState = GAME_OVER;


        }
    }
}

/*!
 * \brief Restartuje grę, przywracając początkowe ustawienia.
 */

void restartGame(){
    score = 0;
    snakeLength = 2;
    direction = GLUT_KEY_RIGHT;
    lastDirection = GLUT_KEY_RIGHT;

    snakeX[0] = COLS * CELL_SIZE / 2; // Centered horizontally
    snakeY[0] = (ROWS + UI_HEIGHT) * CELL_SIZE / 2; // Centered vertically
    snakeX[1] = snakeX[0] - CELL_SIZE; // One block to the left
    snakeY[1] = snakeY[0]; // Same row as the head

    generateFood();
    gameState = PLAYING;

    glutTimerFunc(0, update, 0);
}
/*!
 * \brief Obsługuje zdarzenie naciśnięcia klawisza F1 w stanie zakończonej gry, restartując grę.
 * \param[in] key Kod klawisza naciśniętego przez użytkownika.
 * \param[in] x Pozycja węża na osi x.
 * \param[in] y Pozycja węża na osi y.
 */
void keyboardNormal(unsigned char key, int x, int y){
    if (gameState == GAME_OVER && key == GLUT_KEY_F1){
        score = 0;
        snakeLength = 2;
        direction = GLUT_KEY_RIGHT; // Reset direction to initial value
        lastDirection = GLUT_KEY_RIGHT;

        // Reinitialize snake position
        snakeX[0] = COLS * CELL_SIZE / 2; // Centered horizontally
        snakeY[0] = (ROWS + UI_HEIGHT) * CELL_SIZE / 2; // Centered vertically
        snakeX[1] = snakeX[0] - CELL_SIZE; // One block to the left
        snakeY[1] = snakeY[0]; // Same row as the head

        generateFood();

        gameState = PLAYING;

        glutPostRedisplay();
    }
}
/*!
 * \brief Obsługuje zdarzenia naciśnięcia klawiszy strzałek lub klawisza F1.
 *
 * Jeśli gra jest w trakcie, reaguje na naciśnięcia klawiszy strzałek, zmieniając kierunek ruchu węża.
 * Jeśli gra jest zakończona, reaguje na naciśnięcie klawisza F1, aby zrestartować grę.
 *
 * \param[in] key Kod klawisza naciśniętego przez użytkownika.
 * \param[in] x Pozycja węża na osi x.
 * \param[in] y Pozycja węża na osi y.
 */
void keyboard(int key, int x, int y) {
    if (gameState == PLAYING) {

        switch (key) {
            case GLUT_KEY_UP:
                if (lastDirection != GLUT_KEY_DOWN && snakeLength > 1)
                    direction = key;
                break;
            case GLUT_KEY_DOWN:
                if (lastDirection != GLUT_KEY_UP && snakeLength > 1)
                    direction = key;
                break;
            case GLUT_KEY_LEFT:
                if (lastDirection != GLUT_KEY_RIGHT && snakeLength > 1)
                    direction = key;
                break;
            case GLUT_KEY_RIGHT:
                if (lastDirection != GLUT_KEY_LEFT && snakeLength > 1)
                    direction = key;
                break;
        }
    }else if(gameState == GAME_OVER && key == GLUT_KEY_F1){
        restartGame();
        init();
    }
}

/*!
 * \brief Zmienia rozmiar okna gry, utrzymując proporcje komórek.
 * \param[in] w Nowa szerokość okna.
 * \param[in] h Nowa wysokość okna.
 */

void reshape(int w, int h){
    if(w / (float)COLS < h / (float)ROWS){

        h = w / COLS * ROWS;
    }else {
        w = h / ROWS * COLS;
    }
    glViewport(0, 0, w, h);





}

/*!
 * \brief Funkcja główna programu.
 *
 * Inicjalizuje bibliotekę GLUT, tworzy okno gry, ustawia funkcje zwrotne dla różnych zdarzeń,
 * inicjalizuje stan gry, generuje jedzenie i uruchamia główną pętlę programu.
 *
 * \param[in] argc Liczba argumentów wiersza poleceń.
 * \param[in] argv Tablica argumentów wiersza poleceń.
 * \return Kod zakończenia programu.
 */

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(COLS * CELL_SIZE, (ROWS ) * CELL_SIZE);
    glutCreateWindow("Snake Game");
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;

}
