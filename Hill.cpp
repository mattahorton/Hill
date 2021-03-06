//-----------------------------------------------------------------------------
// name: Hill.cpp
// desc: Music 256A Final Project
//
// author: Matt Horton (mattah@ccrma.stanford.edu)
//   date: fall 2014
//   uses: RtAudio by Gary Scavone
//-----------------------------------------------------------------------------
#include "RtAudio.h"
#include "color.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <cstring>
#include <sys/time.h>
#include <time.h>
#include "y-entity.h"
#include "globals.h"
#include "bk-sim.h"
#include "Mediator.h"
#include "audio.h"
#include <FTGL/ftgl.h>
#include "ScoreParser.h"
#include "SndSrc.h"
#include "Mediator.h"
using namespace std;

#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef __UNIX_JACK__
#include <GL/glut.h>
#endif




//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void initGfx();
void idleFunc();
void displayFunc();
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void specialFunc( int, int, int );
void mouseFunc( int button, int state, int x, int y );
void help();
void drawTerrain();
void LoadRawFile(char * strName, size_t nSize, uint8_t *pHeightMap);
void LoadWavFile(const char * strName);
void LoadJSONFile(char * strName);
int Height(uint8_t *pHeightMap, float x, float z);
void nextLine();
void updateMix(float radius);
void fadeOut();

// our datetype
#define SAMPLE float
// corresponding format for RtAudio
#define MY_FORMAT RTAUDIO_FLOAT32
// for convenience
#define MY_PIE 3.14159265358979

// refresh rate settings
long time_pre = 0;
int refresh_rate = 15000; //us
struct timeval timer;
// global buffer
long g_bufferSize;

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400


uint8_t g_HeightMap[MAP_SIZE*MAP_SIZE];    // Holds The Height Map Data ( NEW )

GLsizei g_height, g_width = 400;

// Camera position and orientation
iSlew3D cam = iSlew3D(500,200,400);
//float camX = 500, camY = 200, camZ = 400;
float camAngle = atan(cam.actual().z/cam.actual().x);
float r = sqrt(cam.actual().x*cam.actual().x+cam.actual().z*cam.actual().z);
float persp = 50.0f;
GLfloat ratioz = 1.0f;

std::vector<float> lineTimes;

//-----------------------------------------------------------------------------
// Name: LoadRawFile( )
// Desc: load raw file for height map
//-----------------------------------------------------------------------------
void LoadRawFile(char * strName, size_t nSize, uint8_t *pHeightMap)
{
    FILE *pFile = fopen( strName, "rb" );

    // Check To See If We Found The File And Could Open It
    if ( pFile == NULL )
    {
        // Display Error Message And Stop The Function
        cerr << "Can't Find The Height Map!"<<endl;
        exit(1);
    }

    // Here We Load The .RAW File Into Our pHeightMap Data Array
    // We Are Only Reading In '1', And The Size Is (Width * Height)
    size_t read = fread( pHeightMap, 1, nSize, pFile );

    // After We Read The Data, It's A Good Idea To Check If Everything Read Fine
    if (ferror( pFile ))
    {
        cerr << "Failed To Get Data!" << endl;
    }

    // Close The File
    fclose(pFile);
}

//-----------------------------------------------------------------------------
// Name: LoadJSONFile( )
// Desc: load JSON file for poem
//-----------------------------------------------------------------------------
void LoadJSONFile(char * strName)
{
    FILE *pFile = fopen( strName, "rb" );

    // Check To See If We Found The File And Could Open It
    if ( pFile == NULL )
    {
        // Display Error Message And Stop The Function
        cerr << "Can't Find The Poem!"<<endl;
        exit(1);
    }

    char buffer[65536];

    // initialize ScoreParser
    Globals::parser = new ScoreParser();
    // create file read stream
    rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
    // parse file read stream
    Globals::parser->score.ParseStream<0, UTF8<>, FileReadStream>(is);
    // init score
    Globals::parser->initScore();
    // instantiate mediator
    Globals::mediator = new Mediator();

    // Get lines and their times
    const Value& lines = Globals::parser->score["lines"];
    assert(lines.IsArray());
    string l;
    float t = 0;

    for (SizeType i = 0; i < lines.Size(); i++) {

      // Add poem times
      Globals::lineStrings.push_back(lines[i]["text"].GetString());

      // Add camera positions
      if(lines[i].HasMember("cam")){
        Globals::lineRadii.push_back((float)lines[i]["cam"]["r"].GetDouble());
        Globals::lineAngles.push_back((float)lines[i]["cam"]["angle"].GetDouble());
      } else if (Globals::lineRadii.size() > 0){
        Globals::lineRadii.push_back(Globals::lineRadii.at(Globals::lineRadii.size()-1));
        Globals::lineAngles.push_back(Globals::lineAngles.at(Globals::lineAngles.size()-1));
      } else {
        Globals::lineRadii.push_back(r);
        Globals::lineAngles.push_back(camAngle);
      }

      // Add text offsets
      if(lines[i].HasMember("offset")){
        if (lines[i]["offset"].HasMember("x")) {
          Globals::lineXs.push_back((float)lines[i]["offset"]["x"].GetDouble());
        } else{
          Globals::lineXs.push_back(0.0f);
        }

        if (lines[i]["offset"].HasMember("y")) {
          Globals::lineYs.push_back((float)lines[i]["offset"]["y"].GetDouble());
        } else{
          Globals::lineYs.push_back(0.0f);
        }
        if (lines[i]["offset"].HasMember("z")) {
          Globals::lineZs.push_back((float)lines[i]["offset"]["z"].GetDouble());
        } else{
          Globals::lineZs.push_back(0.0f);
        }

      } else {
        Globals::lineXs.push_back(0.0f);
        Globals::lineYs.push_back(0.0f);
        Globals::lineZs.push_back(0.0f);
      }

      // add line times and callbacks
      t = (float)lines[i]["time"].GetDouble() + t;
      lineTimes.push_back(t);
      Globals::mediator->registerSingleCallback((int)(lineTimes[i]*THE_SRATE*2), &nextLine);
      Globals::mediator->registerSingleCallback((int)((lineTimes[i]-1)*THE_SRATE*2), &fadeOut);
    }


    // After We Read The Data, It's A Good Idea To Check If Everything Read Fine
    if (ferror( pFile ))
    {
        cerr << "Failed To Get Data!" << endl;
    }

    // Close The File
    fclose(pFile);
}

//-----------------------------------------------------------------------------
// Name: LoadWavFile( )
// Desc: load Wav file
//-----------------------------------------------------------------------------
void LoadWavFile(const char * strName) {
  // go
  if( !Globals::sndfile.read( strName ) )
    exit( 1 );

  cerr << Globals::sndfile.getSize() << endl;
  Globals::sukothai = new Sukothai(Globals::sndfile.getAudio(),10,Globals::sndfile.getSize());
}



//-----------------------------------------------------------------------------
// Name: help( )
// Desc: print usage
//-----------------------------------------------------------------------------
void help()
{
   cerr << "----------------------------------------------------" << endl;
   cerr << "Hill (v1.0)" << endl;
   cerr << "Matt Horton" << endl;
   cerr << "http://ccrma.stanford.edu/~mattah/256a/Hill/" << endl;
   cerr << "----------------------------------------------------" << endl;
   cerr << "Place 'poem.json' and audio file in 'Data' directory." << endl;
   cerr << "Press 'space' to start poem." << endl;
   cerr << "See website for poem schema." << endl;
   cerr << "----------------------------------------------------" << endl;
}



//-----------------------------------------------------------------------------
// name: main()
// desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    // init audio
    audio_init(THE_SRATE, FRAMESIZE, NUMCHANNELS);

    // initialize GLUT
    glutInit( &argc, argv );

    // init gfx
    initGfx();

    // instantiate simulation
    Globals::sim = new BKSim();

    // Draw sequencer at start
    drawTerrain();

    const char * wav = Globals::parser->track;
    LoadWavFile(wav);

    // Start Audio
    audio_start();

    // print help
    help();

    // let GLUT handle the current thread from here
    glutMainLoop();
}


//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void initGfx()
{
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( Globals::windowWidth, Globals::windowHeight );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "Hill" );

    char * file = "Data/mountains.raw";
    char * poem = "Data/poem.json";

    //Read our .RAW file, store it in g_HeightMap
    LoadRawFile(file, MAP_SIZE * MAP_SIZE, g_HeightMap);
    LoadJSONFile(poem);

    cam.updateSet(
      Vector3D(
        cam.slewX().value,
        Height(g_HeightMap, cam.actual().x, cam.actual().z) + 250,
        cam.slewZ().value ));

    // set the idle function - called when idle
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the special keyboard function - called on special keyboard events
    glutSpecialFunc( specialFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );

    // set clear color
    // set the GL clear color - use when the color buffer is cleared
    glClearColor(
      Globals::bgColor.actual().x,
      Globals::bgColor.actual().y,
      Globals::bgColor.actual().z,
      1.0f );
    // enable color material
    glEnable( GL_COLOR_MATERIAL );
    // enable depth test
    glEnable( GL_DEPTH_TEST );

}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    Globals::windowWidth = w; Globals::windowHeight = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    //ratioz = (GLfloat)w/(GLfloat)h;
    gluPerspective(persp, (GLfloat)w/(GLfloat)h, 0.5f, 5000.0f);
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
    // position the view point
    gluLookAt(
      cam.actual().x,cam.actual().y,cam.actual().z,
      0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f );
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    float speed = 8.0f;     // Movement speed
    bool skip = false;

    switch( key )
    {
        case ' ': // space
            if(!Globals::started) nextLine();
            Globals::started = true;
            break;
        case 27: // escape
            exit(1);
            break;
        case 13: // enter
            for (int i = 0; i < Globals::waves.size(); i++) {
              if (Globals::waves[i] == -1) {
                Globals::waves[i] = 256;
                break;
              }
            }
            skip = true;
            nextLine();
            break;
        case '~':
        case '`': // toggle fullscreen
          // check fullscreen
          if( !Globals::fullscreen )
          {
              Globals::lastWindowWidth = Globals::windowWidth;
              Globals::lastWindowHeight = Globals::windowHeight;
              glutFullScreen();
          }
          else
              glutReshapeWindow( Globals::lastWindowWidth, Globals::lastWindowHeight );

          // toggle variable value
          Globals::fullscreen = !Globals::fullscreen;
          break;

    }

    glutPostRedisplay( );
}

//-----------------------------------------------------------------------------
// Name: specialFunc( )
// Desc: special key event
//-----------------------------------------------------------------------------
void specialFunc(int key, int xx, int yy) {

    float fraction = 1.0f;
    float speed = 0.1f;
    float radspeed = 5.0f;
    Vector3D upd8;

    switch (key) {
      case GLUT_KEY_LEFT :
        camAngle -= speed;
        break;
      case GLUT_KEY_RIGHT :
        camAngle += speed;
        break;
      case GLUT_KEY_UP :
        r += radspeed;
        break;
      case GLUT_KEY_DOWN :
        r -= radspeed;
        break;
    }
    cerr << r << endl;
    upd8 = Vector3D(r*sin(camAngle), cam.slewY().goal, r*cos(camAngle));
    cam.update(upd8,.5);

}


//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // when left mouse button is down
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        // when right mouse button down
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else
    {
    }

    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}



//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{

    Globals::bgColor.interp();

    glClearColor(
      Globals::bgColor.actual().x,
      Globals::bgColor.actual().y,
      Globals::bgColor.actual().z,
      1.0f );
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // load the identity matrix
    glLoadIdentity( );

    cam.interp();
    updateMix(r);

    persp = 75.0f - ((r-400.0f)/400.0f)*30.0f;
    gluPerspective(persp, Globals::lastWindowWidth/Globals::lastWindowHeight, 0.5f, 5000.0f);

    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );

    // load the identity matrix
    glLoadIdentity( );

    // position the view point
    // Need to work on the point to vector
    gluLookAt( cam.actual().x,cam.actual().y,cam.actual().z, 0, 0.0f, 0, 0.0f, 1.0f, 0.0f );



    // cascade simulation
    Globals::sim->systemCascade();

    // flush!
    glFlush( );
    // swap the double buffer
    glutSwapBuffers( );
}


//-----------------------------------------------------------------------------
// Name: drawTerrain()
// Desc: Draw terrain
//-----------------------------------------------------------------------------
void drawTerrain() {
  YEntity * entity = new YEntity();
  YTerrain * ter = new YTerrain(g_HeightMap, true, false);
  //YTerrain * terLine = new YTerrain(g_HeightMap, false, true);
  YText * text = new YText(0.0f);

  text->set("");
  text->iLoc.updateSet(Vector3D(0,100,0));
  text->iRGB.updateSet(Vector3D(1,1,1));

  ter->sca.set(0.1,0.1,0.1);
  //terLine->loc.set(0,1,0);
  //terLine->sca.set(0.1,0.1,0.1);
  entity->sca.set(1.5,1.2,1.2);

  Globals::sim->root().addChild(entity);

  entity->addChild(ter);
  //entity->addChild(terLine);
  Globals::sim->root().addChild(text);
  Globals::terrain = entity;
  Globals::ter = ter;
  Globals::text = text;
}


//-----------------------------------------------------------------------------
// Name: Height()
// Desc: Get terrain height
//-----------------------------------------------------------------------------
int Height(uint8_t *pHeightMap, float x, float z)			// This Returns The Height From A Height Map Index
{
    // Translate the world coordinates to array indices
    int col = int(x / STEP_SIZE) % MAP_SIZE;
    int row = int(z / STEP_SIZE) % MAP_SIZE;

    return pHeightMap[(row * MAP_SIZE) + col];			// Index Into Our Height Array And Return The Height
}

//-----------------------------------------------------------------------------
// Name: nextLine()
// Desc: initiate the next line of the poem
//-----------------------------------------------------------------------------
void nextLine() {
  float playedRatio = (float)Globals::sndfile.playhead()/(float)Globals::sndfile.getSize();

  // Update cam based on score
  camAngle = Globals::lineAngles.at(Globals::currentLine);
  r = Globals::lineRadii.at(Globals::currentLine);
  Vector3D upd8 = Vector3D(r*sin(camAngle), cam.slewY().goal, r*cos(camAngle));
  cam.update(upd8,.1);

  Globals::text->fade(0.0f,0); // Finish fade immediately if it's not done
  Globals::text->ori.set(0, camAngle*180/3.14159f, 0);
  iSlew3D loc = Globals::text->iLoc;

  Globals::text->iLoc.updateSet(
    Vector3D(
      (r-500)*sin(camAngle),
      cam.actual().y-100,
      (r-500)*cos(camAngle)
    )
  );

  Globals::text->iLoc.update(
    Vector3D(
      (r-500)*sin(camAngle)+Globals::lineXs.at(Globals::currentLine),
      cam.actual().y-100+Globals::lineYs.at(Globals::currentLine),
      (r-500)*cos(camAngle)+Globals::lineZs.at(Globals::currentLine)
    ), .5
  );

  if (Globals::currentLine < Globals::lineStrings.size()){

    // Set new text and fade in
    Globals::text->set(Globals::lineStrings.at(Globals::currentLine));
    Globals::text->fade(1.0f,.8);
  }
  Globals::currentLine++;
}

//-----------------------------------------------------------------------------
// Name: updateMix()
// Desc: update the effect mix based on radius
//-----------------------------------------------------------------------------
void updateMix(float radius) {
  if(radius < 400.0f) {
    Globals::mix = 1.0f;
  } else if (radius > 800.0f){
    Globals::mix = 10.0f;
  } else {
    Globals::mix = 1.0f + (radius-400.0f)/(400.0f/9);
  }
}

void fadeOut(){
  Globals::text->fade(0.0f,iSlew3D::slewForDuration(1)+.5);
}
