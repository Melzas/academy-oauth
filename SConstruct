from SConfig import *

configuration = ARGUMENTS.get('config', RELEASE_CONFIG);

if configuration == TEST_CONFIG:
    execfile('LoadGTest.py')

if Environment()['CC'] == 'cl':
    environment = Environment(CPPPATH='#include', tools = ['mingw']);
else:
    environment = Environment(CPPPATH='#include')

if environment['CC'] == 'gcc':
    if configuration == TEST_CONFIG:
        environment.Append(CCFLAGS = ['-Wall', '-Wextra', '-Werror', '-pedantic', '-std=gnu++0x'])
    else:
        environment.Append(CCFLAGS = ['-Wall', '-Wextra', '-Werror', '-pedantic', '-std=c++0x'])
else:
    print 'Unsupported Compiler: ' + environment['CC']

if configuration == TEST_CONFIG:
    SConscript('test/SConscript', variant_dir = 'build/' + configuration, duplicate = 0,
    exports = {'environment' : environment})
else:
    SConscript('src/SConscript', variant_dir = 'build/' + configuration, duplicate = 0,
    exports = {'configuration' : configuration, 'environment' : environment})