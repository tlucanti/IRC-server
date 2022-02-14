
### TODO

- check socket disconnection
- exception handling
- ! Socket destructor `close`
- clang tidy
- c++98 compile
- reformat tlucanti::IRCParser::parse and remove if/else
- make invalid port checker in argv
- make checker for server compats (32 channels per user, max channel len = 50,
	max nick len = 20, max topic len = 300)
- make change nickname nice message
- write 001 instead 1 in IRC code response

### NOW

- write all commands in README.md
- fix WHO command realname
- add all commands from README.md

### DONE

- debug web
- makefile
- irc command parcer
- delete other `IRCParserException` constructors

### COMMITS


+6045	-1787
---------------
+440	-175
+276	-63
+466	-215
+365	-158
+949	-273
+361	-210
+1323	-378
+887	-54
+84		-32
+402	-159
+402	-70
+90		-0
