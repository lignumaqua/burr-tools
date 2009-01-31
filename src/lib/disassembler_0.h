/* Burr Solver
 * Copyright (C) 2003-2009  Andreas R�ver
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
#ifndef __DISASSEMBLER_0_H__
#define __DISASSEMBLER_0_H__

#include "disassembler_a.h"

class separation_c;

class disassemblerNode_c;

/* this class is a disassembler especially tunes for puzzles with no
 * coordinated motion
 *
 * is is implemented using Bill Cuttlers algorithm, so please read there
 * in case you are interested how it works. The comments are written with
 * the thought that you know his algorithm
 */
class disassembler_0_c : public disassembler_a_c {

private:

  /* the real disassembly routine. It separates the puzzle into 2 parts
   * and gets called recursively with each subpart to disassemble
   *
   * the return is the disassembly tree for that part
   *
   * pieces contains the names of all the pieces that are still inside the
   * subpuzzle puzzle, start defines the starting position of these pieces
   */
  separation_c * disassemble_rec(const std::vector<unsigned int> & pieces, disassemblerNode_c * start);

public:

  /* construct the disassembler for this concrete problem, is can not be
   * changed, once you done that but you can analyse many assemblies for
   * disassembability
   */
  disassembler_0_c(const problem_c * puz) : disassembler_a_c(puz) { }

  ~disassembler_0_c() { }
};

#endif
