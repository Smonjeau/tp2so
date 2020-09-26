/* Sets the RSP to context, thus switching the process context */

void switchContext(void *context);

/* Creates a context for a new process and returns its RSP */

void *createContext(int argc, char **argv, void *main);