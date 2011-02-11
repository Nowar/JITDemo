// Copyright (C) 2011 Nowar
//  
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
namespace llvm {
class raw_ostream;
} // namespace llvm

class Usage {
 public:
  static void showUsage(llvm::raw_ostream& os, const char* progName) {
    os << "Usage: " << progName << " LLVM-IR-file\n";
    os << "\n";
    os << progName << " is a trivial JIT-Compiler using LLVM.\n";
    os << "It reads LLVM module from the input argument, creates a JIT-Compiler, and runs on 'main' function.\n";
    os << "Note: We assume 'main' needs no arguments.\n";
  }
 private:
  Usage();
};
