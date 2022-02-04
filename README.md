
# IRC C++ SERVER

## Server commands

JOIN \#CHANNEL1, \#CHANNEL2, \[...\]
> join to channels named by comas, if channel is not exist - creates channel and
> makes current user operator of new channel

KICK \#CHANNEL USERNAME \[REASON\]
> kick user `USERNAME` from channel `#CHANNEL` and if specified send him message
> `REASON`, command available only for operators, user `USERNAME` can join
> channel `CHANNEL` again

KILL USERNAME REASON
> send message `REASON` to user `USERNAME` and breaks connection with him, user
> `USERNAME` can login again

MSG USERNAME MESSAGE
> send private message `MESSAGE` to user `USERNAME`

NICK USERNAME
> change user nickname to `USERNAME`

OPER LOGIN PASSWORD
> make current user operator o
