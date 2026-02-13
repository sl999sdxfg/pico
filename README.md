```text                                   
__/\\\\\\\\\\\\\____/\\\\\\\\\\\________/\\\\\\\\\_______/\\\\\______        
 _\/\\\/////////\\\_\/////\\\///______/\\\////////______/\\\///\\\____       
  _\/\\\_______\/\\\_____\/\\\_______/\\\/_____________/\\\/__\///\\\__      
   _\/\\\\\\\\\\\\\/______\/\\\______/\\\______________/\\\______\//\\\_     
    _\/\\\/////////________\/\\\_____\/\\\_____________\/\\\_______\/\\\_    
     _\/\\\_________________\/\\\_____\//\\\____________\//\\\______/\\\__   
      _\/\\\_________________\/\\\______\///\\\___________\///\\\__/\\\____  
       _\/\\\______________/\\\\\\\\\\\____\////\\\\\\\\\____\///\\\\\/_____ 
        _\///______________\///////////________\/////////_______\/////_______
```                           
# Pico - a Linux shell that can do basic stuff I hope                                        

### Build and run:
```bash
make             # builds debug
make debug       # default for make
make release     # builds release
make run         # runs debug
make run-release # runs release
make clean       # cleans build files
```


### Todo:

- [ x ] Project setup & infrastructure — repo, README, LICENSE, CONTRIBUTING, build system (Makefile/CMake), dependencies, CI/testing setup.
- [ ] Step 1: REPL & input reading — interactive prompt + non-interactive script mode; line read and basic handling.
- [ ] Step 2: Tokenization / lexer — split input into tokens (words, operators, quotes, escapes) with error handling.
- [ ] Step 3: Parser / command structure — build command structs or AST (argv, redirections, pipes, job flags).
- [ ] Step 4: Execute external commands & builtins — fork/exec external programs, handle builtins like cd, exit, pwd, export.
- [ ] Step 5: I/O redirection & pipelines — implement >, >>, <, and pipelines (|) connecting processes.
- [ ] Step 6: Background jobs, signals & job control — support &, SIGINT/SIGTSTP handling, process groups, fg/bg.
- [ ] Step 7: Shell expansions & usability features — variable & command substitution, globbing, here-docs, control operators, history, prompt, aliases.
- [ ] Testing & examples — unit tests for lexer/parser, integration tests for pipelines/redirs/jobs, example .myshellrc, usage docs.

### Project structure
```text
.
├── build/          <- Compiled executables and intermediate build files
|   ├── debug/ 
|   ├── release/      
├── docs/           <- Documentation                              
├── src/            <- Source code files
├── tests/          <- Unit tests
├── LICENSE         <- License file
├── Makefile        <- Main project makefile
└── README.md       <- Project information, build/usage instructions
```