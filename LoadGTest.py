import urllib2
import zipfile
from StringIO import StringIO
import os.path

def extractWithoutPath(archive, archivePath, extractPath):
    newFile = open(extractPath,'w')
    newFile.write(archive.read(archivePath))
    newFile.close();

files = [
    ['gtest-1.6.0/fused-src/gtest/gtest-all.cc', 'test/gtest/gtest-all.cc'],
    ['gtest-1.6.0/fused-src/gtest/gtest.h', 'test/gtest/gtest.h']
]

filesMissing = False

for (archivePath, extractPath) in files:
    if not os.path.isfile(extractPath):
        filesMissing = True

if filesMissing:
    print "GTest not found, downloading..."

    if not os.path.exists('test/gtest'):
        os.makedirs('test/gtest')

    response = urllib2.urlopen('https://googletest.googlecode.com/files/gtest-1.6.0.zip')
    gtestRawZip = response.read()
    gtestZip = zipfile.ZipFile(StringIO(gtestRawZip))
    for (archivePath, extractPath) in files:
        extractWithoutPath(gtestZip, archivePath, extractPath)