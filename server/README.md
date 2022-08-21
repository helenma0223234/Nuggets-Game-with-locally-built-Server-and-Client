# CS50 Final Project Nugget Game: Server Code

Written by: Kewen H.'23, Helen M.'23, So A.'23

This subdirectory includes code that implements the server of the Nugget Game.

The server utilizes modules from the support library, and mainly the game module (built along with the grid module) as specified in the
implementation spec [IMPLEMENTATION.md].

## server.c

The server code opens a port and awaits clients to connect;
then behave according to if the client is a player or a spectator.
Basic functionalities are explained in the design and requirement spec.
