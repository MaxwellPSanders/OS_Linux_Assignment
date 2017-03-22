This is a linux shell named msh, which stands for maverick shell.
It works like a normal shell using the fork-exec model of running processes.
This can be compiled using gcc on the linux command line.

The program uses some of my teachers code, which is why the MIT license is at the top of the code. That said most of the code of his, which I used as a template for some string parsing, I replaced since I preferred tokenizing over using strsep since it allowed for a large amount of whitespace between tokens, making the code less breakable.

Once it is compiled and you run it, the only way you can exit is by typing "exit" since it blocks both ctrl-c and z.
Once it is running the commands supported are:
Programs you have installed in the computer and their modifiers (example. ls -a)
Directory changing
up to 255 character inputs
10 tokens (example. ls -a -a -a -a -a -a -a -a -a)
typing showpid will show the last 10 processes spawned

