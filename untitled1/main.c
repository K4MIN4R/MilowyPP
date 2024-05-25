#define GLFW_INCLUDE_NONE
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define ROWS 40
#define COLS 40
#define CELL_SIZE 21
#define BORDER_THICKNESS 2
#define UI_HEIGHT 2
//#define WINDOW_HEIGHT_OFFSET (UI_HEIGHT * CELL_SIZE)

enum GameState{PLAYING, GAME_OVER};
enum GameState gameState = PLAYING;

int snakeX[(ROWS * COLS) + 1]; // +1 for the initial two-block snake
int snakeY[(ROWS * COLS) + 1];
int snakeLength = 2; // Start with two blocks
int direction = GLUT_KEY_RIGHT;
int lastDirection = GLUT_KEY_RIGHT; // Variable to keep track of last direction

int foodX, foodY;
int score = 0;
int randX, randY;

void generateFood(){
    srand(time(NULL));
    randX = rand() % (39+1-0) + 0 ;
    foodX = randX * CELL_SIZE;
    printf("foodX: %d\n", foodX);
    randY = rand() % (37 + 1 - 4)+4;
    foodY = randY * CELL_SIZE;
    printf("foodY: %d\n", foodY);



}




void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, COLS * CELL_SIZE, UI_HEIGHT * CELL_SIZE, (ROWS + 2 )*CELL_SIZE);
    srand(time(NULL));
    randX = rand() % (40+1-0) + 0 ;
    foodX = randX * CELL_SIZE;
//    printf("foodX: %d\n", foodX);

    randY = rand() % (37 + 1 - 4)+4;
    foodY = randY * CELL_SIZE;
 //   printf("foodY: %d\n", foodY);
   // foodY = (rand() % (ROWS - 2)*CELL_SIZE) + CELL_SIZE;
/*
    foodX = (rand() % COLS) * CELL_SIZE;

*/
    // Initialize snake with two blocks
    snakeX[0] = COLS * CELL_SIZE / 2; // Centered horizontally
    snakeY[0] = (ROWS + UI_HEIGHT) * CELL_SIZE / 2; // Centered vertically
    snakeX[1] = snakeX[0] - CELL_SIZE; // One block to the left
    snakeY[1] = snakeY[0]; // Same row as the head
}





void drawCell(int x, int y) {
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + CELL_SIZE, y);
    glVertex2i(x + CELL_SIZE, y + CELL_SIZE);
    glVertex2i(x, y + CELL_SIZE);
    glEnd();
}


void drawSnake() {
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < snakeLength; ++i) {
        drawCell(snakeX[i], snakeY[i]);
    }
}

void drawFood() {
    glColor3f(1.0, 0.0, 0.0);
    drawCell(foodX, foodY);
}

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
       //     printf("value_snakeY: ");
            printf("foodX: %d\n", foodX); // Do Debugowania
            printf("foodY: %d\n", foodY); // do Debugowania
          //  printf( "%d\n", snakeY[0]);
          //  printf("snakeX: ");
    //printf("%d\n", snakeX[0]);

            if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
                gameState = GAME_OVER;
                break;
            }

            }



        // Check for collision with border
        if(snakeX[0] < 0 || snakeX[0] >= COLS * CELL_SIZE || snakeY[0] >= (ROWS-2) * CELL_SIZE || snakeY[0] < (4*CELL_SIZE)){
            gameState = GAME_OVER;

        /*  //  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Generate a blank background, use swapbuffers to clear the second background - game
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            glutSwapBuffers();
            sleep(5);
            exit(0);
    */
        }
    }
}


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


void reshape(int w, int h){
    if(w / (float)COLS < h / (float)ROWS){

        h = w / COLS * ROWS;
    }else {
        w = h / ROWS * COLS;
    }
    glViewport(0, 0, w, h);





}

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

