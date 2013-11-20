#pragma once

#include "lib.h"
#include "glut\glut.h"
#include "cudaKernel.h"


// The one and only application object


// The one and only application object

//HFONT c;

//Kich thuoc ban dau cua cua so
int wWidth = 1000;	
int wHigh = 500;
int kindOfGeometric = 0;

bool bDraw = false;
bool bModified = true;

bool chosenGeometric = false;
bool wantChooseGeometric = true;

int x_1, y_1;



int object_num = 0;


void GenerateMenu();



void InitSetup()
{
	//Neu hinh nen khong co nhu cau thay doi mau nen thi ham glClearColor chi can goi 1 lan
	//Nhung lan sau muon xoa man hinh thi chi can goi glClear() la OpenGL tu dong lay mau da
	//duoc set bang glClearColor de to
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	//Thu vien glut khong tu dong set 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluOrtho2D( 0, wWidth, 0, wHigh );

	
	
}

#define PI 3.1415926535898 

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
        glVertex2f(x, y);
        for( int n = 0; n <= segments; ++n ) {
            float const t = 2*PI*(float)n/(float)segments;
            glVertex2f(x + sin(t)*r, y + cos(t)*r);
        }
    glEnd();
}

void drawAllElectrons(){

	glColor3f(1,1,1);
	for(int atomid = 0; atomid < numElectronsMap; atomid+= 4 ){
		float y = hElectronsMap[atomid+1];
		float x = hElectronsMap[atomid];
		float energyOfAtom = hElectronsMap[atomid+3];
		circle(x, y, energyOfAtom,100);
	}
}

void drawAllGrid(){

	glColor3f(0,1,0);
	 for(int i = 0; i < numRowsPointsMap; i++){
		 glBegin(GL_LINES);
			glVertex2i(0, yLimitedBottom + i*gridspacing);
			glVertex2i(0+numColsPointsMap*gridspacing, 0 + i*gridspacing);
		glEnd();
	 }
	 for(int i = 0; i < numColsPointsMap; i++){
		 glBegin(GL_LINES);
			glVertex2i(0 + i*gridspacing, 0 );
			glVertex2i(0+ i*gridspacing, 0 + numRowsPointsMap*gridspacing);
		glEnd();
	 }
}

void drawAllEnergyPoint(){
	
	glColor3f(1,0,0);
	 for(int i = 0; i < numRowsPointsMap; i++){
		 for(int j = 0; j < numColsPointsMap; j++){
			 float x = j*gridspacing;
			 float y = i*gridspacing;
			 float r = hEnergyPointsMap[i * numColsPointsMap + j];
			 
			circle(x, y, r/10.0, 100);
		
		 }
	 }
	
	
	/*
	 for(int i = 0; i < numRowsPointsMap/5; i++){
		 for(int j = 0; j < numColsPointsMap/5; j++){
			 cout << hEnergyPointsMap[i * numColsPointsMap + j] << " ";
		 }
		 cout << endl << endl;
	 }
	*/ 
	 //cout << "Done";
}

void randomPositionOfElectrons(){

	for(int atomid = 0; atomid < numElectronsMap; atomid+= 4 ){
		int xMove = abs(rand() % 100 -2);
		if((hElectronsMap[atomid] + xMove > xLimitedLeft) && (hElectronsMap[atomid] + xMove < xLimitedRight)   ){
			hElectronsMap[atomid] += xMove;
		}
		int yMove = abs(rand() % 100 -2);
		if((hElectronsMap[atomid + 1] + yMove > yLimitedBottom) && (hElectronsMap[atomid + 1] + yMove < yLimitedTop)   ){
			hElectronsMap[atomid + 1] += yMove;
		}
	}

	glutPostRedisplay();

}

void RefreshScene()
{

	//glClearColor( fRed, fGreen, fBlue, 1.0 );
//	GenerateMenu();
	glClear( GL_COLOR_BUFFER_BIT );

	drawAllGrid();
	calculateEnergyGridsOnGPU();

	drawAllEnergyPoint();
	drawAllElectrons();
	randomPositionOfElectrons();
	//glutPostRedisplay();

	glFlush();
}




void KeyPress( unsigned char iKey, int x, int y )
{
	switch ( iKey )
	{
	case 43:
		{
			
			break;
		}
	case 45:
		{
	
			break;
		}
	case 108: // l		
	case 76:// L
		{
		
			break;
		}
	case 114: // r		
	case 82:// R
		{
			
			break;
		}
	case 13: // enter
		{
		/*	bDraw = false;
			first->toEnd();
			pCur = first;*/
			break;
		}
	case 113:	//q: Quit, Hay loi doi voi ung dung single window
		{
			int iCurID = glutGetWindow();
			glutDestroyWindow( iCurID );
			
			exit( 0 );

			break;
		}

	case 102:	//f: fullscreen
		{
			glutFullScreen();
			break;
		}

	}
	//glutPostRedisplay();
}

void KeyPressSp( int iKey, int x, int y )
{
	switch(iKey){
	case GLUT_KEY_LEFT: // trai
		{
		
			break;
		}
	case GLUT_KEY_RIGHT:
		{
		
			break;
		}
	case GLUT_KEY_UP: 
		{
			
			break;
		}
	case GLUT_KEY_DOWN: // trai
		{
			
			break;
		}
	}
	//glutPostRedisplay();
}
void MouseLeftDown( int iButton, int iState, int x, int y )
{
	if ( iButton == GLUT_LEFT_BUTTON )
	{
		if ( iState == GLUT_DOWN )
		{
	

				/*glutPostRedisplay();*/
		}else{
			
		}
		//glutPostRedisplay();
	}else{

	}
}

void ActivMouseMove(int x, int y){

	//glutPostRedisplay();
}


void ReshapeScene( int w, int h )
{
	wHigh = h;
	wWidth = w;

	glViewport( 0, 0, w, h );
	glLoadIdentity();
	glOrtho( 0, w, 0, h, 1.0, -1.0 );

	//glutPostRedisplay();
}

void SelMenuColor( int iID )
{
	switch ( iID )
	{
	case 0:	//Red
		
		break;

	case 1:	//Green
		
		break;

	case 2:	//Blue
	
		break;

	case 3:	//yellow
		
		break;

	case 4:	//magenta
		
		break;
	case 5:	//white
		
		break;
	}

	//glutPostRedisplay();
}

void SelMenuLine( int iID )
{

	if( iID == 0  )
	{

	}
	if( iID == 1 )
	{

	}
	//glutPostRedisplay();
}

void SelMenuChose( int iID )
{

}


void GenerateMenu()
{
	glutAttachMenu( GLUT_RIGHT_BUTTON );
}


 
void cleanUpEverything(){
//	delete(hEnergyPointsMap);
//	delete(hElectronsMap);
}

int openGLMain(int argc, char* argv[]){
		// Initialise GLFW


	int nRetCode = cudaMain(argc, argv);
		// TODO: code your application's behavior here.
		glutInit( &argc, argv );
		glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA );
		glutInitWindowPosition( 100, 100 );
		glutInitWindowSize( wWidth, wHigh );
		glutCreateWindow( "My window" );



		//Cac setup khac
		InitSetup();

		glutDisplayFunc( RefreshScene );
		
		glutReshapeFunc( ReshapeScene );

		glutKeyboardFunc( KeyPress );
		glutSpecialFunc( KeyPressSp);

		glutMouseFunc( MouseLeftDown );

		glutMotionFunc( ActivMouseMove );

	//	GenerateMenu();
		atexit(cleanUpEverything);

		glutMainLoop();



		cleanUpEverything();

	return nRetCode;
}