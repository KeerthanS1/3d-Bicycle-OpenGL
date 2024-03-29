/************************************** 
* Beginning of the header file "cycle.h" 
***************************************/ 
#include<GL/glut.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<math.h> 

#define   PI            3.14159 
#define WIN_WIDTH      600 
#define WIN_HEIGHT      600    
#define CYCLE_LENGTH   3.3f 
#define ROD_RADIUS      0.05f 
#define NUM_SPOKES      20 
#define SPOKE_ANGLE      18 
#define RADIUS_WHEEL   1.0f 
#define TUBE_WIDTH      0.08f 
#define RIGHT_ROD      1.6f 
#define RIGHT_ANGLE      48.0f 
#define MIDDLE_ROD      1.7f 
#define MIDDLE_ANGLE   106.0f 
#define BACK_CONNECTOR   0.5f 
#define LEFT_ANGLE      50.0f 
#define WHEEL_OFFSET   0.11f 
#define WHEEL_LEN      1.1f 
#define TOP_LEN         1.5f 
#define CRANK_ROD      0.7f 
#define CRANK_RODS      1.12f 
#define CRANK_ANGLE      8.0f 
#define HANDLE_ROD      1.2f 
#define FRONT_INCLINE   70.0f 
#define HANDLE_LIMIT   70.0f 

#define INC_STEERING   2.0f 
#define INC_SPEED      0.05f 

/***************************************** 
*    All the Global Variables are Here 
****************************************/ 
/***************************************** 
*   Cycle - related variables 
******************************************/    
GLfloat pedalAngle, speed, steering; 

/******************************* 
*   User view realted variables 
********************************/ 
GLfloat camx,camy,camz; 
GLfloat anglex,angley,anglez; 

/**************************** 
*   Mouse related variables 
****************************/ 
int prevx,prevy; 
GLenum Mouse; 

/************************** 
*   Cycle position related 
*   variables 
***************************/ 
GLfloat xpos,zpos,direction; 

void ZCylinder(GLfloat radius,GLfloat length); 
void XCylinder(GLfloat radius,GLfloat length); 

void drawFrame(void); 
void gear( GLfloat inner_radius, GLfloat outer_radius, 
        GLfloat width,GLint teeth, GLfloat tooth_depth ); 
void drawChain(void); 
void drawPedals(void); 
void drawTyre(void); 
void drawSeat(void); 
void help(void); 
void init(void); 
void reset(void); 
void display(void); 
void idle(void); 
void updateScene(void); 
void landmarks(void);    
void special(int key,int x,int y); 
void keyboard(unsigned char key,int x,int y); 
void mouse(int button,int state,int x,int y); 
void motion(int x,int y); 
void reshape(int w,int h); 
void glSetupFuncs(void); 
GLfloat Abs(GLfloat); 
GLfloat degrees(GLfloat); 
GLfloat radians(GLfloat); 
GLfloat angleSum(GLfloat, GLfloat); 

       

void ZCylinder(GLfloat radius,GLfloat length) 
{ 
   GLUquadricObj *cylinder; 
   cylinder=gluNewQuadric(); 
   glPushMatrix(); 
      glTranslatef(0.0f,0.0f,0.0f); 
      gluCylinder(cylinder,radius,radius,length,15,5); 
   glPopMatrix(); 
} 

void XCylinder(GLfloat radius,GLfloat length) 
{ 
   glPushMatrix(); 
      glRotatef(90.0f,0.0f,1.0f,0.0f); 
      ZCylinder(radius,length); 
   glPopMatrix(); 
} 

 
void updateScene() 
{ 
   GLfloat xDelta, zDelta; 
   GLfloat rotation; 
   GLfloat sin_steering, cos_steering; 

   // if the tricycle is not moving then do nothing 
   if (-INC_SPEED < speed && speed < INC_SPEED) return; 

   if(speed < 0.0f) 
         pedalAngle = speed = 0.0f; 
    
  

   xDelta = speed*cos(radians(direction + steering)); 
   zDelta = speed*sin(radians(direction + steering)); 
   xpos += xDelta; 
   zpos -= zDelta; 
   pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL)); 

 
   sin_steering = sin(radians(steering)); 
   cos_steering = cos(radians(steering)); 

 
   rotation = atan2(speed * sin_steering, CYCLE_LENGTH + speed * cos_steering); 
   direction = degrees(angleSum(radians(direction),rotation)); 
} 


GLfloat angleSum(GLfloat a, GLfloat b) 
{ 
  a += b; 
  if (a < 0) return a+2*PI; 
  else if (a > 2*PI) return a-2*PI; 
  else return a; 
} 

/************************************************ 
*   Draw the metal frame of the cycle and also 
*   draw the seat and the back wheel with 
*   this. 
*   All these parts are always together in the 
*   same plane.They never move out ot the 
*   PLANE!   ;) 
************************************************/ 
void drawFrame() 
{ 
   glColor3f(1.0f,0.0f,0.0f); 

   /******************************** 
   *   First draw the all the items 
   *   at the center of the frame. 
   *   Draw the bigger gear,the small 
   *   cylinder acting as the socket 
   *   for the pedals.Also DON'T 
   *   forget to draw the two 
   *   connecting cemtral rods 
   *********************************/ 
   glPushMatrix(); 
      /****************************** 
      *   Allow me to draw the BIGGER 
      *   gear and the socket cylinder 
      *******************************/ 
      glPushMatrix(); 
         /*************************** 
         *   Let the gear have the 
         *   green color 
         ***************************/ 
         glColor3f(0.0f,1.0f,0.0f); 
          
         /************************** 
         *   The gear should be 
         *   outside the frame !!! 
         *   This is the bigger 
         *   GEAR 
         ***************************/ 
         glPushMatrix(); 
            glTranslatef(0.0f,0.0f,0.06f); 
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f); 
            gear(0.08f,0.3f,0.03f,30,0.03f); 
         glPopMatrix(); 
         /*************************** 
         *   Restore the color of the 
         *   frame 
         ****************************/ 
         glColor3f(1.0f,1.0f,0.0f); 
         glTranslatef(0.0f,0.0f,-0.2f); 
         ZCylinder(0.08f,0.32f); 
      glPopMatrix(); 
      /***************************** 
      *   Lets first draw the 
      *   rightmost rod of the frame 
      *******************************/ 
      glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f); 
      XCylinder(ROD_RADIUS,RIGHT_ROD); 
       
      /******************************* 
      *   Now draw the centre rod of 
      *   the frame which also supports 
      *   the seat 
      *********************************/ 
      glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f); 
      XCylinder(ROD_RADIUS,MIDDLE_ROD); 
      /******************************** 
      *   We have drawn the support.So 
      *   let's draw the seat with a 
      *   new color 
      *********************************/ 
      glColor3f(1.0f,0.0f,0.0f); 
      glTranslatef(MIDDLE_ROD,0.0f,0.0f); 
      glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f); 
      glScalef(0.3f,ROD_RADIUS,0.25f); 
      drawSeat(); 
      /********************** 
      *   Restore the color ! 
      ************************/ 
      glColor3f(1.0f,0.0f,0.0f); 
   glPopMatrix(); 
   /********************************* 
   *   Draw the horizontal part of 
   *   the frame. 
   *********************************/ 
    
   /********************************* 
   *   Draw the main single rod 
   *   connecting the center of the 
   *   frame to the back wheel of the 
   *   cycle 
   **********************************/ 
   glPushMatrix(); 
      glRotatef(-180.0f,0.0f,1.0f,0.0f); 
      XCylinder(ROD_RADIUS,BACK_CONNECTOR); 
    
      /*********************************** 
      *   Draw the two rods on the either 
      *   side of the wheel 
      *   These rods are part of the 
      *   horizontal part of the cycle 
      ************************************/ 
      glPushMatrix(); 
         glTranslatef(0.5f,0.0f,WHEEL_OFFSET); 
         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH); 
      glPopMatrix(); 
      glPushMatrix(); 
         glTranslatef(0.5f,0.0f,-WHEEL_OFFSET); 
         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH); 
      glPopMatrix(); 
   glPopMatrix(); 
    
   /************************************ 
   *   Draw the leftmost rods of the 
   *   frame of the cycle 
   *************************************/ 
   glPushMatrix(); 
      glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f); 
         /******************************** 
      *   Transalted to the back wheel 
      *   position.Why not draw the back 
      *   wheel and also the gear ? :)) 
      **********************************/ 
      glPushMatrix(); 
         glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f); 
         drawTyre(); 
         glColor3f(0.0f,1.0f,0.0f); 
            gear(0.03f,0.15f,0.03f,20,0.03f); 
         glColor3f(1.0f,0.0f,0.0f); 
      glPopMatrix(); 
      glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f); 
          
      /************************************ 
      *   Draw the two rods on the either 
      *   side of the wheel connecting the 
      *   backwheel and topmost horizontal 
      *   part of the wheel 
      *************************************/ 
      glPushMatrix(); 
         glTranslatef(0.0f,0.0f,-WHEEL_OFFSET); 
         XCylinder(ROD_RADIUS,WHEEL_LEN); 
      glPopMatrix(); 
      glPushMatrix(); 
         glTranslatef(0.0f,0.0f,WHEEL_OFFSET); 
         XCylinder(ROD_RADIUS,WHEEL_LEN); 
      glPopMatrix(); 
       
      /***************************** 
      *   Draw the single rod of the 
      *   same setup 
      ******************************/ 
      glTranslatef(WHEEL_LEN,0.0f,0.0f); 
      XCylinder(ROD_RADIUS,CRANK_ROD); 
    
      /***************************** 
      *   Now Draw The topmost 
      *   Horizontal rod 
      *****************************/ 

      glTranslatef(CRANK_ROD,0.0f,0.0f); 
      glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f); 
      XCylinder(ROD_RADIUS,TOP_LEN); 
       
      /******************************* 
      *   Now instead of again traversing 
      *   all the way back and again 
      *   forward.WHY NOT DRAW THE 
      *   HANDLE FROM HERE ITSELF? 
      ********************************/ 
      /***************************** 
      *   Now draw the handle and 
      *   small support rod which 
      *   is incorporated in the 
      *   frame itself. 
      *   Set y-axis at te required 
      *   incline. 
      ******************************/ 
      glTranslatef(TOP_LEN,0.0f,0.0f); 
      glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f); 
       
      /****************************** 
      *   Draw the small rod that acts 
      *   as the socket joining the 
      *   frame and the handle 
      ******************************/ 
      glPushMatrix(); 
         glTranslatef(-0.1f,0.0f,0.0f); 
         XCylinder(ROD_RADIUS,0.45f); 
      glPopMatrix(); 
       
      /****************************** 
      *   I Hope the handle can rotate 
      *   about its mean position 
      *******************************/ 
      glPushMatrix(); 
         glRotatef(-steering,1.0f,0.0f,0.0f); 
         /****************************** 
         *   Roll back to the height of 
         *   the handle to draw it 
         *******************************/ 
         glTranslatef(-0.3f,0.0f,0.0f); 

         /******************************** 
         *   We cannot use the incline    
         *   the incline to draw the 
         *   horizontal part of the rod 
         ********************************/ 
         glPushMatrix(); 
            glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f); 

            glPushMatrix(); 
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2); 
               ZCylinder(ROD_RADIUS,HANDLE_ROD); 
            glPopMatrix(); 

            glPushMatrix(); 
               glColor3f(1.0f,0.0f,0.0f); 
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2); 
               ZCylinder(0.07f,HANDLE_ROD/4); 
               glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4); 
               ZCylinder(0.07f,HANDLE_ROD/4); 
               glColor3f(1.0f,0.0f,0.0f); 
            glPopMatrix(); 
         glPopMatrix(); 

         /********************************* 
         *   Using this incline now draw 
         *   the handle.Maybe use this again 
         *   to draw the wheel. ;) 
         **********************************/ 
         glPushMatrix(); 
            /**************************** 
            *   Draw the main big rod 
            ****************************/ 
            XCylinder(ROD_RADIUS,CRANK_ROD); 

            /****************************** 
            *   Why not draw the two rods and 
            *   the WHEEL?   :) 
            *   Yes!So,first go to the 
            *   end of the main rod. 
            *******************************/ 
            glTranslatef(CRANK_ROD,0.0f,0.0f); 
            glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f); 

            /******************************* 
            *   Draw the two rods connecting 
            *   the handle and the front 
            *   wheel. 
            *   The two rods are at a incline 
            *   to the connector. 
            ********************************/ 
            glPushMatrix(); 
               glTranslatef(0.0f,0.0f,WHEEL_OFFSET); 
               XCylinder(ROD_RADIUS,CRANK_RODS); 
            glPopMatrix(); 
            glPushMatrix(); 
               glTranslatef(0.0f,0.0f,-WHEEL_OFFSET); 
               XCylinder(ROD_RADIUS,CRANK_RODS); 
            glPopMatrix(); 
               /******************************** 
            *   Why not draw the wheel. 
            *   The FRONT wheel to be precise 
            *********************************/ 
            glTranslatef(CRANK_RODS,0.0f,0.0f); 
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f); 
            drawTyre(); 
         glPopMatrix(); 
      glPopMatrix();   /*   End of the rotation of the handle effect   */ 
   glPopMatrix(); 
} 

// Portions of this code have been borrowed from Brian Paul's Mesa 
// distribution. 
/* 
* Draw a gear wheel.  You'll probably want to call this function when 
* building a display list since we do a lot of trig here. 
* 
* Input:  inner_radius - radius of hole at center 
*      outer_radius - radius at center of teeth 
*      width - width of gear 
*      teeth - number of teeth 
*      tooth_depth - depth of tooth 
*/ 

void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width, 
        GLint teeth, GLfloat tooth_depth ) 
{ 
    GLint i; 
    GLfloat r0, r1, r2; 
    GLfloat angle, da; 
    GLfloat u, v, len; 
    const double pi = 3.14159264; 

    r0 = inner_radius; 
    r1 = outer_radius - tooth_depth/2.0; 
    r2 = outer_radius + tooth_depth/2.0; 

    da = 2.0*pi / teeth / 4.0; 

    glShadeModel( GL_FLAT ); 

    glNormal3f( 0.0, 0.0, 1.0 ); 

    /* draw front face */ 
    glBegin( GL_QUAD_STRIP ); 
    for (i=0;i<=teeth;i++) { 
   angle = i * 2.0*pi / teeth; 
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 ); 
   glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 ); 
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 ); 
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 ); 
    } 
    glEnd(); 

    /* draw front sides of teeth */ 
    glBegin( GL_QUADS ); 
    da = 2.0*pi / teeth / 4.0; 
    for (i=0;i<teeth;i++) { 
   angle = i * 2.0*pi / teeth; 

   glVertex3f( r1*cos(angle),      r1*sin(angle),     width*0.5 ); 
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     width*0.5 ); 
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 ); 
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 ); 
    } 
    glEnd(); 


    glNormal3f( 0.0, 0.0, -1.0 ); 

    /* draw back face */ 
    glBegin( GL_QUAD_STRIP ); 
    for (i=0;i<=teeth;i++) { 
   angle = i * 2.0*pi / teeth; 
   glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 ); 
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 ); 
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 ); 
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 ); 
    } 
    glEnd(); 

    /* draw back sides of teeth */ 
    glBegin( GL_QUADS ); 
    da = 2.0*pi / teeth / 4.0; 
    for (i=0;i<teeth;i++) { 
   angle = i * 2.0*pi / teeth; 

   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 ); 
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 ); 
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 ); 
   glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 ); 
    } 
    glEnd(); 


    /* draw outward faces of teeth */ 
    glBegin( GL_QUAD_STRIP ); 
    for (i=0;i<teeth;i++) { 
   angle = i * 2.0*pi / teeth; 

   glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 ); 
   glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 ); 
   u = r2*cos(angle+da) - r1*cos(angle); 
   v = r2*sin(angle+da) - r1*sin(angle); 
   len = sqrt( u*u + v*v ); 
   u /= len; 
   v /= len; 
   glNormal3f( v, -u, 0.0 ); 
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),      width*0.5 ); 
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 ); 
   glNormal3f( cos(angle), sin(angle), 0.0 ); 
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 ); 
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 ); 
   u = r1*cos(angle+3*da) - r2*cos(angle+2*da); 
   v = r1*sin(angle+3*da) - r2*sin(angle+2*da); 
   glNormal3f( v, -u, 0.0 ); 
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 ); 
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 ); 
   glNormal3f( cos(angle), sin(angle), 0.0 ); 
    } 

    glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 ); 
    glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 ); 

    glEnd(); 


    glShadeModel( GL_SMOOTH ); 

    /* draw inside radius cylinder */ 
    glBegin( GL_QUAD_STRIP ); 
    for (i=0;i<=teeth;i++) { 
   angle = i * 2.0*pi / teeth; 
   glNormal3f( -cos(angle), -sin(angle), 0.0 ); 
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 ); 
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 ); 
    } 
    glEnd(); 

} 

/****************************************** 
*   Could not model the exact chain 
*   Think it eats up a lot of power if 
*   approximated by a lot of spheres 
*   So approximated with the stippled 
*   lines instead 
******************************************/ 
void drawChain() 
{ 
   GLfloat depth; 
   static int mode=0; 

   glColor3f(0.0f,1.0f,0.0f); 
   glEnable(GL_LINE_STIPPLE); 
   mode=(mode+1)%2; 
    
   if(mode==0 && speed>0) 
      glLineStipple(1,0x1c47); 
   else if(mode==1 && speed>0) 
      glLineStipple(1,0x00FF); 
    
   glBegin(GL_LINES); 
   for(depth=0.06f;depth<=0.12f;depth+=0.01f) 
   { 
      glVertex3f(-1.6f,0.15f,ROD_RADIUS); 
      glVertex3f(0.0f,0.3f,depth); 
       
      glVertex3f(-1.6f,-0.15f,ROD_RADIUS); 
      glVertex3f(0.0f,-0.3f,depth); 
   } 
   glEnd(); 
   glDisable(GL_LINE_STIPPLE); 
} 

void drawSeat() 
{ 
   /********************************* 
   *   Draw the top of the seat 
   **********************************/ 
   glBegin(GL_POLYGON); 
      glVertex3f(-0.1f, 1.0f, -0.5f); 
      glVertex3f(   1.0f, 1.0f, -0.3f); 
      glVertex3f( 1.0f, 1.0f,  0.3f); 
      glVertex3f(-0.1f, 1.0f,  0.5f); 
      glVertex3f(-0.5f, 1.0f,  1.0f); 
      glVertex3f(-1.0f, 1.0f,  1.0f); 
      glVertex3f(-1.0f, 1.0f, -1.0f); 
      glVertex3f(-0.5f, 1.0f, -1.0f); 
   glEnd(); 

   /********************************** 
   *   Draw the bottom base part of the 
   *   seat 
   ************************************/ 
   glBegin(GL_POLYGON); 
      glVertex3f(-0.1f, -1.0f, -0.5f); 
      glVertex3f(   1.0f, -1.0f, -0.3f); 
      glVertex3f( 1.0f, -1.0f,  0.3f); 
      glVertex3f(-0.1f, -1.0f,  0.5f); 
      glVertex3f(-0.5f, -1.0f,  1.0f); 
      glVertex3f(-1.0f, -1.0f,  1.0f); 
      glVertex3f(-1.0f, -1.0f, -1.0f); 
      glVertex3f(-0.5f, -1.0f, -1.0f); 
   glEnd(); 

   /********************** 
   *   Draw the sides! 
   ***********************/ 
   glBegin(GL_QUADS); 
      glVertex3f(1.0f,1.0f,-0.3f); 
      glVertex3f(1.0f,1.0f,0.3f); 
      glVertex3f(1.0f,-1.0f,0.3f); 
      glVertex3f(1.0f,-1.0f,-0.3f); 

      glVertex3f(1.0f,1.0f,0.3f); 
      glVertex3f(-0.1f,1.0f,0.5f); 
      glVertex3f(-0.1f,-1.0f,0.5f); 
      glVertex3f(1.0f,-1.0f,0.3f); 

      glVertex3f(1.0f,1.0f,-0.3f); 
      glVertex3f(-0.1f,1.0f,-0.5f); 
      glVertex3f(-0.1f,-1.0f,-0.5f); 
      glVertex3f(1.0f,-1.0f,-0.3f); 

      glVertex3f(-0.1f,1.0f,0.5f); 
      glVertex3f(-0.5f,1.0f,1.0f); 
      glVertex3f(-0.5f,-1.0f,1.0f); 
      glVertex3f(-0.1f,-1.0f,0.5f); 

      glVertex3f(-0.1f,1.0f,-0.5f); 
      glVertex3f(-0.5f,1.0f,-1.0f); 
      glVertex3f(-0.5f,-1.0f,-1.0f); 
      glVertex3f(-0.1f,-1.0f,-0.5f); 

      glVertex3f(-0.5f,1.0f,1.0f); 
      glVertex3f(-1.0f,1.0f,1.0f); 
      glVertex3f(-1.0f,-1.0f,1.0f); 
      glVertex3f(-0.5f,-1.0f,1.0f); 

      glVertex3f(-0.5f,1.0f,-1.0f); 
      glVertex3f(-1.0f,1.0f,-1.0f); 
      glVertex3f(-1.0f,-1.0f,-1.0f); 
      glVertex3f(-0.5f,-1.0f,-1.0f); 

      glVertex3f(-1.0f,1.0f,1.0f); 
      glVertex3f(-1.0f,1.0f,-1.0f); 
      glVertex3f(-1.0f,-1.0f,-1.0f); 
      glVertex3f(-1.0f,-1.0f,1.0f); 

   glEnd(); 


} 

void drawPedals() 
{ 
   glColor3f(1.0f,1.0f,0.0f); 
   /*************************** 
   *   Lets draw the two pedals 
   *   offset from the center 
   *   of the frame. 
   ****************************/ 

   /***************************** 
   *   First draw the one visible 
   *   to the viewer 
   ******************************/ 
   glPushMatrix(); 
      glTranslatef(0.0f,0.0f,0.105f); 
      glRotatef(-pedalAngle,0.0f,0.0f,1.0f); 
      glTranslatef(0.25f,0.0f,0.0f); 
      /************************* 
      *   Draw the pedal rod 
      *************************/ 
      glPushMatrix(); 
         glScalef(0.5f,0.1f,0.1f); 
         glutSolidCube(1.0f); 
      glPopMatrix(); 

      /************************ 
      *   Draw the resting pad 
      ************************/ 
      glPushMatrix(); 
         glTranslatef(0.25f,0.0f,0.15f); 
         glRotatef(pedalAngle,0.0f,0.0f,1.0f); 
         glScalef(0.2f,0.02f,0.3f); 
         glutSolidCube(1.0f); 
      glPopMatrix(); 

   glPopMatrix(); 

   /******************************* 
   *   Draw the one on the other 
   *   side  of the frame 
   *******************************/ 
   glPushMatrix(); 
      glTranslatef(0.0f,0.0f,-0.105f); 
      glRotatef(180.0f-pedalAngle,0.0f,0.0f,1.0f); 
      glTranslatef(0.25f,0.0f,0.0f); 

      /*************************** 
      *   Now again draw the pedal 
      *   rod 
      ****************************/ 
      glPushMatrix(); 
         glScalef(0.5f,0.1f,0.1f); 
         glutSolidCube(1.0f); 
      glPopMatrix(); 

      /**************************** 
      *   Draw the resting pad of 
      *   the pedal 
      *****************************/ 
      glPushMatrix(); 
         glTranslatef(0.25f,0.0f,-0.15f); 
         glRotatef(pedalAngle-180.0f,0.0f,0.0f,1.0f); 
         glScalef(0.2f,0.02f,0.3f); 
         glutSolidCube(1.0f); 
      glPopMatrix(); 

   glPopMatrix(); 

   glColor3f(1.0f,0.0f,0.0f); 
} 

void drawTyre(void) 
{ 
   int i; 
   //   Draw The Rim 
   glColor3f(1.0f,1.0f,1.0f); 
   glutSolidTorus(0.06f,0.92f,4,30); 
   //   Draw The Central Cylinder 
   //   Length of cylinder  0.12f 
   glColor3f(1.0f,1.0f,0.5f); 
   glPushMatrix(); 
      glTranslatef(0.0f,0.0f,-0.06f); 
      ZCylinder(0.02f,0.12f); 
   glPopMatrix(); 
   glutSolidTorus(0.02f,0.02f,3,20); 

   //   Draw The Spokes 
   glColor3f(0.0f,0.0f,1.0f); 
   for(i=0;i<NUM_SPOKES;++i) 
   { 
      glPushMatrix(); 
         glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f); 
         glBegin(GL_LINES); 
            glVertex3f(0.0f,0.02f,0.0f); 
            glVertex3f(0.0f,0.86f,0.0f); 
         glEnd(); 
      glPopMatrix(); 
   } 

   //   Draw The Tyre 
   glColor3f(0.0f,0.0f,0.0f); 
   glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30); 
   glColor3f(1.0f,0.0f,0.0f); 
} 

void init() 
{ 
   GLfloat mat_specular[]={1.0,1.0,1.0,1.0}; 
   GLfloat mat_shininess[]={100.0}; 
   GLfloat light_directional[]={1.0,1.0,1.0,1.0}; 
   GLfloat light_positional[]={1.0,1.0,1.0,0.0}; 
   GLfloat light_diffuse[]={1.0,1.0,1.0}; 

   reset(); 

   glShadeModel(GL_SMOOTH);    

   glLightfv(GL_LIGHT0,GL_POSITION,light_directional); 
   glLightfv(GL_LIGHT0,GL_AMBIENT,light_diffuse); 
   glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse); 
   glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess); 
   glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular); 
   glColorMaterial(GL_FRONT,GL_DIFFUSE); 

   glEnable(GL_LIGHTING); 
   glEnable(GL_LIGHT0); 
   glEnable(GL_COLOR_MATERIAL); 
   glEnable(GL_DEPTH_TEST); 
} 
    
void landmarks(void) 
{ 
      GLfloat i; 
      glColor3f(1.0f,0.0f,.0f); 

      /************************************ 
      *   Draw the ground for the cycle 
      *   Looks incomplete with it!Don't 
      *   forget to define the normal 
      *   vectors for the vertices. 
      *   gotta fix this bug! 
      ************************************/ 
      glBegin(GL_LINES); 
      for(i=-100.0f ; i<100.0f ; i += 1.0f) 
      { 
         glVertex3f(-100.0f,-RADIUS_WHEEL,i); 
         glVertex3f( 100.0f,-RADIUS_WHEEL,i); 
         glVertex3f(i,-RADIUS_WHEEL,-100.0f); 
         glVertex3f(i,-RADIUS_WHEEL,100.0f); 
      } 
      glEnd(); 
} 

void display(void) 
{ 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   glEnable(GL_NORMALIZE); 

   glPushMatrix();    
      /******************************* 
       *    Prepare the rotations 
       *    and start doing the 
       *    remaining scene 
       *******************************/ 
      glRotatef(angley,1.0f,0.0f,0.0f); 
      glRotatef(anglex,0.0f,1.0f,0.0f); 
      glRotatef(anglez,0.0f,0.0f,0.0f); 

      /*********************** 
       *    Start rendering    
       *    the scene; 
       *    the bicycle ;) 
       **********************/     
    
      landmarks(); 

      /**************************** 
      *   Move the cycle. 
      ****************************/ 
      glPushMatrix(); 
         glTranslatef(xpos,0.0f,zpos); 
         glRotatef(direction,0.0f,1.0f,0.0f); 

         drawFrame();    
         drawChain(); 
         drawPedals(); 
      glPopMatrix(); 

   glPopMatrix(); 

   glMatrixMode(GL_MODELVIEW); 
   glLoadIdentity(); 
   gluLookAt(camx,camy,camz,  camx,0.0,0.0,  0.0,1.0,0.0); 

   glutSwapBuffers(); 
} 

/************************ 
*   Returns the absolute 
*   value of a given 
*   float number 
************************/ 
GLfloat Abs(GLfloat a) 
{ 
   if(a < 0.0f) 
      return -a; 
   else 
      return a; 
} 

/************************ 
*   Returns the value of 
*   the given angle in 
*   degrees 
************************/ 
GLfloat degrees(GLfloat a) 
{ 
   return a*180.0f/PI; 
} 

/************************ 
*   Returns the value of 
*   the given angle in 
*   radians 
************************/ 
GLfloat radians(GLfloat a) 
{ 
   return a*PI/180.0f; 
} 

/************************* 
*   The idle function of 
*   the program which makes 
*   the contniuous loop 
***************************/ 
void idle(void) 
{ 
   updateScene(); 
   glutPostRedisplay(); 
} 

void special(int key,int x,int y) 
{ 
   switch(key) 
   { 
      case GLUT_KEY_UP: 
         camz -= 0.1f; 
         break; 
      case GLUT_KEY_DOWN: 
         camz += 0.1f; 
         break; 
      case GLUT_KEY_LEFT: 
         camx -= 0.1f; 
         break; 
      case GLUT_KEY_RIGHT: 
         camx += 0.1f; 
         break; 
   } 
   glutPostRedisplay(); 
} 

/***************************** 
*   Reset The scene 
*****************************/ 
void reset() 
{ 
   anglex=angley=anglez=0.0f; 
   pedalAngle=steering=0.0f; 
   Mouse=GLUT_UP; 
   pedalAngle=speed=steering=0.0f; 
   camx=camy=0.0f; 
   camz=5.0f; 
   xpos=zpos=0.0f; 
   direction=0.0f; 
} 

void keyboard(unsigned char key,int x,int y) 
{ 
   GLfloat r=0.0f; 

   switch(key) 
   { 
      case 's': 
      case 'S': 
         reset(); 
         break; 
      case 'L': 
         if(steering < HANDLE_LIMIT) 
               steering += INC_STEERING; 
         break; 
      case 'R': 
         if(steering > -HANDLE_LIMIT) 
            steering -= INC_STEERING; 
         break; 
      case 'F': 
         speed += INC_SPEED; 
         break; 
      case 'B': 
         speed -= INC_SPEED; 
         break;    
      case 27: 
         exit(1); 
   } 
    

   pedalAngle += speed; 
   if(speed < 0.0f) 
      speed = 0.0f; 
   if(pedalAngle < 0.0f) 
      pedalAngle = 0.0f; 
   if(pedalAngle >= 360.0f) 
      pedalAngle -= 360.0f; 

 
   glutPostRedisplay(); 
} 

void mouse(int button,int state,int x,int y) 
{ 
   switch(button) 
   { 
      case GLUT_LEFT_BUTTON: 
         if(state==GLUT_DOWN) 
         { 
            Mouse=GLUT_DOWN; 
            prevx=x; 
            prevy=y; 
         } 
         if(state==GLUT_UP) 
         { 
            Mouse=GLUT_UP; 
         } 
         break; 
      case GLUT_RIGHT_BUTTON: 
         /*   DO NOTHING   */ 
         break; 
   } 
   glutPostRedisplay(); 
} 

void passive(int x,int y) 
{ 
/*   DO NOTHING   */ 
} 

void motion(int x,int y) 
{ 
   if(Mouse==GLUT_DOWN)    
   { 
      int deltax,deltay; 
      deltax=prevx-x; 
      deltay=prevy-y; 
      anglex += 0.5*deltax; 
      angley += 0.5*deltay; 
      if(deltax!=0 && deltay!=0) 
         anglez += 0.5*sqrt(deltax*deltax + deltay*deltay); 

      if(anglex < 0) 
         anglex+=360.0; 
      if(angley < 0) 
         angley+=360.0; 
      if(anglez < 0) 
         anglez += 360.0; 

      if(anglex > 360.0) 
         anglex-=360.0; 
      if(angley > 360.0) 
         angley-=360.0; 
      if(anglez > 360.0) 
         anglez-=360.0; 
   } 
   else 
   { 
      Mouse=GLUT_UP; 
   } 
   prevx=x; 
   prevy=y; 
   glutPostRedisplay();    
} 

void reshape(int w,int h) 
{ 
   glViewport(0,0,(GLsizei)w,(GLsizei)h); 
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity(); 
   gluPerspective(60.0,(GLfloat)w/(GLfloat)h,0.1,100.0); 
   //Angle,Aspect Ratio,near plane,far plane 
   glMatrixMode(GL_MODELVIEW); 
   glLoadIdentity(); 
   gluLookAt(camx,camy,camz,  0.0,0.0,0.0,  0.0,1.0,0.0); 
} 

void glSetupFuncs(void) 
{ 
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutIdleFunc(idle); 
   glutSpecialFunc(special); 
   glutKeyboardFunc(keyboard); 
   glutMouseFunc(mouse); 
   glutMotionFunc(motion); 
   glutPassiveMotionFunc(passive); 
   glutSetCursor(GLUT_CURSOR_CROSSHAIR); 
} 

void help(void) 
{ 
   printf("3D Model of a Bicycle\n"); 
   printf("Group Project BY KEERTHAN S AND MAHENDRA MB\n\n");
   printf("UNLOCK THE BICYCLE\n");
   printf("'F' to move forward and to speed up\n"); 
   printf("'B' to slow down or apply break\n"); 
   printf("'L' to rotate the handle in clockwise direction\n"); 
   printf("'R' to rotate the handle in anti-clockwise direction\n"); 
   printf("'s' or 'S' to reset the scene\n"); 
   printf("Arrow keys to move the camera\n"); 
   printf("Mouse to move the scene\n"); 
} 

int main(int argc,char *argv[]) 
{ 
   help(); 
   glutInit(&argc,argv); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowPosition(100,100); 
   glutInitWindowSize(WIN_WIDTH,WIN_HEIGHT); 
   glutCreateWindow("BiCycle"); 
   init(); 
   glSetupFuncs(); 
   glutMainLoop(); 
} 
