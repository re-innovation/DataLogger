"""
verify.linkitone.py

Verifies that all .ino (Arduino) applications for LinkIt ONE build successfully.

Requirements:
- The Arduino executable folder must exist in the system path
- The LinkItOne SDK must be installed
"""

import os
import subprocess

def getDirectoryName(directory):
    head, tail = os.path.split(directory)
    return tail

def getExpectedINODirectoryName(inoFilename):
    return inoFilename[:-4]

def generateINOList(rootDir):
    inoFiles = []

    for dirPath, subdirList, fileList in os.walk(rootDir):
        for fname in fileList:
            if fname.endswith(".ino"):
                expectedDirName = getExpectedINODirectoryName(fname)
                actualDirName = getDirectoryName(dirPath)
                if (expectedDirName == actualDirName):
                    inoFiles.append(os.path.join(dirPath, fname))
                else:
                    print("File {0} expected to be in directory {1} (actually in {2})".format(fname, expectedDirName, actualDirName))

    return inoFiles

def verify(ide, f):
    command_list = [ide, "--verify", "--board", "arduino:mtk:linkit_one", f]

    if (subprocess.call(command_list) != 0):
        print("Verify failed for {0}".format(os.path.basename(f)))

def verifyAll(rootDir):

    ide = os.environ['LINKITONEIDE']
    print("Using arduino exe at {0}".format(ide))
    
    for f in generateINOList(rootDir):
        verify(ide, f)


if __name__ == "__main__":
	verifyAll(os.getcwd())

