/* BurrTools
 *
 * BurrTools is the legal property of its developers, whose
 * names are listed in the COPYRIGHT file, which is included
 * within the source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "piececolor.h"

#include <math.h>

#define COLS 18

// the table for the first COLS fixed defined colours
static float r[COLS] = {
  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.6f,
  0.0f, 0.6f, 0.6f, 0.0f, 0.6f, 0.0f, 0.6f, 1.0f, 1.0f
};
static float g[COLS] = {
  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.6f, 0.0f,
  0.6f, 0.6f, 0.0f, 1.0f, 1.0f, 0.6f, 0.0f, 0.6f, 0.0f
};
static float b[COLS] = {
  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.6f, 0.0f, 0.0f,
  0.6f, 0.0f, 0.6f, 0.6f, 0.0f, 1.0f, 1.0f, 0.0f, 0.6f
};

#define JITTERS 53

// the table with the modification values for the multi-pieces
static float jr[JITTERS] = {
   0.0f,
  -0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
  -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
   0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.5f, -0.5f,
  -0.4f,  0.4f, -0.4f,  0.4f, -0.4f,  0.4f, -0.4f,  0.4f,  0.4f,
  -0.4f, -0.4f,  0.4f,  0.4f, -0.4f, -0.4f,  0.4f,  0.0f,  0.0f,
   0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.4f, -0.4f
};
static float jg[JITTERS] = {
   0.0f,
  0.5f,  -0.5f, 0.5f,  -0.5f, 0.5f,  -0.5f, 0.5f,  -0.5f,  -0.5f,
  -0.3f,  0.3f, -0.3f,  0.0f,  0.0f,  0.0f,  0.0f,  0.3f, -0.3f,
  -0.3f,  0.3f,  0.0f,  0.0f,  0.3f, -0.3f,  0.0f,  0.0f,
  -0.4f,  0.4f,  0.4f, -0.4f, -0.4f,  0.4f,  0.4f, -0.4f,  0.4f,
  -0.4f,  0.4f, -0.4f,  0.0f,  0.0f,  0.0f,  0.0f,  0.4f, -0.4f,
  -0.4f,  0.4f,  0.0f,  0.0f,  0.4f, -0.4f,  0.0f,  0.0f
};
static float jb[JITTERS] = {
   0.0f,
  -0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
   0.0f,  0.0f,  0.0f,  0.3f, -0.3f,  0.3f, -0.3f,  0.3f, -0.3f,
   0.3f, -0.3f,  0.3f, -0.3f,  0.0f,  0.0f,  0.0f,  0.0f,
  -0.4f,  0.4f,  0.4f, -0.4f,  0.4f, -0.4f, -0.4f,  0.4f,  0.0f,
   0.0f,  0.0f,  0.0f,  0.4f, -0.4f,  0.4f, -0.4f,  0.4f, -0.4f,
   0.4f, -0.4f,  0.4f, -0.4f,  0.0f,  0.0f,  0.0f,  0.0f
};

float pieceColorR(int x) {
  if (x < COLS)
    return r[x];
  else
    return float((1+sin(0.7*x))/2);
}

float pieceColorG(int x) {
  if (x < COLS)
    return g[x];
  else
    return float((1+sin(1.3*x+1.5))/2);
}

float pieceColorB(int x) {
  if (x < COLS)
    return b[x];
  else
    return float((1+sin(3.5*x+2.3))/2);
}

unsigned int pieceColorRi(int x) {
  if (x < COLS)
    return (unsigned int)(r[x]*255);
  else
    return (unsigned int)(255*(1+sin(0.7*x))/2);
}

unsigned int pieceColorGi(int x) {
  if (x < COLS)
    return (unsigned int)(g[x]*255);
  else
    return (unsigned int)(255*(1+sin(1.3*x+1.5))/2);
}

unsigned int pieceColorBi(int x) {
  if (x < COLS)
    return (unsigned int)(b[x]*255);
  else
    return (unsigned int)(255*(1+sin(3.5*x+2.3))/2);
}


/* the problem is that simply adding the jitter
 * would cause an overflow every now and then, so we
 * search in the list of possible jitter values
 * for the x-th with no overflow
 * when the table is exhausted, we simple do no
 * jittering any longer
 */
static int getJitter(int val, int sub) {
  int j = 0;
  float x;

  while (j < JITTERS) {
    x = pieceColorR(val) + jr[j];
    if ((x < 0) || (x > 1)) {
      j++;
      continue;
    }
    x = pieceColorG(val) + jg[j];
    if ((x < 0) || (x > 1)) {
      j++;
      continue;
    }
    x = pieceColorB(val) + jb[j];
    if ((x < 0) || (x > 1)) {
      j++;
      continue;
    }

    if (sub == 0)
      break;

    sub--;
    j++;
  }

  if (j == JITTERS) j = 0;

  return j;
}

static float ramp(float val) {
  return 0.5+0.5*fabs(1-2*val);
}




float max(float a, float b, float c) {
   return ((a > b)? (a > c ? a : c) : (b > c ? b : c));
}
float min(float a, float b, float c) {
   return ((a < b)? (a < c ? a : c) : (b < c ? b : c));
}

/* Convert rgb values of voxel color to hsv
 * Use references to allow returning 3 values
 */

void rgb_to_hsv(float r, float g, float b, float *add_h, float *add_s, float *add_v) {
   float cmax = max(r, g, b); // maximum of r, g, b
   float cmin = min(r, g, b); // minimum of r, g, b
   float diff = cmax-cmin; // diff of cmax and cmin.
   if (cmax == cmin){
      *add_h = 0;
   }
   else if (cmax == r){
      *add_h = fmod((60 * ((g - b) / diff) + 360), 360.0);
   }
   else if (cmax == g){
      *add_h = fmod((60 * ((b - r) / diff) + 120), 360.0);
   }
   else if (cmax == b){
      *add_h = fmod((60 * ((r - g) / diff) + 240), 360.0);
   }
   // if cmax equal zero
      if (cmax == 0) {
         *add_s = 0;
      }
      else {
         *add_s = (diff / cmax);
      }
   // compute v
   *add_v = cmax;
}


/* Convert hsv values of voxel color to rgb
 * Use references to allow returning 3 values
 */

void hsv_to_rgb(float h, float s, float v, float *add_r, float *add_g, float *add_b){
    float C = s*v;
    float X = C*(1-abs(fmod(h/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(h >= 0 && h < 60){
        r = C,g = X,b = 0;
    }
    else if(h >= 60 && h < 120){
        r = X,g = C,b = 0;
    }
    else if(h >= 120 && h < 180){
        r = 0,g = C,b = X;
    }
    else if(h >= 180 && h < 240){
        r = 0,g = X,b = C;
    }
    else if(h >= 240 && h < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    *add_r = r+m;
    *add_g = g+m;
    *add_b = b+m;
}


/* For each of R, G, and B for multiple identical parts. Convert color to hsv
 * Then modify hue and saturation for subsequent pieces using the count, sub.
 * Ensure we don't over or underflow. Hue valid from 0 - 360, saturation  valid from 0 - 1.
 * One option. Decrement saturation by 0.1 every step, incrment hue by 10 every 10 steps.
 */

float pieceColorR(int x, int sub) {
  
  float r = pieceColorR(x);
  float g = pieceColorG(x);
  float b = pieceColorB(x);

  float h, s, v;
  rgb_to_hsv(r, g, b, &h, &s, &v);
  s = s - fmod((sub)/5.0,1);
  if (s<0) { s = s + 1; }
  h = fmod(h + 10.0 * int(sub / 5.0), 360.0);
  hsv_to_rgb(h, s, v, &r, &g, &b);

  return r;
}

float pieceColorG(int x, int sub) {
  
  float r = pieceColorR(x);
  float g = pieceColorG(x);
  float b = pieceColorB(x);

  float h, s, v;
  rgb_to_hsv(r, g, b, &h, &s, &v);
  s = s - fmod((sub)/5.0,1);
  if (s<0) { s = s + 1; }
  h = fmod(h + 10.0 * int(sub / 5.0), 360.0);
  hsv_to_rgb(h, s, v, &r, &g, &b);

  return g;
}

float pieceColorB(int x, int sub) {
  
  float r = pieceColorR(x);
  float g = pieceColorG(x);
  float b = pieceColorB(x);

  float h, s, v;
  rgb_to_hsv(r, g, b, &h, &s, &v);
  s = s - fmod((sub)/5.0,1);
  if (s<0) { s = s + 1; }
  h = fmod(h + 10.0 * int(sub / 5.0), 360.0);
  hsv_to_rgb(h, s, v, &r, &g, &b);

  return b;
}





/*
float pieceColorR(int x, int sub) {

  float jitter = jr[getJitter(x, sub)];
  float val = pieceColorR(x);

  return val + jitter;
}

float pieceColorG(int x, int sub) {

  float jitter = jg[getJitter(x, sub)];
  float val = pieceColorG(x);

  return val + jitter;
}

float pieceColorB(int x, int sub) {

  float jitter = jb[getJitter(x, sub)];
  float val = pieceColorB(x);

  return val + jitter;
}

*/

unsigned int pieceColorRi(int x, int sub) {

  //float jitter = jr[getJitter(x, sub)];
  float val = pieceColorR(x,sub);

  return (unsigned int)((val)*255);
}

unsigned int pieceColorGi(int x, int sub) {

  //float jitter = jg[getJitter(x, sub)];
  float val = pieceColorG(x,sub);

  return (unsigned int)((val)*255);
}

unsigned int pieceColorBi(int x, int sub) {

  //float jitter = jb[getJitter(x, sub)];
  float val = pieceColorB(x,sub);

  return (unsigned int)((val)*255);
}

/* float darkPieceColor(float f) { return float(f * 0.9); } */
/* float lightPieceColor(float f) { return float(1 - (0.9 * (1-f))); } */

float darkPieceColor(float f) { return float(f); }
float lightPieceColor(float f) { return float(f); }

Fl_Color contrastPieceColor(int x) {
  if (3*pieceColorRi(x) + 6*pieceColorGi(x) + pieceColorBi(x) < 1275)
    return fl_rgb_color(255, 255, 255);
  else
    return fl_rgb_color(0, 0, 0);
}


