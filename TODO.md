
### TODO

- check socket disconnection
- exception handling
- ! Socket destructor `close`
- clang tidy
- c++98 compile
- reformat tlucanti::IRCParser::parse and remove if/else
- make change nickname nice message
- write 001 instead 1 in IRC code response
- remove all #warnings

### NOW

- backward file transfer
- write all commands in README.md
- fix WHO command realname
- add all commands from README.md
- make checker for server compats (32 channels per user, max channel len = 50,
	max nick len = 20, max topic len = 300)

### DONE

- server ping response
- forward file transfer
- make invalid port checker in argv
- ping - pong
- debug web
- makefile
- irc command parcer
- delete other `IRCParserException` constructors
- all user and modes support

### COMMITS 

7162

|	+10922	|	-3760	|
|-----------|-----------|
|	+1039	|	-206	|
|	+105	|	-574	|
|	+21		|	-0		|
|	+135	|	-15		|
|	+435	|	-300	|
|	+740	|	-195	|
|	+328	|	-88		|
|	+1300	|	-407	|
|	+457	|	-103	|
|	+317	|	-85		|
|	+440	|	-175	|
|	+276	|	-63		|
|	+466	|	-215	|
|	+365	|	-158	|
|	+949	|	-273	|
|	+361	|	-210	|
|	+1323	|	-378	|
|	+887	|	-54		|
|	+84		|	-32		|
|	+402	|	-159	|
|	+402	|	-70		|
|	+90		|	-0		|
