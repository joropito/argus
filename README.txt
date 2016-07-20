# argus
Multithreaded pawn scripting scheduler. Allows you to run several pawn scripts using linux pthreads. Ideal for network monitoring tools, massive text files processing and whatever you want.


# More info about pawn at http://www.compuphase.com/pawn/pawn.htm
# Pawn compiler 4.0.4733

* supports loadable modules (example amxpower.cpp)
* JIT option not working yet
* use argus.ini to setup settings
* no memory leaks found by now

Makefile builds:
argusrun: thread manager (it uses argus.ini)
amxrun: to test manually just one compiled script (it needs AMXLIB for loadable modules)
modules: by now only amxpower.so loadable module

------------------------
ARGUS.INI
------------------------
[CONFIG]
name=Argus
version=1.0
plugindir=/argus/plugins/
moduledir=/argus/modules/
sleeptime=1000000 ; sleep time between each frame (plugin.Run() calls)
apps=4 ; amount of apps (4 => APP_0 to APP_3)
test=10 ; set as 0 or less to disable test loops, after this amount of loops the process will end

[APP_0]
threads=3 ; amount of threads for this app
plugin=test1.amx ; plugin for this app
framefunc=prueba ; function to call on this app

[APP_1]
threads=10
plugin=test1.amx
framefunc=init

[APP_2]
threads=5
plugin=test1.amx
framefunc=main
action=loop ; set "loop" action to loop over framefunc, otherwise just 1 call per thread
pause=5 ; sleep time in seconds between framefunc call inside loop
