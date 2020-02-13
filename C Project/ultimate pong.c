#include <stdio.h>
#include <SFML/Graphics.h>
#include <chipmunk/chipmunk.h>

void drawText(sfRenderWindow* window, sfFont* font, char *str, sfColor col, int x, int y) {
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setString(text, str);
    sfText_setColor(text, col);
    sfFloatRect dim = sfText_getLocalBounds(text);
    sfText_setPosition(text, (sfVector2f) {x-dim.width/2, y-dim.height/2});
while
    sfRenderWindow_drawText(window, text, NULL);
    sfText_destroy(text);
}

void drawScore(sfRenderWindow* window, sfFont* font, int score, sfColor col, int x, int y) {
    char str[1];
    sprintf(str, "%d", score);
    drawText(window, font, str, col, x, y);
}

int main(void){

	cpVect gravity = cpv(0, 0);
	cpSpace *space = cpSpaceNew();
	cpBody *body = cpBodyNewStatic();
	cpSpaceSetGravity(space, gravity);
	cpSpaceSetDamping(space, 1.001);

	cpShape *ground = cpSegmentShapeNew(body, cpv(0, 590), cpv(800, 600), 0);
	cpShapeSetFriction(ground, 0);
	cpShapeSetElasticity(ground, 1);
	cpSpaceAddShape(space, ground);

	cpShape *ceiling = cpSegmentShapeNew(body, cpv(0, 0), cpv(800, 0), 0);
	cpShapeSetFriction(ceiling, 0);
    cpShapeSetElasticity(ceiling, 1);
	cpSpaceAddShape(space, ceiling);

    // paddle1
	cpFloat massRect = 10000;
	cpFloat width = 10;
	cpFloat height = 50;
	cpFloat momentRect = cpMomentForBox(massRect, width, height);
	cpBody *boxBody = cpSpaceAddBody(space, cpBodyNew(massRect, momentRect));
	cpBodySetPosition(boxBody, cpv(50, 300));
	cpShape *boxShape = cpSpaceAddShape(space, cpBoxShapeNew(boxBody, width, height, 0));
	cpShapeSetFriction(boxShape, 0);
	cpShapeSetElasticity(boxShape, 1);
    cpFloat yVelocity = 0;

    sfVector2f sizeRect;
    sizeRect.x = 10;
    sizeRect.y = 50;
	sfRectangleShape* rectangle;
	rectangle = sfRectangleShape_create();
	sfRectangleShape_setFillColor(rectangle, sfColor_fromRGB(0,100,0));
	sfRectangleShape_setOutlineThickness(rectangle, 2);
	sfRectangleShape_setOutlineColor(rectangle, sfColor_fromRGB(0,160,80));
	sfRectangleShape_setSize(rectangle, sizeRect);
	sfVector2f vectorRect = {100.0, 100.0};

	// paddle2
	cpFloat massRect1 = 10000;
	cpFloat width1 = 10;
	cpFloat height1 = 50;
	cpFloat momentRect1 = cpMomentForBox(massRect1, width1, height1);
	cpBody *boxBody1 = cpSpaceAddBody(space, cpBodyNew(massRect1, momentRect1));
	cpBodySetPosition(boxBody1, cpv(750, 300));
	cpShape *boxShape1 = cpSpaceAddShape(space, cpBoxShapeNew(boxBody1, width1, height1, 0));
	cpShapeSetFriction(boxShape1, 0);
	cpShapeSetElasticity(boxShape1, 1);
    cpFloat yVelocity1 = 0;

    sfVector2f sizeRect1;
    sizeRect1.x = 10;
    sizeRect1.y = 50;
	sfRectangleShape* rectangle1;
	rectangle1 = sfRectangleShape_create();
	sfRectangleShape_setFillColor(rectangle1, sfBlue);
	sfRectangleShape_setOutlineThickness(rectangle1, 2);
	sfRectangleShape_setOutlineColor(rectangle1, sfColor_fromRGB(0,80,200));
	sfRectangleShape_setSize(rectangle1, sizeRect1);
	sfVector2f vectorRect1 = {100.0, 100.0};

    // ball
	cpFloat mass = 1;
	cpFloat radius = 10;
	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);
	cpBody *ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpShape *ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
	cpShapeSetFriction(ballShape, 1);
	cpBodySetPosition(ballBody, cpv(400, 300));
	cpBodySetVelocity(ballBody, cpv(10,0));
	cpShapeSetElasticity(ballShape, 1);

	sfCircleShape* ball;
	ball = sfCircleShape_create();
	sfCircleShape_setFillColor(ball, sfWhite);
	sfCircleShape_setRadius(ball, radius);
	sfVector2f vector = {100.0, 100.0};

	// ball trail
    int i;
	sfCircleShape* trail[5];
	for (i = 0; i < 5; i++) {
        trail[i] = sfCircleShape_create();
        sfCircleShape_setFillColor(trail[i], (sfColor) {255, (250/(i+1)), (250/(i+1)), (5-i)*35});
        sfCircleShape_setRadius(trail[i], radius-i);
        sfCircleShape_setPosition(trail[i], (sfVector2f) {400, 300});
	}

	// divider
	sfVector2f sizeDiv,posDiv;
	posDiv.x = 400;
    posDiv.y = 0;
    sizeDiv.x = 1.0;
    sizeDiv.y = 600.0;
    sfRectangleShape* divider;
    divider = sfRectangleShape_create();
    sfRectangleShape_setSize(divider, sizeDiv);
    sfRectangleShape_setFillColor(divider, sfWhite);
    sfRectangleShape_setPosition(divider, posDiv);

    sfFont* font = sfFont_createFromFile("slkscrb.ttf");

    //score
    int score = 0;
    int score1 = 0;

    //window set up
	cpFloat timeStep = 1/60.0;


	sfVideoMode mode = {800, 600, 24};
	sfRenderWindow* window = sfRenderWindow_create(mode, "PONG", sfResize | sfClose, NULL);
	if (!window){
		puts("Unable to create window, aborting.");
		return EXIT_FAILURE;
	}

    sfKeyCode key;
    int frame = 0;
	sfEvent event;
	while (sfRenderWindow_isOpen(window)){
		while (sfRenderWindow_pollEvent(window, &event)){
			if (event.type == sfEvtClosed){
				puts("Closing render window.");
				sfRenderWindow_close(window);
			} else if (event.type == sfEvtKeyPressed){
			    key = event.key.code;
                //paddle1 moves
                if (key == sfKeyW){
                    yVelocity = -0.1;
                }
                if (key == sfKeyS){
                    yVelocity = 0.1;
                }
                //paddle2 moves
                if (key == sfKeyUp){
                    yVelocity1 = -0.1;
                }
                if (key == sfKeyDown){
                    yVelocity1 = 0.1;
                }

			}
			else if (event.type == sfEvtKeyReleased){
			    key = event.key.code;
                //paddle1 moves
                if (key == sfKeyW){
                    yVelocity = 0;
                }
                if (key == sfKeyS){
                    yVelocity = 0;
                }
                //paddle2 moves
                if (key == sfKeyDown){
                    yVelocity1 = 0;
                }
                if (key == sfKeyUp){
                    yVelocity1 = 0;
                }
			}
		}

		cpVect posRect = cpBodyGetPosition(boxBody);
		cpBodySetPosition(boxBody, cpv(50, posRect.y+yVelocity));
		posRect = cpBodyGetPosition(boxBody);
		vectorRect.x = posRect.x-(width/2);
		vectorRect.y = posRect.y-(height/2);
		sfRectangleShape_setPosition(rectangle, vectorRect);
		sfRenderWindow_drawRectangleShape(window, rectangle, NULL);

		cpVect posRect1 = cpBodyGetPosition(boxBody1);
		cpBodySetPosition(boxBody1, cpv(750, posRect1.y+yVelocity1));
		posRect1 = cpBodyGetPosition(boxBody1);
		vectorRect1.x = posRect1.x-(width/2);
		vectorRect1.y = posRect1.y-(height/2);
		sfRectangleShape_setPosition(rectangle1, vectorRect1);
		sfRenderWindow_drawRectangleShape(window, rectangle1, NULL);

		cpVect pos = cpBodyGetPosition(ballBody);
		vector.x = pos.x - radius;
		vector.y = pos.y - radius;
		sfCircleShape_setPosition(ball, vector);

        if (frame%70 == 0) {
            for (i = 4; i > 0; i--) {
                sfVector2f prev = sfCircleShape_getPosition(trail[i-1]);
                sfCircleShape_setPosition(trail[i], (sfVector2f) {prev.x+1, prev.y+1});
            }
            sfCircleShape_setPosition(trail[0], (sfVector2f) {vector.x, vector.y});
        }

		for (i = 4; i >= 0; i--) {
            sfRenderWindow_drawCircleShape(window, trail[i], NULL);
		}

		sfRenderWindow_drawCircleShape(window, ball, NULL);

        if (cpBodyGetPosition(ballBody).x < 5){
            score1 += 1;
            cpBodySetPosition(ballBody, cpv(400, 300));
            cpBodySetVelocity(ballBody, cpv(10, 0));
            cpBodySetPosition(boxBody, cpv(50, 300));
            cpBodySetPosition(boxBody1, cpv(750, 300));
        }

		if (cpBodyGetPosition(ballBody).x > 795){
            score += 1;
            cpBodySetPosition(ballBody, cpv(400, 300));
            cpBodySetVelocity(ballBody, cpv(-10, 0));
            cpBodySetPosition(boxBody, cpv(50, 300));
            cpBodySetPosition(boxBody1, cpv(750, 300));
		}
        drawScore(window, font, score, sfWhite, 350, 10);
        drawScore(window, font, score1, sfWhite, 450, 10);

        if (score >= 5 || score1 >= 5){
            sfRenderWindow_clear(window, sfBlack);
            drawText(window, font, "GAME OVER", sfRed, 400, 200);
            if (score > score1){
                drawText(window, font, "PLAYER 1 WINS!!", sfColor_fromRGB(70, 0, 10), 400, 400);
            } else {
                drawText(window, font, "PLAYER 2 WINS!!", sfColor_fromRGB(70, 0, 10), 400, 400);
            }
        } else {
            sfRenderWindow_drawRectangleShape(window, divider, NULL);
        }


		sfRenderWindow_display(window);
		sfRenderWindow_clear(window, sfBlack);

		cpSpaceStep(space, timeStep);
		frame++;
	}

    cpShapeFree(boxShape);
    cpShapeFree(boxShape1);
    cpShapeFree(ball);
    cpShapeFree(ground);
    cpShapeFree(ceiling);
    cpSpaceFree(space);

    sfFont_destroy(font);
    sfRectangleShape_destroy(rectangle);
    sfRectangleShape_destroy(rectangle1);
    sfRectangleShape_destroy(divider);
    sfRenderWindow_destroy(window);
    sfCircleShape_destroy(ball);
    for(i=0; i<5; i++){
        sfCircleShape_destroy(trail[i]);
    }

    return 0;
}
