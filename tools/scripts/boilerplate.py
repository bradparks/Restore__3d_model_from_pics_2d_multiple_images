#!/usr/bin/env python
# Copyright (c) 2015, Kai Wolf
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Create files with copyright boilerplate and header include guards.
# Usage: tools/boilerplate.py path/to/file.{hpp,cpp}

from datetime import date
import os
import os.path
import sys

LINES = [
    'Copyright (c) %d, Kai Wolf\n' % date.today().year,
    'Permission is hereby granted, free of charge, to any person obtaining a copy',
    'of this software and associated documentation files (the "Software"), to deal',
    'in the Software without restriction, including without limitation the rights',
    'to use, copy, modify, merge, publish, distribute, sublicense, and/or sell',
    'copies of the Software, and to permit persons to whom the Software is',
    'furnished to do so, subject to the following conditions:\n',
    'The above copyright notice and this permission notice shall be included in',
    'all copies or substantial portions of the Software.\n',
    'THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR',
    'IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,',
    'FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE',
    'AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER',
    'LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,',
    'OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE',
    'SOFTWARE.'
]

EXTENSIONS_TO_COMMENTS = {
    'h': '//',
    'hpp': '//',
    'cpp': '//',
    'py': '#'
}

def get_header(filename):
  _, ext = os.path.splitext(filename)
  ext = ext[1:]
  comment = EXTENSIONS_TO_COMMENTS[ext] + ' '
  return '\n'.join([comment + line for line in LINES])


def _CppHeader(filename):
  guard = filename.replace('/', '_').replace('.', '_').upper() + '_'
  return '\n'.join([
    '',
    '#ifndef ' + guard,
    '#define ' + guard,
    '',
    '// C system files',
    '// none',
    '',
    '// C++ system files',
    '// none',
    '',
    '// header files of other libraries',
    '// none',
    '',
    '// header files of project libraries',
    '// none',
    '',
    '#endif',
    ''
  ])


def cpp_implementation(filename):
  base, _ = os.path.splitext(filename)
  include = '#include "' + base + '.h"'
  return '\n'.join([
    '', include,
    '',
    '// C system files',
    '// none',
    '',
    '// C++ system files',
    '// none',
    '',
    '// header files of other libraries',
    '// none',
    '',
    '// header files of project libraries',
    '// none',
    '',
    ])


def create_file(filename):
  contents = get_header(filename) + '\n'

  if filename.endswith('.h'):
    contents += _CppHeader(filename)
  elif filename.endswith('.cpp'):
    contents += cpp_implementation(filename)

  fd = open(filename, 'w')
  fd.write(contents)
  fd.close()


def main():
  files = sys.argv[1:]
  if len(files) < 1:
    print >> sys.stderr, 'Usage: boilerplate.py path/file.hpp path/file.cpp'
    return 1

  # Perform checks first so that the entire operation is atomic.
  for f in files:
    _, ext = os.path.splitext(f)
    if not ext[1:] in EXTENSIONS_TO_COMMENTS:
      print >> sys.stderr, 'Unknown file type for %s' % f
      return 2

    if os.path.exists(f):
      print >> sys.stderr, 'A file at path %s already exists' % f
      return 2

  for f in files:
    create_file(f)


if __name__ == '__main__':
  sys.exit(main())
