/* Burr Solver
 * Copyright (C) 2003-2007  Andreas R�ver
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
#include "WindowWidgets.h"
#include "piececolor.h"
#include "guigridtype.h"

#include "../lib/voxel.h"
#include "../lib/puzzle.h"

#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Multiline_Output.H>

#include <math.h>

// some tool widgets, that may be swapped out later into another file

// draws an definable evenly spaced number of lines in one direction
class LineSpacer : Fl_Widget {

  int lines;
  bool vertical;
  int gap;

  public:

    LineSpacer(int x, int y, int w, int h, int borderSpace) : Fl_Widget(x, y, w, h), lines(2), vertical(true), gap(borderSpace) {}

    void draw(void) {

      fl_color(color());
      fl_rectf(x(), y(), w(), h());

      if (lines <= 1) return;

      fl_color(0);

      if (vertical) {

        for (int i = 0; i < lines; i++) {
          int ypos = y()+ gap + (h()-2*gap-1)*i/(lines-1);
          fl_line(x(), ypos, x()+w()-1, ypos);
        }

      } else {

        for (int i = 0; i < lines; i++) {
          int xpos = x()+ gap + (w()-2*gap-1)*i/(lines-1);
          fl_line(y(), xpos, y()+w()-1, xpos);
        }
      }

    }

    void setLines(int l, int vert) {
      lines = l;
      vertical = vert;
      redraw();
    }

};

static void cb_VoxelEditGroupZselect_stub(Fl_Widget* o, void* v) { ((VoxelEditGroup_c*)v)->cb_Zselect((Fl_Slider*)o); }
static void cb_VoxelEditGroupSqedit_stub(Fl_Widget* o, void* v) { ((VoxelEditGroup_c*)v)->cb_Sqedit((gridEditor_c*)o); }

VoxelEditGroup_c::VoxelEditGroup_c(int x, int y, int w, int h, puzzle_c * puzzle, const guiGridType_c * ggt) : Fl_Group(0, 0, 300, 300), layoutable_c(x, y, w, h) {

  x = 0;
  y = 0;
  w = 300;
  h = 300;

  zselect = new Fl_Slider(x, y, 15, h);
  zselect->tooltip(" Select Z Plane ");
  zselect->color((Fl_Color)237);
  zselect->step(1);
  zselect->callback(cb_VoxelEditGroupZselect_stub, this);
  zselect->clear_visible_focus();

  space = new LineSpacer(x+15, y, 5, h, 4);

  {
    Fl_Box* o = new Fl_Box(x+25, y, 5, h-5);
    o->box(FL_FLAT_BOX);
    o->color(fl_rgb_color(0, 192, 0));
  }
  {
    Fl_Box* o = new Fl_Box(x+25, y+h-5, w-25, 5);
    o->box(FL_FLAT_BOX);
    o->color((Fl_Color)1);
  }

  sqedit = ggt->getGridEditor(x+35, y, w-35, h-10, puzzle);
  sqedit->tooltip(" Fill and empty cubes ");
  sqedit->box(FL_NO_BOX);
  sqedit->callback(cb_VoxelEditGroupSqedit_stub, this);
  sqedit->clear_visible_focus();

  resizable(sqedit);
}

void VoxelEditGroup_c::newGridType(const guiGridType_c * ggt, puzzle_c * puzzle) {

  gridEditor_c * nsq;

  nsq = ggt->getGridEditor(sqedit->x(), sqedit->y(), sqedit->w(), sqedit->h(), puzzle);
  nsq->tooltip(" Fill and empty cubes ");
  nsq->box(FL_NO_BOX);
  nsq->callback(cb_VoxelEditGroupSqedit_stub, this);
  nsq->clear_visible_focus();

  resizable(nsq);

  remove(sqedit);
  delete sqedit;

  sqedit = nsq;
  add(sqedit);
}


void VoxelEditGroup_c::setZ(unsigned int val) {
  if (val > zselect->maximum()) val = (unsigned int)zselect->maximum();
  zselect->value(int(zselect->maximum()-val));
  sqedit->setZ(val);
}

void VoxelEditGroup_c::setPuzzle(puzzle_c * puzzle, unsigned int num) {
  sqedit->setPuzzle(puzzle, num);
  if (puzzle && (num < puzzle->shapeNumber())) {
    voxel_c * v = puzzle->getShape(num);
    if (v) {
      zselect->bounds(0, v->getZ()-1);
      zselect->value(int(zselect->maximum()-sqedit->getZ()));
      space->setLines(v->getZ(), true);
    }
  }
}

void VoxelEditGroup_c::draw() {
  fl_push_clip(x(), y(), w(), h());
  Fl_Group::draw();
  fl_pop_clip();
}

static void cb_BlockListGroupSlider_stub(Fl_Widget* o, void* v) { ((LBlockListGroup_c*)(o->parent()))->cb_slider(); }
void LBlockListGroup_c::cb_list(void) {

  if (List->getReason() == PieceSelector::RS_CHANGEDHIGHT) {

    Slider->range(0, List->calcHeight());
    if (Slider->value() > List->calcHeight())
      Slider->value(List->calcHeight());

    List->setShift((int)Slider->value());

  } else {
    callbackReason = List->getReason();
    do_callback(this, user_data());
  }
}

static void cb_BlockListGroupList_stub(Fl_Widget* o, void* v) { ((LBlockListGroup_c*)(o->parent()))->cb_list(); }

LBlockListGroup_c::LBlockListGroup_c(int x, int y, int w, int h, BlockList * l) : Fl_Group(0, 0, 100, 100), layoutable_c(x, y, w, h), List(l) {

  box(FL_DOWN_FRAME);

  x = y = 0;
  w = h = 100;
  x++; y++; w-=2; h-=2;

  // important the list must be added first to be drawn first
  // this is necessary to find out the size of the list and update the
  // slider accordingly
  add(List);

  Slider = new Fl_Slider(x+w-15-1, y-1, 15+2, h+2);
  Slider->maximum(0);
  Slider->callback(cb_BlockListGroupSlider_stub);
  Slider->clear_visible_focus();

  w-=15;

  new Fl_Box(FL_UP_FRAME, x, y, w, h, 0);
  x++; y++; w-=2; h-=2;

  List->resize(x, y, w, h);
  List->callback(cb_BlockListGroupList_stub);

  resizable(List);
  end();
}

static void cb_ConstraintsGroupSlider_stub(Fl_Widget* o, void* v) { ((LConstraintsGroup_c*)(o->parent()))->cb_slider(); }
static void cb_ConstraintsGroupList_stub(Fl_Widget* o, void* v) { ((LConstraintsGroup_c*)(o->parent()))->cb_list(); }
void LConstraintsGroup_c::cb_list(void) {

  if (List->getReason() == ColorConstraintsEdit::RS_CHANGEDHIGHT) {

    Slider->range(0, List->calcHeight());
    if (Slider->value() > List->calcHeight())
      Slider->value(List->calcHeight());

    List->setShift((int)Slider->value());

  } else {
    callbackReason = List->getReason();
    do_callback(this, user_data());
  }
}

LConstraintsGroup_c::LConstraintsGroup_c(int x, int y, int w, int h, ColorConstraintsEdit * l) : Fl_Group(0, 0, 100, 100), layoutable_c(x, y, w, h), List(l) {

  box(FL_DOWN_FRAME);

  x = y = 0;
  w = h = 100;
  x++; y++; w-=2; h-=2;

  add(List);

  Slider = new Fl_Slider(x+w-15-1, y-1, 15+2, h+2);
  Slider->maximum(0);
  Slider->callback(cb_ConstraintsGroupSlider_stub);
  Slider->clear_visible_focus();

  w-=15;

  new Fl_Box(FL_UP_FRAME, x, y, w, h, 0);
  x++; y++; w-=2; h-=2;

  List->resize(x, y, w, h);
  List->callback(cb_ConstraintsGroupList_stub);

  resizable(List);
  end();
}

static void cb_View3dGroupSlider_stub(Fl_Widget* o, void* v) { ((View3dGroup*)(o->parent()))->cb_slider(); }

void View3dGroup::cb_slider(void) {
  View3D->setSize(exp(6-slider->value()));
}

View3dGroup::View3dGroup(int x, int y, int w, int h, const guiGridType_c * ggt) : Fl_Group(x, y, w, h) {
  box(FL_DOWN_BOX);

  View3D = ggt->getVoxelDrawer(x, y, w-15, h);
  View3D->tooltip(" Rotate the puzzle by dragging with the mouse ");
  View3D->box(FL_NO_BOX);

  slider = new Fl_Slider(x+w-15, y, 15, h);
  slider->tooltip("Zoom view.");
  slider->maximum(6);
  slider->minimum(0);
  slider->step(0.01);
  slider->value(2);
  slider->callback(cb_View3dGroupSlider_stub);
  slider->clear_visible_focus();

  cb_slider();

  resizable(View3D);
  end();
}

void View3dGroup::newGridType(const guiGridType_c * ggt) {

  View3D->hide();

  voxelDrawer_c * nv;

  nv = ggt->getVoxelDrawer(View3D->x(), View3D->y(), View3D->w(), View3D->h());
  nv->tooltip(" Rotate the puzzle by dragging with the mouse ");
  nv->box(FL_NO_BOX);

  resizable(nv);

  remove(View3D);
  delete View3D;

  View3D = nv;
  add(View3D);

  cb_slider();

  View3D->show();
}

LSeparator_c::LSeparator_c(int x, int y, int w, int h, const char * label, bool button) : Fl_Group(0, 0, 300, 10), layoutable_c(x, y, w, h) {

  x = 0;
  y = 0;
  w = 300;
  h = 10;

  if (label) {
    int lw, lh;

    fl_font(labelfont(), labelsize()-4);

    lw = lh = 0;
    fl_measure(label, lw, lh);
    (new Fl_Box(FL_FLAT_BOX, x, y, lw+4, h, label))->labelsize(labelsize()-4);

    x += lw + 6;
    w -= lw + 6;
  }

  if (button) {
    new Fl_Box(FL_UP_BOX, x+w-8, y+h/2-4, 8, 8, 0);
    w -= 8;
  }

  resizable(new Fl_Box(FL_THIN_DOWN_BOX, x, y+h/2-1, w, 2, 0));

  end();
}

ResultViewer::ResultViewer(int x, int y, int w, int h, puzzle_c * p) : Fl_Box(0, 0, 10, 10), layoutable_c(x, y, w, h), puzzle(p), problem(0) {
  bt_assert(p);
  bg = color();
//  setcontent();
  box(FL_BORDER_BOX);
  clear_visible_focus();
}

void ResultViewer::setPuzzle(puzzle_c * p, unsigned int prob) {
  puzzle = p;
  problem = prob;
  redraw();
//  setcontent();
}

void ResultViewer::draw(void) {
  if (problem >= puzzle->problemNumber() ||
      (puzzle->probGetResult(problem) > puzzle->shapeNumber())) {
    label("No Result");
    color(bg);
    labelcolor(fl_rgb_color(255, 0, 0));
  } else {
    static char txt[120];

    unsigned int result = puzzle->probGetResult(problem);

    if (puzzle->probGetResultShape(problem)->getName().length())
      snprintf(txt, 120, "Result: S%i - %s", result+1, puzzle->probGetResultShape(problem)->getName().c_str());
    else
      snprintf(txt, 19, "Result: S%i", result + 1);

    unsigned char r, g, b;

    r = pieceColorRi(result);
    g = pieceColorGi(result);
    b = pieceColorBi(result);

    color(fl_rgb_color(r, g, b));

    if ((int)3*r + 6*g + 1*b > 1275)
      labelcolor(fl_rgb_color(0, 0, 0));
    else
      labelcolor(fl_rgb_color(255, 255, 255));

    label(txt);
  }
  Fl_Box::draw();
}

void View3dGroup::showSingleShape(const puzzle_c * puz, unsigned int shapeNum) {
  View3D->update(false);
  View3D->showSingleShape(puz, shapeNum);
  View3D->update(true);
}

void View3dGroup::showProblem(const puzzle_c * puz, unsigned int probNum, unsigned int selShape) {

  View3D->update(false);
  View3D->showProblem(puz, probNum, selShape);
  View3D->update(true);
}

void View3dGroup::showColors(const puzzle_c * puz, voxelDrawer_c::colorMode mode) {
  View3D->update(false);
  View3D->showColors(puz, mode);
  View3D->update(true);
}

void View3dGroup::showAssembly(const puzzle_c * puz, unsigned int probNum, unsigned int solNum) {
  View3D->update(false);
  View3D->showAssembly(puz, probNum, solNum);
  View3D->update(true);
}

void View3dGroup::showPlacement(const puzzle_c * puz, unsigned int probNum, unsigned int piece, unsigned char trans, int x, int y, int z) {

  View3D->update(false);
  View3D->showPlacement(puz, probNum, piece, trans, x, y, z);
  View3D->update(true);
}

void View3dGroup::updatePositions(piecePositions_c *shifting) {
  View3D->update(false);
  View3D->updatePositions(shifting);
  View3D->update(true);
}

void View3dGroup::updateVisibility(PieceVisibility * pcvis) {
  View3D->update(false);
  View3D->updateVisibility(pcvis);
  View3D->update(true);
}

static void cb_ButtonGroup_stub(Fl_Widget* o, void* v) { ((ButtonGroup*)v)->cb_Push((Fl_Button*)o); }

ButtonGroup::ButtonGroup(int x, int y, int w, int h) : layouter_c(x, y, w, h), currentButton(0) {
  end();
}

Fl_Button * ButtonGroup::addButton(void) {

  int c = children();

  LFl_Button * b = new LFl_Button(0, c, 0, 1, 1);
  b->selection_color(fl_lighter(color()));
  b->clear_visible_focus();
  b->setPadding(2, 0);

  b->callback(cb_ButtonGroup_stub, this);

  if (c == 0)
    b->set();
  else
    b->clear();

  add(b);

  return b;
}

void ButtonGroup::cb_Push(Fl_Button * btn) {

  Fl_Button ** a = (Fl_Button**) array();

  for (int i = 0; i < children(); i++)
    if (a[i] != btn) {
      a[i]->clear();
    } else {
      a[i]->set();
      currentButton = i;
    }

  do_callback();
}

void ButtonGroup::select(int num) {
  if (num < children())
    cb_Push((Fl_Button*)array()[num]);
}

LStatusLine::LStatusLine(int x, int y, int w, int h) : layouter_c(x, y, w, h) {

  text = new LFl_Box(0, 0, 1, 1);
  text->box(FL_UP_BOX);
  text->color(FL_BACKGROUND_COLOR);
  text->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  text->weight(1, 0);

  mode = new ButtonGroup(1, 0, 1, 1);
  mode->addButton()->image(pm.get(ViewModeNormal_xpm));
  mode->addButton()->image(pm.get(ViewModeColor_xpm));
  mode->addButton()->image(pm.get(ViewMode3D_xpm));

  clear_visible_focus();

  end();
}

void LStatusLine::setText(const char * t) {

  text->copy_label(t);
}

