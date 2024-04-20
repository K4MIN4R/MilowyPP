#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ROWS 40
#define COLS 40
#define CELL_SIZE 21
#define BORDER_THICKNESS 2
#define UI_HEIGHT 2
//#define WINDOW_HEIGHT_OFFSET (UI_HEIGHT * CELL_SIZE)

int snakeX[(ROWS * COLS) + 1]; // +1 for the initial two-block snake
int snakeY[(ROWS * COLS) + 1];
int snakeLength = 2; // Start with two blocks
int direction = GLUT_KEY_RIGHT;
int lastDirection = GLUT_KEY_RIGHT; // Variable to keep track of last direction

int foodX, foodY;
int score = 0;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, COLS * CELL_SIZE, UI_HEIGHT * CELL_SIZE, (ROWS + 2 )*CELL_SIZE);
    srand(time(NULL));
    foodX = (rand() % COLS) * CELL_SIZE;
    foodY = (rand() % ROWS) * CELL_SIZE;

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSnake();
    drawFood();
    UI();
    drawBorders(); // Draw borders
    glutSwapBuffers();
}

void update(int value) {
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
        foodX = (rand() % COLS) * CELL_SIZE;
        foodY = (rand() % ROWS) * CELL_SIZE;
        score++;
    }

    // Check for collisions with tail
    for (int i = 1; i < snakeLength; ++i) {
        if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
            exit(0); // End the game when the snake collides with itself
        }
    }

    // Check for collision with border
    if(snakeX[0] < 0 || snakeX[0] >= COLS * CELL_SIZE || snakeY[0] >= (ROWS-2) * CELL_SIZE || snakeY[0] < 0){
        exit(0);
    }
}

void keyboard(int key, int x, int y) {
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
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(COLS * CELL_SIZE, (ROWS ) * CELL_SIZE);
    glutCreateWindow("Snake Game");
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
