#!/usr/bin/python
# Copyright (c) 2015-2016, Kai Wolf
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
# This script sorts the includes of the source and header files of the given
# directory.
#
# Usage: python sort_includes.py source_file_directory

import os.path
import re
import sys


def main(path):
    for subdir, dirs, files in os.walk(path):
        for afile in files:
            filepath = subdir + os.sep + afile

            # aggregate include headers for one file
            if filepath.endswith("cpp") or filepath.endswith("hpp"):
                if not os.path.isfile(filepath):
                    print 'File does not exist.'
                else:
                    with open(filepath) as f:
                        content = f.read().splitlines()
                pattern = '\#include.*\Z'
                includelines = []
                includes_sorted = False

                # do the actual sorting of the include headers
                for index in range(len(content)):
                    if re.match(pattern, content[index]):
                        # sort include headers for each block separately
                        condition = re.match(pattern, content[index])
                        while condition:
                            includelines.append(content[index])
                            content.pop(index)
                            condition = re.match(pattern, content[index])
                        # check if includes have changed
                        old_includes = includelines[:]
                        includelines.sort(key=str.lower)
                        includes_sorted |= (old_includes != includelines)
                        includelines.reverse()
                        for line in includelines:
                            content.insert(index, line)
                        includelines[:] = []
                if includes_sorted:
                    print filepath, "sorted"
                f.close()

                myfile = open(filepath, 'w')

                for line in content:
                    myfile.write(line + "\n")

                myfile.close()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Usage: python sort_includes.py source_file_directory"
        sys.exit()
    path = sys.argv[1]
    main(path)
