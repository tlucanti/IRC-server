
# IRC C++ SERVER

## Server commands

### \[:USERNAME\] PASS PASSWORD
> used `PASSWORD` to authorize server connection (must be first command from
> client to server)
> example:
> ```
> PASS mypassword
> ```
> responses:
> - ERR_NEEDMOREPARAMS

### \[:OLDINCK\] NICK USERNAME
> change or create user nickname to `USERNAME`
> if user with current nickname exist in server - ERR_NICKCOLLISION thrown
> prefix `OLDNICK` can be used to change other user nickname
> #### example:
> ```
> NICK mynewnick
> ```
> ```
> :oldnick NICK newnick
> ```
> #### responses:
> - ERR_NONICKNAMEGIVEN
> - ERR_ERRONEUSNICKNAME
> - ERR_NICKNAMEINUSE
> - ERR_NICKCOLLISION

### \[:SOURCENAME\] USER USERNAME HOSTNAME SERVERNAME :REALNAME
> command used to authenticate user in server database (must be second command
> from client to server after command `PASS`)
> #### example:
> ```
> USER guest tolmoon tolsun :Ronnie Reagan
> ```
> #### responses:
> - ERR_NEEDMOREPARAMS
> - ERR_ALREADYREGISTRED

### \[:USERNAME\] OPER LOGIN PASSWORD
> make current user irc operator, `LOGIN` and `PASSWORD` are required to gain
> privileges
> #### example:
> ```
> OPER login password
> ```
> #### responses:
> - ERR_NEEDMOREPARAMS
> - RPL_YOUREOPER
> - ERR_NOOPERHOST
> - ERR_PASSWDMISMATCH

### \[:USERNAME\] QUIT :\[MESSAGE\]
> command to end client session, server closes connection with client after this
> message. If `MESSAGE` is not given - nickname will be used
> #### example:
> ```
> QUIT :Gone to have lunch
> ```
> #### responses:
> None

### \[:USERNAME\] JOIN \#CHANNEL1\[,&CHANNEL2 \[...\]\] \[KEY1\[,KEY2\] \[...\]\]
> join to channels named by comas using keys (if provided), if channel is not
> exist - creates channel and makes current user operator of new channel
> #### example:
> ```
> JOIN #ch1
> ```
> ```
> JOIN &ch2 password2, #ch3 password3
> ```
> ```
> :user JOIN #channel
> ```
> #### responses
> - ERR_NEEDMOREPARAMS
> - ERR_BANNEDFROMCHAN
> - ERR_INVITEONLYCHAN
> - ERR_BADCHANNELKEY
> - ERR_CHANNELISFULL
> - ERR_BADCHANMASK
> - ERR_NOSUCHCHANNEL
> - ERR_TOOMANYCHANNELS
> - RPL_TOPIC

### \[:USERNAME\] PART \#CHANNEL1 \[, &CHANNEL2\] \[...\]
> leave current user from listed channels
> #### example:
> ```
> PART #ch1, &ch2
> ```
> #### responses:
> - ERR_NEEDMOREPARAMS
> - ERR_NOSUCHCHANNEL
> - ERR_NOTONCHANNEL

### MODE

### \[:USERNAME\] TOPIC #CHANNEL \[:NEWTOPIC\]
> command changes topic of channel `CHANNEL` to `NEWTOPIC` or get channel topic
> if no arguments provided
> #### example:
> ```
> TOPIC #ch1
> ```
> ```
> TOPIC &ch2 :new better topic
> ```
> ```
> :user TOPIC #ch1
> ```
> #### responses:
> - ERR_NEEDMOREPARAMS
> - ERR_NOTONCHANNEL
> - RPL_NOTOPIC
> - RPL_TOPIC
> - ERR_CHANOPRIVSNEEDED

### \[:USERNAME\] NAMES \[#CHANNEL1\] \[, &CHANNEL2\] \[...\]
> command list all visible (not private (+p), or secret (+s)) nicknames from
> listed channels, if no arguments provided - returned information about all
> online users. If user online and not in channel - his channel name is `*`
> #### example:
> ```
> NAMES #ch1
> ```
> ```
> NAMES
> ```
> ##### responses:
> - RPL_NAMREPLY
> - RPL_ENDOFNAMES

### \[:USERNAME\] LIST \[#CHANNEL1\] \[, &CHANNEL2\] \[...\]
> command list channels and their topics, if no arguments provided - listed
> all channels. Private channels are listed without their topics as channel
> `Prv` unless current client is not in this private channel, secret channels 
> are not listed at all, unless current client is not in this secret channel
> #### example:
> ```
> LIST #ch1
> ```
> ```
> LIST
> ```
> - ERR_NOSUCHSERVER
> - RPL_LISTSTART
> - RPL_LIST
> - RPL_LISTEND

### \[:USERNAME\] INVITE NICKNAME #CHANNEL
> command invite users to channel, there is no requirement that the channel
> `CHANNEL` must exist, if channel `CHANNEL` is invite only (mode +i), current
> user should be operator of channel `CHANNEL`
> #### example:
> ```
> INVITE myfriend #ch1
> ```
> ```
> :user INVITE another #ch1
> ```
> ##### responses:
> - ERR_NEEDMOREPARAMS
> - ERR_NOSUCHNICK
> - ERR_NOTONCHANNEL
> - ERR_USERONCHANNEL
> - ERR_CHANOPRIVSNEEDED
> - RPL_INVITING
> - RPL_AWAY


### \[:USERNAME\] KICK \#CHANNEL USERNAME \[REASON\]
> kick user `USERNAME` from channel `#CHANNEL` and if specified send him message
> `REASON`, command available only for operators, user `USERNAME` can join
> channel `CHANNEL` again (command `KICK` is forced command `PART`)

KILL USERNAME REASON
> send message `REASON` to user `USERNAME` and breaks connection with him, user
> `USERNAME` can login again

PRIVMSG USERNAME MESSAGE
> send private message `MESSAGE` to user `USERNAME`

